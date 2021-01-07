#pragma once
#include "PhysicsManager.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "Components/Body.h"
#include "CollisionManager.h"

extern GameObjectManager* gpGameObjectManager;
extern CollisionManager* gCollisionManager;

CollideEvent::CollideEvent() : Event(EventType::COLLIDE)
{
	 
}

CollideEvent::~CollideEvent()
{

}

PhysicsManager::PhysicsManager()
{

}
PhysicsManager::~PhysicsManager()
{

}
void PhysicsManager::Update(float frameTime)
{
	//Integrate the bodies.
	for (auto gameObject : gpGameObjectManager->mGameObjects)
	{
		Body* body = static_cast<Body*>(gameObject->GetComponent(L"Body"));
		if (nullptr != body)
		{
			body->Integrate(0.1f, frameTime);
		}
	}

	//Reset previous contacts
	gCollisionManager->Reset();

	for (auto pObject1 = gpGameObjectManager->mGameObjects.begin(); pObject1 != gpGameObjectManager->mGameObjects.end(); ++pObject1)
	{
		Body* pBody1 = static_cast<Body*>((*pObject1)->GetComponent(L"Body"));
		if (nullptr == pBody1)
			continue;

		for (auto pObject2 = pObject1 + 1; pObject2 != gpGameObjectManager->mGameObjects.end(); ++pObject2)
		{
			Body* pBody2 = static_cast<Body*>((*pObject2)->GetComponent(L"Body"));
			if (nullptr == pBody2)
				continue;
			if ((*pObject1)->objectGroup == L"UI" || (*pObject2)->objectGroup == L"UI")
				continue;
			if ((*pObject1)->objectGroup == L"PlayerOnly" && (*pObject2)->objectType != L"Player" || (*pObject2)->objectGroup == L"PlayerOnly" && (*pObject1)->objectType != L"Player") //Only players can touch some objects.
				continue;
			if (((*pObject1)->objectGroup == L"Scenery" && (*pObject2)->objectGroup != L"Character") || ((*pObject2)->objectGroup == L"Scenery" && (*pObject1)->objectGroup != L"Character")) //Scenery only collides with characters.
				continue;
			if (((*pObject1)->objectGroup == L"AttackZone" && (*pObject2)->objectGroup != L"Character") || ((*pObject2)->objectGroup == L"AttackZone" && (*pObject1)->objectGroup != L"Character")) //Attack zones only collide with characters.
				continue;

			gCollisionManager->CheckCollisionAndGenerateContact(pBody1->shape, pBody1->positionX, pBody1->positionY, pBody2->shape, pBody2->positionX, pBody2->positionY);
		}
	}

	for (auto pContact : gCollisionManager->contacts)
	{
		CollideEvent colllisionEvent;

		colllisionEvent.mGameObjects[0] = pContact->bodies[0]->mpOwner;
		colllisionEvent.mGameObjects[1] = pContact->bodies[1]->mpOwner;

		pContact->bodies[0]->mpOwner->HandleEvent(&colllisionEvent);
		pContact->bodies[1]->mpOwner->HandleEvent(&colllisionEvent);
	}

}

