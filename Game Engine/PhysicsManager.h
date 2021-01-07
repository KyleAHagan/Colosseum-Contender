#pragma once

#include "EventManager.h"

class CollideEvent : public Event
{
public:
	CollideEvent();
	~CollideEvent();
public:
	GameObject* mGameObjects[2];
private:

private:

};


class PhysicsManager
{
public:
	PhysicsManager();
	~PhysicsManager();

	void Update(float frameTime);

public:

private:

private:
};