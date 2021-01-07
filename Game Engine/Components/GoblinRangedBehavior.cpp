#pragma once
#include "GoblinRangedBehavior.h"
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
#include "ArrowBehavior.h"
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

GoblinRangedBehavior::GoblinRangedBehavior() : Component(L"GoblinRangedBehavior")
{

}

GoblinRangedBehavior::~GoblinRangedBehavior()
{

}

void GoblinRangedBehavior::Serialize(std::wfstream& InputStream)
{
}

void GoblinRangedBehavior::Update()
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

		//Finish the current movement animation if it is in one.
		if (currentAnimation == MoveUp)
		{
			monsterTransform->positionY += monsterStats->movementSpeed;
			return;
		}
		else if (currentAnimation == MoveRight)
		{
			monsterTransform->positionX += monsterStats->movementSpeed;
			return;
		}
		else if (currentAnimation == MoveDown)
		{
			monsterTransform->positionY -= monsterStats->movementSpeed;
			return;
		}
		else if (currentAnimation == MoveLeft)
		{
			monsterTransform->positionX -= monsterStats->movementSpeed;
			return;
		}

		//Run away.
		else if (totalDistance <= 500 && (abs(xDistance) <= abs(yDistance)) && xDistance <= 0)
		{
			if (currentAnimation != MoveUp && currentAnimation != MoveRight && currentAnimation != MoveDown)
			{
				monsterTransform->positionX -= monsterStats->movementSpeed;
				if (currentAnimation != MoveLeft)
				{
					monsterCharacterAnimation->SetAnimation(MoveLeft);
				}
				return;
			}
		}
		else if (totalDistance <= 500 && (abs(xDistance) <= abs(yDistance)) && xDistance >= 0)
		{
			if (currentAnimation != MoveUp && currentAnimation != MoveLeft && currentAnimation != MoveDown)
			{
				monsterTransform->positionX += monsterStats->movementSpeed;
				if (currentAnimation != MoveRight)
				{
					monsterCharacterAnimation->SetAnimation(MoveRight);
				}
				return;
			}
		}

		if (totalDistance <= 500 && (abs(yDistance) <= abs(xDistance)) && yDistance <= 0)
		{
			if (currentAnimation != MoveUp && currentAnimation != MoveRight && currentAnimation != MoveLeft)
			{
				monsterTransform->positionY -= monsterStats->movementSpeed;
				if (currentAnimation != MoveDown)
				{
					monsterCharacterAnimation->SetAnimation(MoveDown);
				}
				return;
			}
		}
		else if (totalDistance <= 500 && (abs(yDistance) <= abs(xDistance)) && yDistance >= 0)
		{
			if (currentAnimation != MoveRight && currentAnimation != MoveLeft && currentAnimation != MoveDown)
			{
				monsterTransform->positionY += monsterStats->movementSpeed;
				if (currentAnimation != MoveUp)
				{
					monsterCharacterAnimation->SetAnimation(MoveUp);
				}
				return;
			}
		}


		//get closer
		else if (totalDistance >= 600 && (abs(xDistance) >= abs(yDistance)) && xDistance <= 0)
		{
			if (currentAnimation != MoveUp && currentAnimation != MoveLeft && currentAnimation != MoveDown)
			{
				monsterTransform->positionX += monsterStats->movementSpeed;
				if (currentAnimation != MoveRight)
				{
					monsterCharacterAnimation->SetAnimation(MoveRight);
				}
				return;
			}
		}
		else if (totalDistance >= 600 && (abs(xDistance) >= abs(yDistance)) && xDistance >= 0)
		{
			if (currentAnimation != MoveUp && currentAnimation != MoveRight && currentAnimation != MoveDown)
			{
				monsterTransform->positionX -= monsterStats->movementSpeed;
				if (currentAnimation != MoveLeft)
				{
					monsterCharacterAnimation->SetAnimation(MoveLeft);
				}
				return;
			}
		}

		else  if (totalDistance >= 600 && (abs(yDistance) >= abs(xDistance)) && yDistance <= 0)
		{
			if (currentAnimation != MoveDown && currentAnimation != MoveRight && currentAnimation != MoveLeft)
			{
				monsterTransform->positionY += monsterStats->movementSpeed;
				if (currentAnimation != MoveUp)
				{
					monsterCharacterAnimation->SetAnimation(MoveUp);
				}
				return;
			}
		}
		else if (totalDistance >= 600 && (abs(yDistance) >= abs(xDistance)) && yDistance >= 0)
		{
			if (currentAnimation != MoveRight && currentAnimation != MoveLeft && currentAnimation != MoveUp)
			{
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
			//Fire an arrow.
			if (totalDistance >= 300 && (abs(yDistance) <= abs(xDistance)) && xDistance >= 0)
			{
				std::cout << "Attack left! X distance: " << xDistance << std::endl;
				monsterCharacterAnimation->SetAnimation(AttackLeft);
			}
			else if (totalDistance >= 300 && (abs(yDistance) <= abs(xDistance)) && xDistance <= 0)
			{
				std::cout << "Attack right! X distance: " << xDistance << std::endl;
				monsterCharacterAnimation->SetAnimation(AttackRight);
			}
			else if (totalDistance >= 300 && (abs(xDistance) <= abs(yDistance)) && yDistance >= 0)
			{
				std::cout << "Attack down! Y distance: " << yDistance << std::endl;
				monsterCharacterAnimation->SetAnimation(AttackDown);
			}
			else if (totalDistance >= 300 && (abs(xDistance) <= abs(yDistance)) && yDistance <= 0)
			{
				std::cout << "Attack up! Y distance: " << yDistance << std::endl;
				monsterCharacterAnimation->SetAnimation(AttackUp);
			}
		}
	}
}




