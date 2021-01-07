#pragma once
#include "OrcBehavior.h"
#include "Controller.h"
#include "..\Input Manager.h"
#include "..\EventManager.h"
#include "..\PhysicsManager.h"
#include "..\GameObject.h"
#include "Transform.h"
#include "Body.h"
#include "Sprite.h"
#include "Stats.h"
#include "AttackZoneBehavior.h"
#include "../GameObjectManager.h"
#include "..\\ResourceManager.h"
#include "../GraphicsManager.h"
#include "../Frame Rate Controller.h"



#include <math.h>
#include <iostream>
#include <fstream>
#include <streambuf>


extern Input_Manager* gInputManager;
extern EventManager* gEventManager;
extern GameObjectManager* gpGameObjectManager;
extern ResourceManager* gpResourceManager;
extern ResourceManager* gpResourceManager;
extern std::vector<std::vector<Sprite*>> spritesUsingEachSpritesheet;
extern Frame_Rate_Controller* gFrameRateController;

extern Transform* playerTransform;
extern Body* playerBody;

OrcBehavior::OrcBehavior() : Component(L"OrcBehavior")
{

}

OrcBehavior::~OrcBehavior()
{

}

void OrcBehavior::Serialize(std::wfstream& InputStream)
{
}

void OrcBehavior::Update()
{
	Stats* monsterStats = static_cast<Stats*>(mpOwner->GetComponent(L"Stats"));
	Body* monsterBody = static_cast<Body*>(mpOwner->GetComponent(L"Body"));
	Transform* monsterTransform = static_cast<Transform*>(mpOwner->GetComponent(L"Transform"));
	CharacterAnimations* monsterCharacterAnimation = static_cast<CharacterAnimations*>(mpOwner->GetComponent(L"CharacterAnimations"));

	if (nullptr != monsterTransform && nullptr != monsterCharacterAnimation)
	{
		action currentAction = frameList[monsterCharacterAnimation->currentFrame].currentAction;
		animation currentAnimation = frameList[monsterCharacterAnimation->currentFrame].currentAnimation;

		float xDistance = monsterTransform->positionX - playerTransform->positionX;
		float yDistance = monsterTransform->positionY - playerTransform->positionY;
		float totalDistance = sqrt(xDistance * xDistance + yDistance * yDistance);
		if (currentAction == Special || currentAction == Attack || currentAction == Struck)
			return;
		float attackXDistance = monsterBody->xScale;
		float attackYDistance = monsterBody->yScale;
		float totalAttackDistance = sqrt(attackXDistance * attackXDistance + attackYDistance * attackYDistance);
		//Finish the current movement animation if it is in one, unless this would pass the target.
		if (currentAnimation == MoveUp && yDistance < 0)
		{
			reactionDelay -= gFrameRateController->GetFrameTime() / 1000.0f;
			reactionDelay = std::fmax(reactionDelay, 0);
			monsterTransform->positionY += monsterStats->movementSpeed;
			return;
		}
		else if (currentAnimation == MoveRight && xDistance > 0)
		{
			reactionDelay -= gFrameRateController->GetFrameTime() / 1000.0f;
			reactionDelay = std::fmax(reactionDelay, 0);
			monsterTransform->positionX += monsterStats->movementSpeed;
			return;
		}
		else if (currentAnimation == MoveDown && yDistance > 0)
		{
			reactionDelay -= gFrameRateController->GetFrameTime() / 1000.0f;
			reactionDelay = std::fmax(reactionDelay, 0);
			monsterTransform->positionY -= monsterStats->movementSpeed;
			return;
		}
		else if (currentAnimation == MoveLeft && xDistance < 0)
		{
			reactionDelay -= gFrameRateController->GetFrameTime() / 1000.0f;
			reactionDelay = std::fmax(reactionDelay, 0);
			monsterTransform->positionX -= monsterStats->movementSpeed;
			return;
		}

		//get closer
		else if (totalDistance >= totalAttackDistance && (abs(xDistance) >= abs(yDistance)) && xDistance <= 0)
		{
			if (currentAnimation != MoveUp && currentAnimation != MoveLeft && currentAnimation != MoveDown)
			{
				reactionDelay -= gFrameRateController->GetFrameTime() / 1000.0f;
				reactionDelay = std::fmax(reactionDelay, 0);
				monsterTransform->positionX += monsterStats->movementSpeed;
				if (currentAnimation != MoveRight)
				{
					monsterCharacterAnimation->SetAnimation(MoveRight);
				}
				return;
			}
		}
		else if (totalDistance >= totalAttackDistance && (abs(xDistance) >= abs(yDistance)) && xDistance >= 0)
		{
			if (currentAnimation != MoveUp && currentAnimation != MoveRight && currentAnimation != MoveDown)
			{
				reactionDelay -= gFrameRateController->GetFrameTime() / 1000.0f;
				reactionDelay = std::fmax(reactionDelay, 0);
				monsterTransform->positionX -= monsterStats->movementSpeed;
				if (currentAnimation != MoveLeft)
				{
					monsterCharacterAnimation->SetAnimation(MoveLeft);
				}
				return;
			}
		}

		else  if (totalDistance >= totalAttackDistance && (abs(yDistance) >= abs(xDistance)) && yDistance <= 0)
		{
			if (currentAnimation != MoveDown && currentAnimation != MoveRight && currentAnimation != MoveLeft)
			{
				reactionDelay -= gFrameRateController->GetFrameTime() / 1000.0f;
				reactionDelay = std::fmax(reactionDelay, 0);
				monsterTransform->positionY += monsterStats->movementSpeed;
				if (currentAnimation != MoveUp)
				{
					monsterCharacterAnimation->SetAnimation(MoveUp);
				}
				return;
			}
		}
		else if (totalDistance >= totalAttackDistance && (abs(yDistance) >= abs(xDistance)) && yDistance >= 0)
		{
			if (currentAnimation != MoveRight && currentAnimation != MoveLeft && currentAnimation != MoveUp)
			{
				reactionDelay -= gFrameRateController->GetFrameTime() / 1000.0f;
				reactionDelay = std::fmax(reactionDelay, 0);
				monsterTransform->positionY -= monsterStats->movementSpeed;
				if (currentAnimation != MoveDown)
				{
					monsterCharacterAnimation->SetAnimation(MoveDown);
				}
				return;
			}
		}

		else
		{
			reactionDelay += gFrameRateController->GetFrameTime() / 1000.0f;
			if (reactionDelay < monsterStats->reactionDelay)
				return;
			reactionDelay = 0;
			//Attack
			if (totalDistance <= totalAttackDistance && (abs(yDistance) <= abs(xDistance)) && xDistance >= 0)
			{
				std::cout << "Attack left! X distance: " << xDistance << std::endl;
				monsterCharacterAnimation->SetAnimation(AttackLeft);
			}
			else if (totalDistance <= totalAttackDistance && (abs(yDistance) <= abs(xDistance)) && xDistance <= 0)
			{
				std::cout << "Attack right! X distance: " << xDistance << std::endl;
				monsterCharacterAnimation->SetAnimation(AttackRight);
			}
			else if (totalDistance <= totalAttackDistance && (abs(xDistance) <= abs(yDistance)) && yDistance >= 0)
			{
				std::cout << "Attack down! Y distance: " << yDistance << std::endl;
				monsterCharacterAnimation->SetAnimation(AttackDown);
			}
			else if (totalDistance <= totalAttackDistance && (abs(xDistance) <= abs(yDistance)) && yDistance <= 0)
			{
				std::cout << "Attack up! Y distance: " << yDistance << std::endl;
				monsterCharacterAnimation->SetAnimation(AttackUp);
			}
		}
	}

}




