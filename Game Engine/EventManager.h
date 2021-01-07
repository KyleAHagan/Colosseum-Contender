#pragma once

#include <list>
#include <unordered_map>
#include "GameObject.h"

enum EventType
{
	COLLIDE,
	PLAYER_HIT,
	MOUSE_CLICK,
	ANIMATION_FRAME_REACHED,
	ATTACK_ZONE_HIT,

	NUM
};

class Event
{
public:
	Event(EventType Type)
	{
		mType = Type;
		mTimer = 0.0f;
	}
	virtual ~Event() {}
public:
	EventType mType;
	float mTimer;
private:

private:

};



class EventManager
{
public:
	EventManager();
	~EventManager();

	void BroadcastEventToEveryone(Event* pEvent);
	void BroadcastEventToSubscribers(Event* pEvent);
	void AddTimedEvent(Event* pEvent); //NOTE: The event itself contains the timer.
	void Update(float frameTime);

	void Subscribe(EventType eventType, GameObject* pGameObject);

public:
	std::list<Event*> mEvents;

	std::unordered_map<EventType, std::list<GameObject*>> mSubscriptions;

private:

private:

};