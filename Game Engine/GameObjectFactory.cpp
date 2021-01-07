#include "GameObjectFactory.h"
#include "GameObjectManager.h"
#include "GameObject.h"

#include "ResourceManager.h"

#include <string>
#include "Components\\Component.h"
#include "Components\\LeftRight.h"
#include "Components\\Sprite.h"
#include "Components\\Transform.h"
#include "Components\\UpDown.h"
#include "Components\\CharacterAnimations.h"
#include "Components\\Body.h"


#include "../Third Party/rapidjson/document.h"
#include "../Third Party/rapidjson/writer.h"
#include "../Third Party/rapidjson/stringbuffer.h"
#include <iostream>
#include <fstream>
#include <streambuf>
#include <vector>
#include "stdio.h"
#include "Windows.h"


extern GameObjectManager* gpGameObjectManager;
extern ResourceManager* gpResourceManager;
extern std::vector<std::vector<Sprite*>> spritesUsingEachSpritesheet;
extern wchar_t* resourcesPath;

GameObject* GameObjectFactory::LoadGameObject(std::wstring pFileName)//NOTES: factory class. CS529 Oct 20 2020 00:36:56
{
	GameObject* pNewGameObject = nullptr;
	std::wstring componentName;
	std::wfstream inputStream(pFileName);

	if (inputStream.is_open())
	{
		pNewGameObject = new GameObject();
		inputStream >> pNewGameObject->objectType;
        inputStream >> pNewGameObject->objectGroup;
		while (getline(inputStream, componentName))
		{
            if (componentName == L"")
                continue;
			Component* pNewComponent = nullptr;

            pNewComponent = pNewGameObject->AddComponent(componentName);

			if (nullptr != pNewComponent)
				pNewComponent->Serialize(inputStream);
		}
		gpGameObjectManager->mGameObjects.push_back(pNewGameObject);
		inputStream.close();
	}
	return pNewGameObject;
}

void GameObjectFactory::LoadLevel(std::wstring pFileName)
{
	std::wfstream inputStream(pFileName);
	std::wstring archetypePath;
	if (inputStream.is_open())
	{
		while (getline(inputStream, archetypePath))
		{
			if (archetypePath == L"")
				continue;
			archetypePath = L"..\\Resources\\" + archetypePath;
			GameObject* pNewGameObject = LoadGameObject(archetypePath);

			Transform* pT = static_cast<Transform*>(pNewGameObject->GetComponent(L"Transform"));
			pT->Serialize(inputStream);

		}
	}
	inputStream.close();
}

