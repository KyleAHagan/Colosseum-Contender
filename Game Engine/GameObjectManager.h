#pragma once

#include <vector>

class GameObject;

class GameObjectManager
{
public:
	GameObjectManager();
	~GameObjectManager();

public:
	std::vector<GameObject*> mGameObjects;
	std::vector<GameObject*> mGameObjectsToCreate;
	std::vector<GameObject*> mGameObjectsToDelete;

private:
private:
};