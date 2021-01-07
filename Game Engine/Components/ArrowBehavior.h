#pragma once
#include "..\EventManager.h"
#include "Component.h"
#include "CharacterAnimations.h"
#include "Transform.h"
#include <vector>

class GameObject;

class ArrowBehavior : public Component
{
public:
	ArrowBehavior();
	~ArrowBehavior();

	void Initialize(float newXSpeed, float newYSpeed);

	void Update();

	virtual void Serialize(std::wfstream& InputStream);

	void HandleEvent(Event* pEvent);

public:
	float xSpeed;
	float ySpeed;
	Transform* arrowPosition;

private:
private:
};