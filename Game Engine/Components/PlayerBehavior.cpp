#include "PlayerBehavior.h"
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




#include <iostream>
#include <fstream>
#include <streambuf>


extern Input_Manager* gInputManager;
extern EventManager* gEventManager;
extern GameObjectManager* gpGameObjectManager;
extern ResourceManager* gpResourceManager;
extern ResourceManager* gpResourceManager;
extern std::vector<std::vector<Sprite*>> spritesUsingEachSpritesheet;

extern float windowWidth;
extern float windowHeight;

PlayerHitEvent::PlayerHitEvent() : Event(EventType::PLAYER_HIT)
{

}

PlayerHitEvent::~PlayerHitEvent()
{

}


PlayerBehavior::PlayerBehavior() : Component(L"PlayerBehavior")
{

}

PlayerBehavior::~PlayerBehavior()
{

}

void PlayerBehavior::Serialize(std::wfstream& InputStream)
{
	gEventManager->Subscribe(EventType::MOUSE_CLICK, this->mpOwner);
}

void PlayerBehavior::Update()
{

}




void PlayerBehavior::HandleEvent(Event* pEvent)
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
			{
				float otherX = otherTransform->positionX;
				float otherY = otherTransform->positionY;
				float forceAngle = atan2(otherY - pBody->positionY, otherX - pBody->positionX);
				pBody->totalForceX -= cos(forceAngle) * 10;
				pBody->totalForceY -= sin(forceAngle) * 10;
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

			float newPositionX = attackerTransform->positionX;
			float newPositionY = attackerTransform->positionY;

			float xScale = 100;//Adjustable attack parameters.
			float yScale = 100;
			float zScale = 1;
			float attackForward = 0.6;
			float attackSides = 1.0;

			if (attackDirection == Up)
			{
				yScale *= attackForward;
				xScale *= attackSides;
				newPositionY += yScale / 2 + attackerBody->yScale / 2.2;
			}
			else if (attackDirection == Right)
			{
				yScale *= attackSides;
				xScale *= attackForward;
				newPositionX += xScale / 2 + attackerBody->xScale / 2.2;
			}
			else if (attackDirection == Down)
			{
				yScale *= attackForward;
				xScale *= attackSides;
				newPositionY -= (yScale / 2 + attackerBody->yScale / 2.2);
			}
			else if (attackDirection == Left)
			{
				yScale *= attackSides;
				xScale *= attackForward;
				newPositionX -= (xScale / 2 + attackerBody->xScale / 2.2);
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
	else if (EventType::MOUSE_CLICK == pEvent->mType)
	{
		Stats* pStats = static_cast<Stats*>(mpOwner->GetComponent(L"Stats"));
		if (nullptr == pStats)
			return;
		direction attackDirection = Omnidirectional;
		MouseClickEvent* pMouseClickEvent = static_cast<MouseClickEvent*>(pEvent);
		CharacterAnimations* pCharacterAnimations = static_cast<CharacterAnimations*>(mpOwner->GetComponent(L"CharacterAnimations"));
		if (nullptr == pCharacterAnimations)
		{
			return;
		}
		action currentAction = frameList[pCharacterAnimations->currentFrame].currentAction;
		animation currentAnimation = frameList[pCharacterAnimations->currentFrame].currentAnimation;
		if (pStats->mana < 20)
			return;
		else
			pStats->mana -= 20;
		pCharacterAnimations->SetAnimation(SpecialDown);
		float xCoordinate;
		float yCoordinate;
		if (pMouseClickEvent->button == 1) //left
		{
			xCoordinate = pMouseClickEvent->xCoordinate/windowWidth;
			yCoordinate = pMouseClickEvent->yCoordinate/windowHeight;
		}
		else if (pMouseClickEvent->button == 3) //right
		{
			Transform* pTransform = static_cast<Transform*>(mpOwner->GetComponent(L"Transform"));
			xCoordinate = pTransform->positionX;
			yCoordinate = pTransform->positionY;
		}

		Component* pNewComponent = nullptr;

		Transform* pTransform = static_cast<Transform*>(mpOwner->GetComponent(L"Transform"));
		GameObject* attackObject = new GameObject();
		attackObject->objectType = L"AttackZone";
		attackObject->objectGroup = L"AttackZone";
		pNewComponent = attackObject->AddComponent(L"Transform");
		if (nullptr != pNewComponent)
		{
			Transform* attackTransform = static_cast<Transform*>(attackObject->GetComponent(L"Transform"));

			float newPositionX = pTransform->positionX;
			float newPositionY = pTransform->positionY;

			if(1 == pMouseClickEvent->button) //left mouse button
			{
				float mouseRelativeX = (pMouseClickEvent->xCoordinate - windowWidth/2);
				float mouseRelativeY = (-pMouseClickEvent->yCoordinate + windowHeight/2);


				newPositionX = pTransform->positionX + mouseRelativeX;
				newPositionY = pTransform->positionY + mouseRelativeY;
			}
			else if (3 == pMouseClickEvent->button) //right mouse button
			{
				newPositionX = pTransform->positionX;
				newPositionY = pTransform->positionY;
			}
			std::cout << "Mouse Coordinate X: " << pMouseClickEvent->xCoordinate << " Final left X: " << newPositionX << " Mouse Coordinate Y: " << pMouseClickEvent->yCoordinate << " Final left Y: " << newPositionY << std::endl;

			attackTransform->positionX = newPositionX;
			attackTransform->positionY = newPositionY;
			attackTransform->positionZ = -20;
			attackTransform->rotation = 0;//degrees clockwise
			attackTransform->scaleX = 200;
			attackTransform->scaleY = 200;
			attackTransform->scaleZ = 1;

			float newMass = 0;
			std::wstring shapeName = L"Circle";

			int VAOID = 4;//Circle

			pNewComponent = attackObject->AddComponent(L"Body");
			if (nullptr != pNewComponent)
			{
				Body* attackBody = static_cast<Body*>(attackObject->GetComponent(L"Body"));
				if (nullptr != attackBody)
				{
					attackBody->Initialize(200, 200, newMass, shapeName, VAOID);
				}
			}
		}
		pNewComponent = attackObject->AddComponent(L"Sprite");
		if (nullptr != pNewComponent)
		{
			Sprite* attackSprite = static_cast<Sprite*>(attackObject->GetComponent(L"Sprite"));
			{
				attackSprite->spriteSheetTextureID = gpResourceManager->ParseSpritesheet(L"Ward.png");
				attackSprite->textureRow = 1;
				attackSprite->textureColumn = 1;
				attackSprite->VAOID = 4; //Circular AttackZone VAO

			}
			if ((attackSprite->spriteSheetTextureID + 1) > spritesUsingEachSpritesheet.size())
			{
				spritesUsingEachSpritesheet.resize(attackSprite->spriteSheetTextureID + 1);
			}
			spritesUsingEachSpritesheet[attackSprite->spriteSheetTextureID].push_back(attackSprite);
		}
		pNewComponent = attackObject->AddComponent(L"AttackZoneBehavior");
		if (nullptr != pNewComponent)
		{
			AttackZoneBehavior* attackZoneB = static_cast<AttackZoneBehavior*>(attackObject->GetComponent(L"AttackZoneBehavior"));
			int newDamage = 10;
			float newPushbackForce = 10;
			int newDirection = attackDirection;
			int wardDuration = 10000;
			Stats* pStats = static_cast<Stats*>(mpOwner->GetComponent(L"Stats"));
			if (nullptr != pStats)
			{
				newDamage = pStats->attackDamage / 10;  //Less damaging than normal attacks
				newPushbackForce = pStats->attackPushbackForce * 10;  //but with more pushback
			}
			CharacterAnimations* pCharacterAnimations = static_cast<CharacterAnimations*>(attackObject->GetComponent(L"CharacterAnimations"));

			attackZoneB->Initialize(this->mpOwner, newDamage, newPushbackForce, Omnidirectional, wardDuration, false);
		}
		gpGameObjectManager->mGameObjectsToCreate.push_back(attackObject);
	}
}