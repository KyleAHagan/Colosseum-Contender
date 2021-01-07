#include "LeftRight.h"
#include "Transform.h"
#include "..\GameObject.h"
#include "..\Frame Rate Controller.h"

extern Frame_Rate_Controller* gFrameRateController; //DEBUG: This is extern in example CS529 October 29 2020 00:26:21

LeftRight::LeftRight() : Component(L"LeftRight")
{
	mGoingLeft = true;
	mTimerMax = 0;
	mTimer = 0;
	mSpeed = 2.0f;
}

LeftRight::~LeftRight()
{

}


void LeftRight::Update(void)
{
	mTimer -= gFrameRateController->GetFrameTime();

	if (mTimer < 0)
	{
		mGoingLeft = !mGoingLeft;
		mTimer = mTimerMax;
	}
	Transform* pTransform = static_cast<Transform*>(mpOwner->GetComponent(L"Transform"));
	if (nullptr != pTransform)
	{
		if (mGoingLeft)
			pTransform->positionX -= mSpeed;
		else
			pTransform->positionX += mSpeed;
	}
}

void LeftRight::Serialize(std::wfstream& InputStream) //TODO ideally, the serializer would be encapsulated. CS229 October 20 2020 00:27:00
{
	int goingLeft; //serializing directly to a boolean might not work, so temp int here.

	InputStream >> goingLeft;
	InputStream >> mTimerMax;

	mGoingLeft = (bool)goingLeft;
	mTimer = mTimerMax;

}