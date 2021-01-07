/* Start Header --------------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: ResourceManager.cpp
Purpose: Managing game resources
Language: C++
Platform: Developed in Microsoft Visual Studio Community 2019 on an i7 and GTX 2060 in Windows 10
Project: CS529_kyle.hagan_final
Creation date: October 11, 2020
Credits: This file was created from a template provided by Antoine Abi Chacra.
End Header ----------------------------------------------------------------*/
#pragma once
#include <unordered_map>

struct SDL_Surface;

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	int ParseSpritesheet(const wchar_t* fileName);

public:



private:

private:


};