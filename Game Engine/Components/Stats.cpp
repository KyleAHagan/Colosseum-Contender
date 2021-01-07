#include "Stats.h"
#include "Transform.h"
#include "..\GameObject.h"
#include "..\Frame Rate Controller.h"

extern Frame_Rate_Controller* gFrameRateController;

Stats::Stats() : Component(L"Stats")
{
	health = 100;
	maxHealth = 100;
	mana = 100;
	maxMana = 100;

	reactionDelay = 1.0; //How long it takes an enemy to react when the player is attackable, for instance.

	attackDamage = 10;
	attackPushbackForce = 1;
	movementSpeed = 1;

}

Stats::~Stats()
{

}


void Stats::Update(void)
{
	if (health > 0 && (health + .01) <= maxHealth)
		health += .01;
	if (mana > 0 && (mana + .01) <= maxMana)
		mana += .01;
}

void Stats::Serialize(std::wfstream& InputStream)
{
	InputStream >> health;
	InputStream >> maxHealth;
	InputStream >> mana;
	InputStream >> maxMana;

	InputStream >> reactionDelay; //How long it takes an enemy to react when the player is attackable, for instance.

	InputStream >> attackDamage;
	InputStream >> attackPushbackForce;
	InputStream >> movementSpeed;

}