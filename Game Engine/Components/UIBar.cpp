#include "UIBar.h"
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

UIBar::UIBar() : Component(L"UIBar")
{
	for (auto gameObject : gpGameObjectManager->mGameObjects)
	{
		if (gameObject->objectType == L"UIBar")
		{
			statToTrack++;
		}
	}
}

UIBar::~UIBar()
{

}

void UIBar::Serialize(std::wfstream& InputStream)
{

}

void UIBar::Update()
{
	if (nullptr == player || nullptr == playerStats)
	{
		for (auto gameObject : gpGameObjectManager->mGameObjects)
		{
			if (gameObject->objectType == L"PLAYER")
			{
				player = gameObject;
				playerStats = static_cast<Stats*>(gameObject->GetComponent(L"Stats"));
				break;
			}
		}
	}
	Transform* barTransform = static_cast<Transform*>(mpOwner->GetComponent(L"Transform"));
	if (nullptr != barTransform && nullptr != player && nullptr != playerStats)
	{
		if (1 == statToTrack)
		{
			barTransform->scaleX = playerStats->health / playerStats->maxHealth;
		}
		else if (2 == statToTrack)
		{
			barTransform->scaleX = playerStats->mana / playerStats->maxMana;
		}
	}
}

void UIBar::HandleEvent(Event* pEvent)
{


}