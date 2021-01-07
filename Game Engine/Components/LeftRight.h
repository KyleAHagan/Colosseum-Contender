#pragma once

#include "Component.h"

class GameObject;

class LeftRight : public Component
{
public:
	LeftRight();
	~LeftRight();

	void Update(void);
	void Serialize(std::wfstream& InputStream);

public:
	bool mGoingLeft;
	int mTimer;
	int mTimerMax;
	float mSpeed;
private:

private:
};