#include "ArrowBehavior.h"
#include "Controller.h"
#include "..\Input Manager.h"
#include "..\EventManager.h"
#include "..\PhysicsManager.h"
#include "..\GameObject.h"
#include "Transform.h"
#include "Body.h"
#include "Sprite.h"
#include "../GameObjectManager.h"
#include "..\\ResourceManager.h"
#include "../GraphicsManager.h"
#include "../Frame Rate Controller.h"



#include <iostream>
#include <fstream>
#include <streambuf>

extern Frame_Rate_Controller* gFrameRateController;
extern Input_Manager* gInputManager;
extern EventManager* gEventManager;
extern GameObjectManager* gpGameObjectManager;
extern ResourceManager* gpResourceManager;
extern ResourceManager* gpResourceManager;
extern std::vector<std::vector<Sprite*>> spritesUsingEachSpritesheet;
extern Frame_Rate_Controller* gFrameRateController;



ArrowBehavior::ArrowBehavior() : Component(L"ArrowBehavior")
{
	xSpeed = 1.0;
	ySpeed = 1.0;
}

void ArrowBehavior::Initialize(float newXSpeed, float newYSpeed)
{
	xSpeed = newXSpeed;
	ySpeed = newYSpeed;
}

ArrowBehavior::~ArrowBehavior()
{

}

void ArrowBehavior::Serialize(std::wfstream& InputStream)
{

}

void ArrowBehavior::Update()
{
	if (nullptr == arrowPosition)
	{
		arrowPosition = static_cast<Transform*>(mpOwner->GetComponent(L"Transform"));
	}
	if (nullptr != arrowPosition)
	{
		arrowPosition->positionX += xSpeed;
		arrowPosition->positionY += ySpeed;
		std::cout << "Arrow Position X: " << arrowPosition->positionX <<  " Arrow Position Y: " << arrowPosition->positionY << std::endl;
	}
}

void ArrowBehavior::HandleEvent(Event* pEvent)
{

}