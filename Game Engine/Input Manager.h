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
#pragma once
#include "SDL_stdinc.h"
#include <SDL.h>
#include "EventManager.h"


class MouseClickEvent : public Event
{
public:
	MouseClickEvent();
	MouseClickEvent(float xCoordinate, float yCoordinate, int newButton);
	~MouseClickEvent();
	float xCoordinate = 0;
	float yCoordinate = 0;
	int button = 1;
};


class Input_Manager
{
public:
	Input_Manager();
	~Input_Manager();

	void Update();

	bool IsKeyPressed(unsigned int KeyScanCode);
	bool IsKeyTriggered(unsigned int KeyScanCode);
	bool IsKeyReleased(unsigned int KeyScanCode);

	void MousePress(SDL_MouseButtonEvent& mouseButtonEvent);

public:


private:
private:
	Uint8 mCurrentState[512];
	Uint8 mPreviousState[512];

};