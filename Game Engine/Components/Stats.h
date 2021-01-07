#pragma once
#include <vector>
#include "Component.h"

class GameObject;

class Stats : public Component
{
public:
	Stats();
	~Stats();

	void Update(void);
	void Serialize(std::wfstream& InputStream);

public:
	float health;
	float maxHealth;
	float mana;
	float maxMana;

	float reactionDelay; //How long it takes an enemy to react when the player is attackable, for instance.

	int attackDamage;
	float attackPushbackForce;

	float movementSpeed;


private:

private:
};