#pragma once

#include <string>

#include "../Third Party/rapidjson/document.h"
#include "../Third Party/rapidjson/writer.h"
#include "../Third Party/rapidjson/stringbuffer.h"
#include "../Third Party/rapidjson/prettywriter.h"
#include <cstdio>
#include <vector>

class GameObject;

class GameObjectFactory
{
public:
	GameObjectFactory() {}
	~GameObjectFactory() {}

	GameObject* LoadGameObject(std::wstring pFileName);
	void LoadLevel(std::wstring pFileName);
	void LoadFloorAndWallsFromTiled(const wchar_t* pFileName);
public:
private:
private:

};