void OrcBehavior::HandleEvent(Event* pEvent)
{

	if (EventType::COLLIDE == pEvent->mType)
	{
		CollideEvent* pCollideEvent = static_cast<CollideEvent*>(pEvent);
		GameObject* otherCollider;
		if (pCollideEvent->mGameObjects[0] == this->mpOwner)
			otherCollider = pCollideEvent->mGameObjects[1];
		else
			otherCollider = pCollideEvent->mGameObjects[0];
		if (otherCollider->objectGroup == L"AttackZone")
		{
			return;
		}

		CharacterAnimations* pCharacterAnimations = static_cast<CharacterAnimations*>(mpOwner->GetComponent(L"CharacterAnimations"));
		if (nullptr == pCharacterAnimations)
		{
			return;
		}

		action currentAction = frameList[pCharacterAnimations->currentFrame].currentAction;
		animation currentAnimation = frameList[pCharacterAnimations->currentFrame].currentAnimation;


		Body* pBody = static_cast<Body*>(mpOwner->GetComponent(L"Body"));
		Transform* pTransform = static_cast<Transform*>(mpOwner->GetComponent(L"Transform"));
		if ((nullptr != pBody) & (nullptr != pTransform))
		{

			Transform* otherTransform = static_cast<Transform*>(otherCollider->GetComponent(L"Transform"));
			if (nullptr != otherTransform)


				if (otherTransform->positionX > pTransform->positionX)
				{
					pTransform->positionX -= 10;
					pBody->velocityX = -10;
					pBody->totalForceX = -1000;
				}
				else if (otherTransform->positionX < pTransform->positionX)
				{
					pTransform->positionX += 10;
					pBody->velocityX = 10;
					pBody->totalForceX = 1000;
				}

			if (otherTransform->positionY > pTransform->positionY)
			{
				pTransform->positionY -= 10;
				pBody->velocityY = -10;
				pBody->totalForceY = -1000;
			}
			else if (otherTransform->positionY < pTransform->positionY)
			{
				pTransform->positionY += 10;
				pBody->velocityY = 10;
				pBody->totalForceY = 1000;
			}
		}
	}
	else if (EventType::ATTACK_ZONE_HIT == pEvent->mType)
	{
		AttackZoneHitEvent* pAttackZoneHitEvent = static_cast<AttackZoneHitEvent*>(pEvent);


		Body* pBody = static_cast<Body*>(mpOwner->GetComponent(L"Body"));
		if (nullptr != pBody)
		{
			if (Up == pAttackZoneHitEvent->direction)
			{
				pBody->totalForceY += pAttackZoneHitEvent->pushbackForce;
			}
			else if (Right == pAttackZoneHitEvent->direction)
			{
				pBody->totalForceX += pAttackZoneHitEvent->pushbackForce;

			}
			else if (Down == pAttackZoneHitEvent->direction)
			{
				pBody->totalForceY -= pAttackZoneHitEvent->pushbackForce;
			}
			else if (Left == pAttackZoneHitEvent->direction)
			{
				pBody->totalForceX -= pAttackZoneHitEvent->pushbackForce;
			}
			else
			{
				Body* pOtherBody = static_cast<Body*>(pAttackZoneHitEvent->attacker->GetComponent(L"Body"));
				if (nullptr != pOtherBody)
				{
					float otherX = pOtherBody->positionX;
					float otherY = pOtherBody->positionY;
					float forceAngle = atan2(otherY - pBody->positionY, otherX - pBody->positionX);
					pBody->totalForceX -= cos(forceAngle) * pAttackZoneHitEvent->pushbackForce;
					pBody->totalForceY -= sin(forceAngle) * pAttackZoneHitEvent->pushbackForce;
				}
			}
		}
		CharacterAnimations* pCharacterAnimations = static_cast<CharacterAnimations*>(mpOwner->GetComponent(L"CharacterAnimations"));
		if (nullptr != pCharacterAnimations)
		{
			pCharacterAnimations->SetAction(Struck);
		}

		Stats* pStats = static_cast<Stats*>(mpOwner->GetComponent(L"Stats"));
		if (nullptr != pStats)
		{
			pStats->health -= pAttackZoneHitEvent->damage;
			if (pStats->health <= 0)
			{
				gpGameObjectManager->mGameObjectsToDelete.push_back(this->mpOwner);
			}
		}

	}
	else if (EventType::ANIMATION_FRAME_REACHED == pEvent->mType)
	{
		AnimationFrameReachedEvent* pAnimationFrameReachedEvent = static_cast<AnimationFrameReachedEvent*>(pEvent);
		if (AttackUp2 == pAnimationFrameReachedEvent->frameReached || AttackRight2 == pAnimationFrameReachedEvent->frameReached || AttackDown2 == pAnimationFrameReachedEvent->frameReached || AttackLeft2 == pAnimationFrameReachedEvent->frameReached)
		{
			direction attackDirection = frameList[pAnimationFrameReachedEvent->frameReached].facing;

			Transform* attackerTransform = static_cast<Transform*>(mpOwner->GetComponent(L"Transform"));
			Body* attackerBody = static_cast<Body*>(mpOwner->GetComponent(L"Body"));
			GameObject* attackObject = new GameObject();
			attackObject->objectType = L"AttackZone";
			attackObject->objectGroup = L"AttackZone";
			attackObject->AddComponent(L"Transform");
			attackObject->AddComponent(L"Body");

			float newPositionX = attackerBody->positionX;
			float newPositionY = attackerBody->positionY;

			float xScale = attackerBody->xScale;
			float yScale = attackerBody->yScale;
			float zScale = 1;
			float attackForward = 1;
			float attackSides = 1;

			if (attackDirection == Up)
			{
				yScale *= attackForward;
				xScale *= attackSides;
				newPositionY += yScale / 2 + attackerBody->yScale / 2;
			}
			else if (attackDirection == Right)
			{
				yScale *= attackSides;
				xScale *= attackForward;
				newPositionX += xScale / 2 + attackerBody->xScale / 2;
			}
			else if (attackDirection == Down)
			{
				yScale *= attackForward;
				xScale *= attackSides;
				newPositionY -= (yScale / 2 + attackerBody->yScale / 2);
			}
			else if (attackDirection == Left)
			{
				yScale *= attackSides;
				xScale *= attackForward;
				newPositionX -= (xScale / 2 + attackerBody->xScale / 2);
			}
			Transform* attackTransform = static_cast<Transform*>(attackObject->GetComponent(L"Transform"));
			attackTransform->positionX = newPositionX;
			attackTransform->positionY = newPositionY;

			float newMass = 0;

			std::wstring shapeName = L"AABB";
			int VAOID = 1;//rectangle

			Body* attackBody = static_cast<Body*>(attackObject->GetComponent(L"Body"));
			attackBody->Initialize(xScale, yScale, newMass, shapeName, VAOID);
			attackBody->positionX = newPositionX;
			attackBody->positionY = newPositionY;

			attackObject->AddComponent(L"Sprite");
			Sprite* attackSprite = static_cast<Sprite*>(attackObject->GetComponent(L"Sprite"));
			{
				attackSprite->spriteSheetTextureID = gpResourceManager->ParseSpritesheet(L"Ward.png");
				attackSprite->textureRow = 1;
				attackSprite->textureColumn = 1;
				attackSprite->VAOID = 1;
			}
			if ((attackSprite->spriteSheetTextureID + 1) > spritesUsingEachSpritesheet.size())
			{
				spritesUsingEachSpritesheet.resize(attackSprite->spriteSheetTextureID + 1);
			}
			spritesUsingEachSpritesheet[attackSprite->spriteSheetTextureID].push_back(attackSprite);
			attackObject->AddComponent(L"AttackZoneBehavior");
			AttackZoneBehavior* attackZoneB = static_cast<AttackZoneBehavior*>(attackObject->GetComponent(L"AttackZoneBehavior"));
			int newDamage = 10;
			float newPushbackForce = 10;
			int newDirection = attackDirection;
			Stats* pStats = static_cast<Stats*>(mpOwner->GetComponent(L"Stats"));
			if (nullptr != pStats)
			{
				newDamage = pStats->attackDamage;
				newPushbackForce = pStats->attackPushbackForce;
			}
			CharacterAnimations* pCharacterAnimations = static_cast<CharacterAnimations*>(attackObject->GetComponent(L"CharacterAnimations"));
			if (nullptr != pCharacterAnimations)
			{
				newDirection = frameList[pCharacterAnimations->currentFrame].facing;
			}

			attackZoneB->Initialize(this->mpOwner, newDamage, newPushbackForce, newDirection, 0.25, false);
			gpGameObjectManager->mGameObjectsToCreate.push_back(attackObject);
		}
	}
}