void GoblinRangedBehavior::HandleEvent(Event* pEvent)
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
	AnimationFrameReachedEvent* pAnimationFrameReachedEvent = static_cast<AnimationFrameReachedEvent*>(pEvent);
	if (AttackUp2 == pAnimationFrameReachedEvent->frameReached || AttackRight2 == pAnimationFrameReachedEvent->frameReached || AttackDown2 == pAnimationFrameReachedEvent->frameReached || AttackLeft2 == pAnimationFrameReachedEvent->frameReached)
	{
		direction attackDirection = frameList[pAnimationFrameReachedEvent->frameReached].facing;

		Transform* attackerTransform = static_cast<Transform*>(mpOwner->GetComponent(L"Transform"));
		Body* attackerBody = static_cast<Body*>(mpOwner->GetComponent(L"Body"));
		GameObject* attackObject = new GameObject();
		attackObject->objectType = L"Arrow";
		attackObject->objectGroup = L"Arrow";
		attackObject->AddComponent(L"Transform");
		attackObject->AddComponent(L"Body");
		attackObject->AddComponent(L"Sprite");
		attackObject->AddComponent(L"AttackZoneBehavior");
		attackObject->AddComponent(L"ArrowBehavior");

		float newPositionX = attackerBody->positionX;
		float newPositionY = attackerBody->positionY;

		float xScale = 20;//Adjustable attack parameters.
		float yScale = 75;
		float zScale = 1;

		if (attackDirection == Up)
		{
			newPositionY += yScale / 2 + attackerBody->yScale / 2;
		}
		else if (attackDirection == Right)
		{
			newPositionX += xScale / 2 + attackerBody->xScale / 2;
		}
		else if (attackDirection == Down)
		{
			newPositionY -= (yScale / 2 + attackerBody->yScale / 2);
		}
		else if (attackDirection == Left)
		{
			newPositionX -= (xScale / 2 + attackerBody->xScale / 2);
		}
		Transform* attackTransform = static_cast<Transform*>(attackObject->GetComponent(L"Transform"));
		attackTransform->positionX = newPositionX;
		attackTransform->positionY = newPositionY;

		attackTransform->scaleX = xScale;
		attackTransform->scaleY = yScale;
		attackTransform->scaleZ = zScale;

		float newMass = 0;

		std::wstring shapeName = L"AABB";
		int VAOID = 1;//rectangle

		Body* attackBody = static_cast<Body*>(attackObject->GetComponent(L"Body"));
		attackBody->Initialize(xScale, yScale, newMass, shapeName, VAOID);
		attackBody->positionX = newPositionX;
		attackBody->positionY = newPositionY;

		Sprite* attackSprite = static_cast<Sprite*>(attackObject->GetComponent(L"Sprite"));
		{
			attackSprite->spriteSheetTextureID = gpResourceManager->ParseSpritesheet(L"Projectile_Arrow.png");
			attackSprite->textureRow = 1;
			attackSprite->textureColumn = 1;
			attackSprite->VAOID = 5;

		}
		if ((attackSprite->spriteSheetTextureID + 1) > spritesUsingEachSpritesheet.size())
		{
			spritesUsingEachSpritesheet.resize(attackSprite->spriteSheetTextureID + 1);
		}
		spritesUsingEachSpritesheet[attackSprite->spriteSheetTextureID].push_back(attackSprite);


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

		attackZoneB->Initialize(this->mpOwner, newDamage, newPushbackForce, Omnidirectional, 8000, true);


		Transform* monsterTransform = static_cast<Transform*>(mpOwner->GetComponent(L"Transform"));
		ArrowBehavior* arrowB = static_cast<ArrowBehavior*>(attackObject->GetComponent(L"ArrowBehavior"));
		float inaccuracyX = 0;
		float inaccuracyY = 0;
		if (!(projectileAccuracy > RAND_MAX))
		{
			inaccuracyX = (std::rand() % (RAND_MAX - (int)projectileAccuracy)) - ((RAND_MAX - projectileAccuracy) / 2);
			inaccuracyY = (std::rand() % (RAND_MAX - (int)projectileAccuracy)) - ((RAND_MAX - projectileAccuracy) / 2);
		}
		float xDistance = -(monsterTransform->positionX - playerTransform->positionX) + inaccuracyX;
		float yDistance = -(monsterTransform->positionY - playerTransform->positionY) + inaccuracyY;
		float totalDistance = sqrt(xDistance * xDistance + yDistance * yDistance);
		arrowB->xSpeed = projectileSpeed * (xDistance / totalDistance);
		arrowB->ySpeed = projectileSpeed * (yDistance / totalDistance);

		attackTransform->rotation = - (std::atan2(yDistance, xDistance) * 180 / 3.14159) -90;  //atan2 gives counterclockwise radians from right. converting to degrees clockwise from up.
		if (attackTransform->rotation > 360)
			attackTransform->rotation -= 360;
		if (attackTransform->rotation < 360)
			attackTransform->rotation += 360;

		gpGameObjectManager->mGameObjectsToCreate.push_back(attackObject);
	}
}