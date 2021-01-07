#pragma once
#include "..\EventManager.h"
#include "Component.h"
#include "CharacterAnimations.h"
#include <vector>

class GameObject;

class AttackZoneHitEvent : public Event
{
public:
	AttackZoneHitEvent();
	AttackZoneHitEvent(int newDamage, float newPushbackForce, int newDirection, GameObject* newAttacker);
	~AttackZoneHitEvent();
	int damage = 10;
	float pushbackForce = 10;
	int direction = 0;
	GameObject* attacker = nullptr;
};

class AttackZoneBehavior : public Component
{
public:
	AttackZoneBehavior();
	~AttackZoneBehavior();

	void Initialize(GameObject* newAttacker, int newDamage, float newPushbackForce, int newDirection, float newDuration, bool newDestroyThisObjectOnHit);

	void Update();

	virtual void Serialize(std::wfstream& InputStream);

	void HandleEvent(Event* pEvent);

public:
	float timeSinceCreation = 0;
	float duration = 0.16;
	bool destroyThisObjectOnHit = false; 

	GameObject* attacker;

	int damage;
	float pushbackForce;
	int direction; //{Up, Right, Down, Left} -> {0,1,2,3}

private:
private:
};