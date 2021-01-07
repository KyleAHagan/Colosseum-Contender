/* Start Header --------------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.
File Name: Main.cpp
Purpose: This contains the program entry point and game loop
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
#include <SDL.h>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <sstream>

#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>

#include "../Third Party/glm/glm/glm.hpp"
#include "../Third Party/glm/glm/ext.hpp"

#include "Input Manager.h"
#include "Frame Rate Controller.h"
#include "ResourceManager.h"
#include "GameObjectManager.h"
#include "GameObject.h"
#include "PhysicsManager.h"
#include "CollisionManager.h"
#include "EventManager.h"
#include "Components/Component.h"
#include "Components/LeftRight.h"
#include "Components/Sprite.h"
#include "Components/Transform.h"
#include "Components/Controller.h"
#include "Components/UpDown.h"
#include "Components/Body.h"
#include "Components/CharacterAnimations.h"

#include "GameObjectFactory.h"
#include "GraphicsManager.h"


GraphicsManager* graphicsManager;
GameObjectManager* gpGameObjectManager;
ResourceManager* gpResourceManager;
Input_Manager* gInputManager;
GameObjectFactory* gpGameObjectFactory;
Frame_Rate_Controller* gFrameRateController;
PhysicsManager* gPhysicsManager;
CollisionManager* gCollisionManager;
EventManager* gEventManager;

SDL_GLContext openGLContext;
SDL_Window* pWindow;

extern std::vector<std::vector<Sprite*>> spritesUsingEachSpritesheet;

extern Transform* playerTransform;
extern Body* playerBody;

float windowWidth = 1920;
float windowHeight = 1080 ;

wchar_t* resourcesPath = L"../Resources/";
const int maxFrameRate = 60; //0 for uncapped.

FILE _iob[] = { *stdin, *stdout, *stderr };

extern "C" FILE * __cdecl __iob_func(void)
{
	return _iob;
}

#pragma comment(lib, "legacy_stdio_definitions.lib")

int main(int argc, char* args[])
{
	if (AllocConsole())
	{
		FILE* file;
		
		freopen_s(&file, "CONOUT$", "wt", stdout);
		freopen_s(&file, "CONOUT$", "wt", stderr);
		freopen_s(&file, "CONOUT$", "wt", stdin);
		SetConsoleTitle(L"Game Console");
	}

	bool appIsRunning = true;


	int error = 0;
	// Initialize SDL
	if ((error = SDL_Init(SDL_INIT_VIDEO)) < 0)
	{
		printf("Couldn't initialize SDL, error %i\n", error);
	}

	//OpenGL version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	pWindow = SDL_CreateWindow("Game window",		// window title
		0,											// initial window x position on screen
		30,											// initial window y position on screen
		windowWidth,								// width, in pixels
		windowHeight,								// height, in pixels
		SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

	// Check that the window was successfully made
	if (NULL == pWindow)
	{
		// In the event that the window could not be made...
		printf("Could not create window: %s\n", SDL_GetError());
	}

	//Create the opengl context and make it current
	openGLContext = SDL_GL_CreateContext(pWindow);

	if (glewInit() != GLEW_OK)
	{
		printf("Error: Couldn't initialize GLEW library.");
	}

	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	gInputManager = new Input_Manager();
	gFrameRateController = new Frame_Rate_Controller(maxFrameRate);
	gpResourceManager = new ResourceManager();
	gpGameObjectManager = new GameObjectManager();
	gpGameObjectFactory = new GameObjectFactory();
	graphicsManager = new GraphicsManager();
	gPhysicsManager = new PhysicsManager();
	gCollisionManager = new CollisionManager();
	gEventManager = new EventManager();

	int levelNumber = 0;
	int numberOfLevels = 10;

	std::wstring wJsonLocation = (L"Level" + std::to_wstring(levelNumber) + L".json");
	const wchar_t* jsonLocation = wJsonLocation.c_str();
	gpGameObjectFactory->LoadFloorAndWallsFromTiled(jsonLocation);
	int debugline = 1;
	std::wstring wLevelFileLocation = (L"..\\Resources\\Level" + std::to_wstring(levelNumber) + L".txt");
	const wchar_t* levelFileLocation = wLevelFileLocation.c_str();
	gpGameObjectFactory->LoadLevel(levelFileLocation);


	bool debugMode = false;
	float frameTimeInSeconds;

	// Game loop
	while(true == appIsRunning)
	{
		gFrameRateController->frameStart();
		SDL_Event e;

		frameTimeInSeconds = gFrameRateController->GetFrameTime()/1000.0f;

		while (SDL_PollEvent(&e) != 0)
		{

			switch (e.type)
			{
			case SDL_QUIT:
				appIsRunning = false;
				break;
			case SDL_MOUSEBUTTONDOWN:
				gInputManager->MousePress(e.button);
				break;
			}

		}
		
		if (gInputManager->IsKeyReleased(SDL_SCANCODE_ESCAPE))
		{
			appIsRunning = false;
		}

		if (gInputManager->IsKeyTriggered(SDL_SCANCODE_COMMA))
		{
			debugMode = !debugMode;
		}

		graphicsManager->DrawWorld(debugMode);

		gInputManager->Update();

		gEventManager->Update(frameTimeInSeconds);

		gPhysicsManager->Update(frameTimeInSeconds);

		//Update
		for (size_t index = 0; index < gpGameObjectManager->mGameObjects.size(); ++index)
		{
			if (nullptr == gpGameObjectManager->mGameObjects[index])
				continue;
			gpGameObjectManager->mGameObjects[index]->Update();
		}

		// create/delete objects here.
		while (gpGameObjectManager->mGameObjectsToCreate.size() > 0)
		{
			if (nullptr == gpGameObjectManager->mGameObjects[0])
				break;
			gpGameObjectManager->mGameObjects.push_back(gpGameObjectManager->mGameObjectsToCreate[0]);
			gpGameObjectManager->mGameObjectsToCreate.erase(gpGameObjectManager->mGameObjectsToCreate.begin());
		}
		while (gpGameObjectManager->mGameObjectsToDelete.size() > 0)
		{
			if (nullptr == gpGameObjectManager->mGameObjects[0])
				break;

			for (int i = gpGameObjectManager->mGameObjects.size() - 1; i >= 0; i--)
			{
				if (gpGameObjectManager->mGameObjects[i] == gpGameObjectManager->mGameObjectsToDelete[0])
				{
					GameObject* objectToDelete =  gpGameObjectManager->mGameObjects[i];
					gpGameObjectManager->mGameObjects.erase(gpGameObjectManager->mGameObjects.begin() + i);
					Sprite* spriteToDelete = static_cast<Sprite*>(objectToDelete->GetComponent(L"Sprite"));
					if (nullptr != spriteToDelete)
					{
						for (int j = spritesUsingEachSpritesheet.size() - 1; j >= 0; j--)
						{
							for (int k = spritesUsingEachSpritesheet[j].size() - 1; k >= 0; k--)
							{
								if (spritesUsingEachSpritesheet[j][k] == spriteToDelete)
								{
									spritesUsingEachSpritesheet[j].erase(spritesUsingEachSpritesheet[j].begin() + k);
								}
							}
						}
					}
					delete objectToDelete;
				}
			}
			gpGameObjectManager->mGameObjectsToDelete.erase(gpGameObjectManager->mGameObjectsToDelete.begin());
		}

		bool monsterAlive = false;
		for (int i = gpGameObjectManager->mGameObjects.size() - 1; i >= 0; i--)
		{
			if (gpGameObjectManager->mGameObjects[i]->objectType != L"PLAYER" && gpGameObjectManager->mGameObjects[i]->objectGroup == L"Character")
			{
				monsterAlive = true;
				break;
			}
		}
		if (!monsterAlive)
		{

			for (auto pGO : gpGameObjectManager->mGameObjects)
				delete pGO;
			gpGameObjectManager->mGameObjects.clear();
			for (auto pGO : gpGameObjectManager->mGameObjectsToCreate)
				delete pGO;
			gpGameObjectManager->mGameObjectsToCreate.clear();
			gpGameObjectManager->mGameObjectsToDelete.clear();//These have already been deleted when deleting the game object manager.

			std::list<Event*>::iterator iterator = gEventManager->mEvents.begin();
			while (iterator != gEventManager->mEvents.end())
			{
				Event* pEvent = *iterator;
				delete pEvent;
				iterator = gEventManager->mEvents.erase(iterator);
			}

			gEventManager->mSubscriptions.clear();


			if (levelNumber > numberOfLevels)
			{
				break;
			}

			levelNumber++;

			std::wstring wJsonLocation = (L"Level" + std::to_wstring(levelNumber) + L".json");
			const wchar_t* jsonLocation = wJsonLocation.c_str();
			gpGameObjectFactory->LoadFloorAndWallsFromTiled(jsonLocation);
			std::wstring wLevelFileLocation = (L"..\\Resources\\Level" + std::to_wstring(levelNumber) + L".txt");
			const wchar_t* levelFileLocation = wLevelFileLocation.c_str();
			gpGameObjectFactory->LoadLevel(levelFileLocation);

			playerTransform = nullptr;
			playerBody = nullptr;
		}




		bool playerAlive = false;
		for (int i = gpGameObjectManager->mGameObjects.size() - 1; i >= 0; i--)
		{
			if (gpGameObjectManager->mGameObjects[i]->objectType == L"PLAYER")
			{
				playerAlive = true;
				break;
			}
		}
		if (!playerAlive)
		{
			for (auto pGO : gpGameObjectManager->mGameObjects)
				delete pGO;
			gpGameObjectManager->mGameObjects.clear();
			for (auto pGO : gpGameObjectManager->mGameObjectsToCreate)
				delete pGO;
			gpGameObjectManager->mGameObjectsToCreate.clear();
			gpGameObjectManager->mGameObjectsToDelete.clear();//These have already been deleted when deleting the game object manager.

			std::list<Event*>::iterator iterator = gEventManager->mEvents.begin();
			while (iterator != gEventManager->mEvents.end())
			{
				Event* pEvent = *iterator;
				delete pEvent;
				iterator = gEventManager->mEvents.erase(iterator);
			}

			gEventManager->mSubscriptions.clear();

			
			//load next level.
			std::wstring wJsonLocation = (L"Level" + std::to_wstring(levelNumber) + L".json");
			const wchar_t* jsonLocation = wJsonLocation.c_str();
			gpGameObjectFactory->LoadFloorAndWallsFromTiled(jsonLocation);
			std::wstring wLevelFileLocation = (L"..\\Resources\\Level" + std::to_wstring(levelNumber) + L".txt");
			const wchar_t* levelFileLocation = wLevelFileLocation.c_str();
			gpGameObjectFactory->LoadLevel(levelFileLocation);

			playerTransform = nullptr;
		}
		
		gFrameRateController->frameEnd();
	}

	SDL_GL_DeleteContext(openGLContext);

	delete gEventManager;
	delete gCollisionManager;
	delete gPhysicsManager;
	delete graphicsManager;
	delete gpGameObjectFactory;
	delete gpGameObjectManager;
	delete gpResourceManager;
	delete gFrameRateController;
	delete gInputManager;

	return 0;
}