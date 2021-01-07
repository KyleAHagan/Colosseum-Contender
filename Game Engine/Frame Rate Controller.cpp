/* Start Header --------------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Frame Rate Controller.cpp
Purpose: Controlling the framerate makes movement and simulation more predictable
Language: C++
Platform: Developed in Microsoft Visual Studio Community 2019 on an i7 and GTX 2060 in Windows 10
Project: CS529_kyle.hagan_final
Creation date: October 11, 2020
Credits: This file was created from a template provided by Antoine Abi Chacra.
End Header ----------------------------------------------------------------*/
#include "Frame Rate Controller.h"
#include "limits.h"
#include "SDL_timer.h"

Frame_Rate_Controller::Frame_Rate_Controller(unsigned int maxFrameRate)
{
	if (0 == maxFrameRate)
	{
		mMaxFrameRate = UINT_MAX;
	}
	else
	{
		mMaxFrameRate = maxFrameRate;
	}
	mTicksPerFrame = 1000 / mMaxFrameRate;
	mTickStart = 0;
	mTickEnd = 0;
	mFrameTime = 0;
}

void Frame_Rate_Controller::frameStart()
{
	mTickStart = SDL_GetTicks();
}
void Frame_Rate_Controller::frameEnd()
{
	mTickEnd = SDL_GetTicks();
	while (mTickEnd - mTickStart < mTicksPerFrame)
		mTickEnd = SDL_GetTicks();

	mFrameTime = mTickEnd - mTickStart;
}