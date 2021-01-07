#pragma once

#include "..\EventManager.h"
#include "Component.h"
#include "CharacterAnimations.h"
#include <vector>

class GameObject;


class GoblinMeleeBehavior : public Component
{
public:
	GoblinMeleeBehavior();
	~GoblinMeleeBehavior();



	void Update();

	virtual void Serialize(std::wfstream& InputStream);

	void HandleEvent(Event* pEvent);
	float reactionDelay = 0;

public:
private:
private:
};