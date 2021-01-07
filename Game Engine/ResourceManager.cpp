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


#include <SDL.h>
#include <string>
#include "stdio.h"
#include "Windows.h"
#include "glew.h"
#include <iostream>
#include <fstream>
#include <streambuf>

#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <wchar.h>
#include "Components/Sprite.h"

#include "ResourceManager.h"
#include "SDL_surface.h"

extern wchar_t* resourcesPath;
extern std::vector<std::vector<Sprite*>> spritesUsingEachSpritesheet;


std::unordered_map<std::wstring, int> spritesheetNamesAndIDs;
std::unordered_map<std::string, SDL_Surface*> spriteMaps;

ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{
	spritesheetNamesAndIDs.clear();
}

int ResourceManager::ParseSpritesheet(const wchar_t* fileName)
{
	wchar_t relativepath[1024] = L"";
	wcscat_s(relativepath, 1024, resourcesPath);
	wcscat_s(relativepath, 1024, fileName);
	wchar_t full_path[_MAX_PATH];
	wchar_t node[256];
	wchar_t dir[_MAX_DIR];
	wchar_t file_name[_MAX_FNAME];
	wchar_t ext[_MAX_EXT];
	_wsplitpath_s(relativepath, node, dir, file_name, ext);

	std::wstring fileNameAsWstring(file_name);

	GLuint textureID;
	std::unordered_map<std::wstring, int>::const_iterator iterator = spritesheetNamesAndIDs.find(fileNameAsWstring);//This assumes that the same filename in different directories will refer to the same content.
	if (iterator != spritesheetNamesAndIDs.end())
	{
		return iterator->second;
	}


	Gdiplus::Bitmap bmp(relativepath);
	Gdiplus::Rect rect(0, 0, bmp.GetWidth(), bmp.GetHeight());
	Gdiplus::BitmapData data;
	bmp.LockBits(&rect, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &data);
	glGenTextures(1, &textureID);


	//bind created texture
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);



	//Load the data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, data.Width, data.Height, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, data.Scan0);

	//Unlock the bits
	bmp.UnlockBits(&data);

	//Unbind the texture
	glBindTexture(GL_TEXTURE_2D, 0);

	spritesheetNamesAndIDs[fileNameAsWstring] = textureID;
	return textureID;
}