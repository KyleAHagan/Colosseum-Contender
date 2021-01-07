 #pragma once

#include "Component.h"
#include "..\EventManager.h"

class GameObject;

class UpDown : public Component
{
public:
	UpDown();
	~UpDown();

	void Update(void);
	void Serialize(std::wfstream& InputStream);

	void HandleEvent(Event* pEvent);

public:
	bool mGoingUp;
	int mTimer;
	int mTimerMax;
	float mSpeed;
private:
private:
};