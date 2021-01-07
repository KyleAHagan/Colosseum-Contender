#pragma once

#include "..\EventManager.h"
#include "Component.h"
#include "CharacterAnimations.h"
#include <vector>

class GameObject;


class GoblinRangedBehavior : public Component
{
public:
	GoblinRangedBehavior();
	~GoblinRangedBehavior();



	void Update();

	virtual void Serialize(std::wfstream& InputStream);

	void HandleEvent(Event* pEvent);
	float reactionDelay = 0;
	double projectileSpeed = 7;
	double projectileAccuracy = (RAND_MAX - 700); //Arrow target has a random numbers added to it that decrease with accuracy. accuracy >= RAND_MAX for perfect accuracy.

public:
private:
private:
};