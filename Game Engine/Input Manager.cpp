/* Start Header --------------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Input Manager.cpp
Purpose: This handles input in one easily updatable place
Language: C++
Platform: Developed in Microsoft Visual Studio Community 2019 on an i7 and GTX 2060 in Windows 10
Project: CS529_kyle.hagan_final
Creation date: October 11, 2020
Credits: This file was created from a template provided by Antoine Abi Chacra.
End Header ----------------------------------------------------------------*/

#include "Input Manager.h"
#include "memory.h"
#include "SDL_keyboard.h"
#include "SDL.h"
#include "EventManager.h"

extern EventManager* gEventManager;


MouseClickEvent::MouseClickEvent() : Event(EventType::MOUSE_CLICK)
{

}
MouseClickEvent::MouseClickEvent(float newXCoordinate, float newYCoordinate, int newButton) : Event(EventType::MOUSE_CLICK)
{
	xCoordinate = newXCoordinate;
	yCoordinate = newYCoordinate;
	button = newButton;

}
MouseClickEvent::~MouseClickEvent()
{

}




Input_Manager::Input_Manager()
{
	//Destination, value, # of bytes. Does not malloc, just sets.
	memset(mCurrentState, 0, 512 * sizeof(Uint8));
	memset(mPreviousState, 0, 512 * sizeof(Uint8));

}

Input_Manager::~Input_Manager()
{

}

void Input_Manager::Update()
{
	//Save the previous state
	memcpy(mPreviousState, mCurrentState, 512 * sizeof(Uint8));
	int numberOfFetched = 0;
	const Uint8* currentKeyboardStates = SDL_GetKeyboardState(&numberOfFetched);

	if (numberOfFetched > 512)
		numberOfFetched = 512;
	
	memcpy(mCurrentState, currentKeyboardStates, numberOfFetched * sizeof(Uint8));

}

bool Input_Manager::IsKeyPressed(unsigned int KeyScanCode)
{
	if (KeyScanCode >= 512)
		return false;
	return mCurrentState[KeyScanCode]; //Any int other than 0 is "true"
}

bool Input_Manager::IsKeyTriggered(unsigned int KeyScanCode)
{
	if (KeyScanCode >= 512)
		return false;
	return !mPreviousState[KeyScanCode] && mCurrentState[KeyScanCode];
}
bool Input_Manager::IsKeyReleased(unsigned int KeyScanCode)
{
	if (KeyScanCode >= 512)
		return false;
	return mPreviousState[KeyScanCode] && !mCurrentState[KeyScanCode];
}

void Input_Manager::MousePress(SDL_MouseButtonEvent& mouseButtonEvent)
{
	if (mouseButtonEvent.button == SDL_BUTTON_LEFT || mouseButtonEvent.button == SDL_BUTTON_RIGHT)
	{
		MouseClickEvent* pEvent = new MouseClickEvent(mouseButtonEvent.x, mouseButtonEvent.y, mouseButtonEvent.button);
		pEvent->mTimer -= 0.0f;
		gEventManager->BroadcastEventToSubscribers(pEvent);
		delete pEvent;
	}
}