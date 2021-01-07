#include "AttackZoneBehavior.h"
#include "Controller.h"
#include "..\Input Manager.h"
#include "..\EventManager.h"
#include "..\PhysicsManager.h"
#include "..\GameObject.h"
#include "Transform.h"
#include "Body.h"
#include "Sprite.h"
#include "../GameObjectManager.h"
#include "..\\ResourceManager.h"
#include "../GraphicsManager.h"
#include "../Frame Rate Controller.h"



#include <iostream>
#include <fstream>
#include <streambuf>

extern Frame_Rate_Controller* gFrameRateController;
extern Input_Manager* gInputManager;
extern EventManager* gEventManager;
extern GameObjectManager* gpGameObjectManager;
extern ResourceManager* gpResourceManager;
extern ResourceManager* gpResourceManager;
extern std::vector<std::vector<Sprite*>> spritesUsingEachSpritesheet;
extern Frame_Rate_Controller* gFrameRateController;


AttackZoneHitEvent::AttackZoneHitEvent() : Event(EventType::ATTACK_ZONE_HIT)
{

}

AttackZoneHitEvent::AttackZoneHitEvent(int newDamage, float newPushbackForce, int newDirection, GameObject* newAttacker) : Event(EventType::ATTACK_ZONE_HIT)
{
	damage = newDamage;
	pushbackForce = newPushbackForce;
	direction = newDirection;
	attacker = newAttacker;
}

AttackZoneHitEvent::~AttackZoneHitEvent()
{

}


AttackZoneBehavior::AttackZoneBehavior() : Component(L"AttackZoneBehavior")
{
	attacker = nullptr;
}

void AttackZoneBehavior::Initialize(GameObject* newAttacker, int newDamage, float newPushbackForce, int newDirection, float newDuration, bool newDestroyThisObjectOnHit)
{
	attacker = newAttacker;
	damage = newDamage;
	pushbackForce = newPushbackForce;
	direction = newDirection;
	duration = newDuration;
	destroyThisObjectOnHit = newDestroyThisObjectOnHit;
}

AttackZoneBehavior::~AttackZoneBehavior()
{

}

void AttackZoneBehavior::Serialize(std::wfstream& InputStream)
{
}





void AttackZoneBehavior::Update()
{
	timeSinceCreation += gFrameRateController->GetFrameTime();
	if (timeSinceCreation > duration)
		gpGameObjectManager->mGameObjectsToDelete.push_back(this->mpOwner);
}




void AttackZoneBehavior::HandleEvent(Event* pEvent)
{

	if (EventType::COLLIDE == pEvent->mType)
	{
		CollideEvent* pCollideEvent = static_cast<CollideEvent*>(pEvent);


			GameObject* otherCollider;
			if (pCollideEvent->mGameObjects[0] == this->mpOwner)
				otherCollider = pCollideEvent->mGameObjects[1];
			else
				otherCollider = pCollideEvent->mGameObjects[0];

			if (otherCollider->objectGroup == L"Character" && otherCollider !=attacker)
			{

				AttackZoneHitEvent* pAZHE = new AttackZoneHitEvent(damage, pushbackForce, direction, this->mpOwner);
				otherCollider->HandleEvent(pAZHE); //this will delete the new event after firing it.
				if (destroyThisObjectOnHit)
				{
					gpGameObjectManager->mGameObjectsToDelete.push_back(this->mpOwner);
				}
			}
		
	}
}