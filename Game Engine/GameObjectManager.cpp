#include "GameObjectManager.h"
#include "GameObject.h"

GameObjectManager::GameObjectManager()
{

}

GameObjectManager::~GameObjectManager()
{
	for (auto pGO : mGameObjects)
		delete pGO;
	mGameObjects.clear();
	for (auto pGO : mGameObjectsToCreate)
		delete pGO;
	mGameObjectsToCreate.clear();
	for (auto pGO : mGameObjectsToDelete)
		delete pGO;
	mGameObjectsToDelete.clear();
}
