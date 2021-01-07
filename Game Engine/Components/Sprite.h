#pragma once

#include "Component.h"

class GameObject;

class Sprite : public Component
{
public:
	Sprite();
	~Sprite();
	void Update();
	void Update(int newTextureRow, int newTextureColumn, bool newFlipScale);
	virtual void Serialize(std::wfstream& InputStream);
public:
	/*
	//NOTES: position coordinates: lower Left (-1, -1) to upper right (1, 1).
				(-1, 1)			(1,1)
				(-1, -1)		(1,-1)

	//		 texture coordinates: upper Left (0,0) to lower right (1,1).
				(0,0)			(1,0)
				(0,1)			(1,1)
	*/
	int spriteSheetTextureID;
	int VAOID;
	int textureRow;
	int textureColumn;
	bool flipSprite;

private:
private:
};