#include "EventManager.h"
#include "GameObjectManager.h"
#include "GameObject.h"

extern GameObjectManager* gpGameObjectManager;

EventManager::EventManager()
{

}

EventManager::~EventManager()
{

}

void EventManager::BroadcastEventToEveryone(Event* pEvent)
{
	for (auto pGameObject : gpGameObjectManager->mGameObjects)
	{
		if (nullptr == pGameObject)
			continue;
		pGameObject->HandleEvent(pEvent);
	}
}

void EventManager::BroadcastEventToSubscribers(Event* pEvent)
{
	std::list<GameObject*>& listOfSubscribers = mSubscriptions[pEvent->mType];
	for (auto pGameObject_Subscriber : listOfSubscribers)
		pGameObject_Subscriber->HandleEvent(pEvent);
}


void EventManager::AddTimedEvent(Event* pEvent)
{
	mEvents.push_back(pEvent);
}

void EventManager::Update(float frameTime)
{
	std::list<Event*>::iterator iterator = mEvents.begin();

	while (iterator != mEvents.end())
	{
		Event* pEvent = *iterator;

		pEvent->mTimer -= frameTime;

		if (pEvent->mTimer <= 0)
		{
			BroadcastEventToSubscribers(pEvent);
			delete pEvent;
			iterator = mEvents.erase(iterator);
		}
		else
			++iterator;
	}

}

void EventManager::Subscribe(EventType eventType, GameObject* pGameObject)
{
	std::list<GameObject*> &listOfSubscribers = mSubscriptions[eventType];

	for (auto pGO_ExistingSubscriber : listOfSubscribers)
	{
		if (pGO_ExistingSubscriber == pGameObject)
			return;
	}

	listOfSubscribers.push_back(pGameObject);
}