#include "Sprite.h"
#include "..\\ResourceManager.h"
#include "SDL_surface.h"
#include <map>

extern std::string resourcesPathSprites;
extern ResourceManager* gpResourceManager;

extern std::vector<std::vector<Sprite*>> spritesUsingEachSpritesheet;

Sprite::Sprite() : Component(L"Sprite")
{
	spriteSheetTextureID;

	textureColumn = 0;
	textureRow = 0;
	flipSprite = false;
}
Sprite::~Sprite()
{

}
void Sprite::Update()
{

}

void Sprite::Update(int newTextureRow, int newTextureColumn, bool newFlipScale)
{
	textureRow = newTextureRow;
	textureColumn = newTextureColumn;
	flipSprite = newFlipScale;

}

void Sprite::Serialize(std::wfstream& InputStream)
{
	wchar_t fileName[1024];
	InputStream >> fileName;
	spriteSheetTextureID = gpResourceManager->ParseSpritesheet(fileName);

	if ((spriteSheetTextureID + 1) > spritesUsingEachSpritesheet.size())
	{
		spritesUsingEachSpritesheet.resize(spriteSheetTextureID + 2);
	}
	spritesUsingEachSpritesheet[spriteSheetTextureID].push_back(this);

	InputStream >> VAOID;
	InputStream >> textureRow;
	InputStream >> textureColumn;
	InputStream >> flipSprite;


}