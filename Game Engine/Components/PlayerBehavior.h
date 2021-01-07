#pragma once
#include "..\EventManager.h"
#include "Component.h"
#include "CharacterAnimations.h"
#include <vector>

class GameObject;

class PlayerHitEvent : public Event
{
public:
	PlayerHitEvent();
	~PlayerHitEvent();
};

class PlayerBehavior : public Component
{
public:
	PlayerBehavior();
	~PlayerBehavior();



	void Update();

	virtual void Serialize(std::wfstream& InputStream);

	void HandleEvent(Event* pEvent);

public:
private:
private:
};