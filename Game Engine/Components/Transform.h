#pragma once

#include "Component.h"

class GameObject;

class Transform : public Component
{
public:
	Transform();
	~Transform();

	void Update();
	void Serialize(std::wfstream& InputStream);

public:

	float positionX, positionY, positionZ;
	float rotation;//degrees clockwise
	float scaleX;
	float scaleY;
	float scaleZ;

private:
private:
};