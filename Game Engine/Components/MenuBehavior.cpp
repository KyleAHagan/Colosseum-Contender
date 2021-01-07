#include "MenuBehavior.h"
#include "Transform.h"
#include "..\GameObject.h"
#include "..\GameObjectManager.h"
#include "..\Frame Rate Controller.h"
#include "..\Input Manager.h"
#include <stdio.h>
#include <iostream>


extern Frame_Rate_Controller* gFrameRateController;
extern EventManager* gEventManager;
extern GameObjectManager* gpGameObjectManager;

MenuBehavior::MenuBehavior() : Component(L"MenuBehavior")
{

}

MenuBehavior::~MenuBehavior()
{

}


void MenuBehavior::Update(void)
{

}

void MenuBehavior::Serialize(std::wfstream& InputStream)
{
	gEventManager->Subscribe(EventType::MOUSE_CLICK, this->mpOwner);

}

void MenuBehavior::HandleEvent(Event* pEvent)
{
	if (EventType::MOUSE_CLICK == pEvent->mType)
	{
		MouseClickEvent* pMouseClickEvent = static_cast<MouseClickEvent*>(pEvent);
		std::cout << "Mouse click at X: " << pMouseClickEvent->xCoordinate << " and Y: " << pMouseClickEvent->yCoordinate << std::endl;
 		if (pMouseClickEvent->xCoordinate > 60 && pMouseClickEvent->xCoordinate < 530 && pMouseClickEvent->yCoordinate > 350 && pMouseClickEvent->yCoordinate < 475)
		{
			for (int i = gpGameObjectManager->mGameObjects.size() - 1; i >= 0; i--)
			{
				gpGameObjectManager->mGameObjectsToDelete.push_back(gpGameObjectManager->mGameObjects[i]);
			}

		}

		if (pMouseClickEvent->xCoordinate > 60 && pMouseClickEvent->xCoordinate < 530 && pMouseClickEvent->yCoordinate > 925 && pMouseClickEvent->yCoordinate < 1050)
		{
			for (int i = gpGameObjectManager->mGameObjects.size() - 1; i >= 0; i--)
			{
				gpGameObjectManager->mGameObjectsToDelete.push_back(gpGameObjectManager->mGameObjects[i]);
			}

		}


	}
}