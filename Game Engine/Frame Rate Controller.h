/* Start Header --------------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Frame Rate Controller.h
Purpose: Controlling the framerate makes movement and simulation more predictable
Language: C++
Platform: Developed in Microsoft Visual Studio Community 2019 on an i7 and GTX 2060 in Windows 10
Project: CS529_kyle.hagan_final
Creation date: October 11, 2020
Credits: This file was created from a template provided by Antoine Abi Chacra.
End Header ----------------------------------------------------------------*/

#pragma once

class Frame_Rate_Controller
{
public:
	Frame_Rate_Controller(unsigned int MaxFrameRate);
	~Frame_Rate_Controller() {}

	void frameStart();
	void frameEnd();
	unsigned int GetFrameTime() { return mFrameTime; }
public:

private:
	unsigned int mTickStart;
	unsigned int mTickEnd;
	unsigned int mMaxFrameRate;
	unsigned int mTicksPerFrame;
	unsigned int mFrameTime;
private:

};