void GameObjectFactory::LoadFloorAndWallsFromTiled(const wchar_t* fileName)
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

    int spriteSheetTextureID = gpResourceManager->ParseSpritesheet(L"Environment_Dungeon.png"); //TODO make this more flexible. read spritesheet name(s) from json.

    std::ifstream levelIn(relativepath);
    std::string contents((std::istreambuf_iterator<char>(levelIn)), std::istreambuf_iterator<char>());
    rapidjson::Document levelJsonDOM;
    levelJsonDOM.Parse(contents.c_str());



    int numLayers = 0, width = 0, height = 0;
    rapidjson::Value::ConstMemberIterator iterator = levelJsonDOM.FindMember("width");//The widths will be the same in each layer, so any one will do.
    if (iterator != levelJsonDOM.MemberEnd() && iterator->value.IsInt())
    {
        width = iterator->value.GetInt();
    }
    else
    {
        //TODO: throw an error.
    }
    iterator = levelJsonDOM.FindMember("height");
    if (iterator != levelJsonDOM.MemberEnd() && iterator->value.IsInt())
    {
        height = iterator->value.GetInt();
    }
    else
    {
        //TODO: throw an error.
    }
    const rapidjson::Value& layers = levelJsonDOM["layers"];
    //There is a "nextlayerid" in level json, but the editor doesn't make sure the layers are numbered 0 to max after one is deleted,
    // so nextlayerid might not reflect the number of layers. Better to count. There might be a rapidjson function to get the number of elements in an object.
    for (rapidjson::Value::ConstValueIterator layerIterator = layers.Begin(); layerIterator != layers.End(); ++layerIterator, ++numLayers) {}

    std::vector<int> level3D;
    int currentLayer = 0, currentRow = 0, currentColumn = 0;
    for (rapidjson::Value::ConstValueIterator layerIterator = layers.Begin(); layerIterator != layers.End(); ++layerIterator, ++currentLayer)
    {
        const rapidjson::Value& layer = *layerIterator;

        for (rapidjson::Value::ConstValueIterator arrayIterator = layer["data"].Begin(); arrayIterator != layer["data"].End(); ++arrayIterator, ++currentColumn)
        {
            if (currentColumn == width)
            {
                currentColumn = 0;
                currentRow++;
            }
            level3D.push_back(arrayIterator->GetInt());
            if (level3D.back() != 0)
            {
                GameObject* pNewGameObject = new GameObject();

                if (level3D.back() == 335)//numbers for chests, switches, and stairs. Eventually powerups.
                {
                    pNewGameObject->objectType = L"Chest";
                    pNewGameObject->objectGroup = L"PlayerOnly";
                }
                else if (level3D.back() == 381)
                {
                    pNewGameObject->objectType = L"Switch";
                    pNewGameObject->objectGroup = L"PlayerOnly";
                }
                else if (level3D.back() == 382)
                {
                    pNewGameObject->objectType = L"Switch";
                    pNewGameObject->objectGroup = L"PlayerOnly";
                }
                else if (level3D.back() == 383)
                {
                    pNewGameObject->objectType = L"Switch";
                    pNewGameObject->objectGroup = L"PlayerOnly";
                }
                else
                {
                    pNewGameObject->objectType = L"Tile";
                    pNewGameObject->objectGroup = L"Scenery";
                }
                Component* pNewComponent = nullptr;
                pNewComponent = pNewGameObject->AddComponent(L"Sprite");

                Sprite* tileSprite = static_cast<Sprite*>(pNewGameObject->GetComponent(L"Sprite"));

                if ((spriteSheetTextureID + 1) > spritesUsingEachSpritesheet.size())
                {
                    spritesUsingEachSpritesheet.resize(spriteSheetTextureID + 1);
                }
                spritesUsingEachSpritesheet[spriteSheetTextureID].push_back(tileSprite);


                tileSprite->spriteSheetTextureID = spriteSheetTextureID;
                int textureMapIndex = level3D.back();
                int textureMapRow = (textureMapIndex) / 20; 
                int textureMapColumn = (textureMapIndex) % 20;

                tileSprite->textureRow = textureMapRow + 1; //moving between 0 and 1 indexed.
                tileSprite->textureColumn = textureMapColumn;
                tileSprite->VAOID = 1;


                pNewComponent = pNewGameObject->AddComponent(L"Transform");
                Transform* tileTransform = static_cast<Transform*>(pNewGameObject->GetComponent(L"Transform"));
                tileTransform->scaleX = 66.66;
                tileTransform->scaleY = 66.66;
                tileTransform->scaleZ = 1;

                tileTransform->positionX = (currentColumn - 1) * (tileTransform->scaleX);
                tileTransform->positionY = (height - currentRow - 2) * (tileTransform->scaleY);
                if (pNewGameObject->objectType == L"Chest" || pNewGameObject->objectType == L"Switch")
                {
                    tileTransform->positionZ = -20;
                }
                else
                {
                    tileTransform->positionZ = -100;
                }



                if (pNewGameObject->objectType == L"Chest" || pNewGameObject->objectType == L"Switch")
                {
                    pNewComponent = pNewGameObject->AddComponent(L"Body");
                    Body* tileBody = static_cast<Body*>(pNewGameObject->GetComponent(L"Body"));
                    tileBody->Initialize(66.66, 66.66, 0, L"AABB", tileSprite->VAOID);
                }

                gpGameObjectManager->mGameObjects.push_back(pNewGameObject);
            }

        }

        currentColumn = 0;
        currentRow = 0;
    }
}
