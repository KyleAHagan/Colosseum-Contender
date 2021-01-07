#include "UpDown.h"
#include "Transform.h"
#include "..\GameObject.h"
#include "..\EventManager.h"
#include "..\Frame Rate Controller.h"
#include <string>

extern Frame_Rate_Controller* gFrameRateController;
extern EventManager* gEventManager;

UpDown::UpDown() : Component(L"UpDown")
{
	mGoingUp = true;
	mTimer = 0;
	mTimerMax = 0;
	mSpeed = 0.001f;
}

UpDown::~UpDown()
{

}

void UpDown::Update(void) 
{
	mTimer -= gFrameRateController->GetFrameTime();

	if (mTimer < 0)
	{
		mGoingUp = !mGoingUp;
		mTimer = mTimerMax;
	}
	Transform* pTransform = static_cast<Transform*>(mpOwner->GetComponent(L"Transform"));
	if (nullptr != pTransform)
	{ 
		if (mGoingUp)
			pTransform->positionY -= mSpeed;
		else
			pTransform->positionY += mSpeed;
	}
}

void UpDown::Serialize(std::wfstream& InputStream)
{
	int goingUp, subscribeToPlayerCollision;

	InputStream >> goingUp;
	InputStream >> mTimerMax;
	InputStream >> subscribeToPlayerCollision;

	mGoingUp = (bool)goingUp;
	mTimer = mTimerMax;

	if (1 == subscribeToPlayerCollision)
	{
		gEventManager->Subscribe(EventType::PLAYER_HIT, this->mpOwner);
	}
}


void UpDown::HandleEvent(Event* pEvent)
{

}