#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include "glew.h"
#include <SDL.h>
#include <iostream>
#include <fstream>
#include <streambuf>

#include "../Third Party/glm/glm/glm.hpp"
#include "../Third Party/glm/glm/ext.hpp"

#include "GraphicsManager.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "CollisionManager.h"
#include "Components/Transform.h"
#include "Components/Sprite.h"
#include "Components/Body.h"
#include "Components/UIBar.h"

extern GameObjectManager* gpGameObjectManager;
extern ResourceManager* gpResourceManager;

extern float windowWidth;
extern float windowHeight;
extern SDL_Window* pWindow;
GLuint gLightingProgramID;
GLuint gUIProgramID;
GLuint gDebugProgramID;
GLuint bufferIDs[5];
GLuint vaoIDs[5];

glm::mat4 projection;

Transform* playerTransform = nullptr;
Body* playerBody =  nullptr;

std::vector<std::vector<Sprite*>> spritesUsingEachSpritesheet;
extern std::unordered_map<std::wstring, int> spritesheetNamesAndIDs;


//In my environment spritesheets, x is 1/20 and y is 1/24.
//In my character spritesheets, x is 1/4 and y is 1/11
float environmentSpritesheetPercentageX = 1.0f / 20.0f;
float environmentSpritesheetPercentageY = 1.0f / 24.0f;
float characterSpritesheetPercentageX = 1.0f / 4.0f;
float characterSpritesheetPercentageY = 1.0f / 11.0f;
float arrowSpritesheetPercentageX = 1.0f;
float arrowSpritesheetPercentageY = 1.0f;



int vertexNumRectangle = 6;
int trianglesInCircle = 32;
int vertexNumCircle = trianglesInCircle * 3;

GraphicsManager::GraphicsManager()
{
	LoadShaders();

	//Sets the default background color.
	glClearColor(0.458f, 0.368f, 0.328f, 1.0f);

	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);

	glGenBuffers(5, bufferIDs); //AABB environment texture percentages, AABB character texture percentages, AABB full texture (menus), circle.
	glGenVertexArrays(5, vaoIDs);



	int coordinatesPerPosition = 4;
	int coordinatesPerColor = 4;
	int coordinatesPerTexturePosition = 2;

	int positionStride = coordinatesPerPosition * sizeof(float); //number of bytes between consecutive vertices, which is the size of a single position (x + y + z) in  bytes.
	int colorStride = coordinatesPerColor * sizeof(float); //number of bytes between consecutive colors
	int textureCoordinateStride = coordinatesPerTexturePosition * sizeof(float); //number of bytes between consecutive texture coordinates

	GLfloat* positionsColorsAndTextures = new GLfloat[vertexNumRectangle * (coordinatesPerPosition + coordinatesPerColor + coordinatesPerTexturePosition)];

	/*
	//NOTES: position coordinates: lower Left (-1, -1) to upper right (1, 1).
					(-1, 1)			(1,1)
					(-1, -1)		(1,-1)

	//		 texture coordinates: upper Left (0,0) to lower right (1,1).
					(0,0)			(1,0)
					(0,1)			(1,1)
	*/
	glBindBuffer(GL_ARRAY_BUFFER, bufferIDs[0]);//Binding the next buffer
	glBindVertexArray(vaoIDs[0]);//Binding the next vao
	int VAOindex = -1;

	//Vertex 1 lower Left
	positionsColorsAndTextures[++VAOindex] = -0.5f;	positionsColorsAndTextures[++VAOindex] = -0.5f;	positionsColorsAndTextures[++VAOindex] = 0.0f; positionsColorsAndTextures[++VAOindex] = 1.0f;
	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 1.0f;	positionsColorsAndTextures[++VAOindex] = 1.0f;
	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = environmentSpritesheetPercentageY;

	//Vertex 2 lower Right
	positionsColorsAndTextures[++VAOindex] = 0.5f;	positionsColorsAndTextures[++VAOindex] = -0.5f;	positionsColorsAndTextures[++VAOindex] = 0.0f; positionsColorsAndTextures[++VAOindex] = 1.0f;
	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 1.0f;	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 1.0f;
	positionsColorsAndTextures[++VAOindex] = environmentSpritesheetPercentageX;		positionsColorsAndTextures[++VAOindex] = environmentSpritesheetPercentageY;

	//Vertex 3 upper Left
	positionsColorsAndTextures[++VAOindex] = -0.5f;	positionsColorsAndTextures[++VAOindex] = 0.5f;	positionsColorsAndTextures[++VAOindex] = 0.0f; positionsColorsAndTextures[++VAOindex] = 1.0f;
	positionsColorsAndTextures[++VAOindex] = 1.0f;	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 1.0f;
	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 0.0f;

	//Vertex 4 upper Left
	positionsColorsAndTextures[++VAOindex] = -0.5f;	positionsColorsAndTextures[++VAOindex] = 0.5f;	positionsColorsAndTextures[++VAOindex] = 0.0f; positionsColorsAndTextures[++VAOindex] = 1.0f;
	positionsColorsAndTextures[++VAOindex] = 1.0f;	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 1.0f;
	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 0.0f;

	//Vertex 5 lower Right
	positionsColorsAndTextures[++VAOindex] = 0.5f;	positionsColorsAndTextures[++VAOindex] = -0.5f;	positionsColorsAndTextures[++VAOindex] = 0.0f; positionsColorsAndTextures[++VAOindex] = 1.0f;
	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 1.0f;	positionsColorsAndTextures[++VAOindex] = 1.0f;
	positionsColorsAndTextures[++VAOindex] = environmentSpritesheetPercentageX;		positionsColorsAndTextures[++VAOindex] = environmentSpritesheetPercentageY;

	//Vertex 6 upper Right
	positionsColorsAndTextures[++VAOindex] = 0.5f;	positionsColorsAndTextures[++VAOindex] = 0.5f;	positionsColorsAndTextures[++VAOindex] = 0.0f; positionsColorsAndTextures[++VAOindex] = 1.0f;
	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 1.0f;	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 1.0f;
	positionsColorsAndTextures[++VAOindex] = environmentSpritesheetPercentageX;		positionsColorsAndTextures[++VAOindex] = 0.0f;

	//copy everything
	glBufferData(GL_ARRAY_BUFFER, vertexNumRectangle * (positionStride + colorStride + textureCoordinateStride), positionsColorsAndTextures, GL_STATIC_DRAW);//If things stop working after locking the computer or something, change this to something other than static draw.

	//Position information
	int positionHandle = 0;
	glEnableVertexAttribArray(positionHandle);
	glVertexAttribPointer(positionHandle, coordinatesPerPosition, GL_FLOAT, false, (positionStride + colorStride + textureCoordinateStride), 0);

	//Color information
	int colorHandle = 1;
	glEnableVertexAttribArray(colorHandle);
	glVertexAttribPointer(colorHandle, coordinatesPerColor, GL_FLOAT, false, (positionStride + colorStride + textureCoordinateStride), (void*)positionStride);

	//Texture coordinates
	int textureCoordinatesHandle = 2;
	glEnableVertexAttribArray(textureCoordinatesHandle);
	glVertexAttribPointer(textureCoordinatesHandle, coordinatesPerTexturePosition, GL_FLOAT, false, (positionStride + colorStride + textureCoordinateStride), (void*)(positionStride + colorStride));

	glFinish();//the c++ code will be blocked until openGL finishes whatever it is doing.

	//---
	/*
	//NOTES: position coordinates: lower Left (-1, -1) to upper right (1, 1).
					(-1, 1)			(1,1)
					(-1, -1)		(1,-1)

	//		 texture coordinates: upper Left (0,0) to lower right (1,1).
					(0,0)			(1,0)
					(0,1)			(1,1)
	*/

	delete[] positionsColorsAndTextures;
	positionsColorsAndTextures = new GLfloat[vertexNumRectangle * (coordinatesPerPosition + coordinatesPerColor + coordinatesPerTexturePosition)];
	glBindBuffer(GL_ARRAY_BUFFER, bufferIDs[1]);//Binding the next buffer
	glBindVertexArray(vaoIDs[1]);//Binding the next vao
	VAOindex = -1;

	//Vertex 1 lower Left
	positionsColorsAndTextures[++VAOindex] = -0.5f;	positionsColorsAndTextures[++VAOindex] = -0.5f;	positionsColorsAndTextures[++VAOindex] = 0.0f; positionsColorsAndTextures[++VAOindex] = 1.0f;
	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 1.0f;	positionsColorsAndTextures[++VAOindex] = 1.0f;
	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = characterSpritesheetPercentageY;

	//Vertex 2 lower Right
	positionsColorsAndTextures[++VAOindex] = 0.5f;	positionsColorsAndTextures[++VAOindex] = -0.5f;	positionsColorsAndTextures[++VAOindex] = 0.0f; positionsColorsAndTextures[++VAOindex] = 1.0f;
	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 1.0f;	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 1.0f;
	positionsColorsAndTextures[++VAOindex] = characterSpritesheetPercentageX;		positionsColorsAndTextures[++VAOindex] = characterSpritesheetPercentageY;

	//Vertex 3 upper Left
	positionsColorsAndTextures[++VAOindex] = -0.5f;	positionsColorsAndTextures[++VAOindex] = 0.5f;	positionsColorsAndTextures[++VAOindex] = 0.0f; positionsColorsAndTextures[++VAOindex] = 1.0f;
	positionsColorsAndTextures[++VAOindex] = 1.0f;	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 1.0f;
	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 0.0f;

	//Vertex 4 upper Left
	positionsColorsAndTextures[++VAOindex] = -0.5f;	positionsColorsAndTextures[++VAOindex] = 0.5f;	positionsColorsAndTextures[++VAOindex] = 0.0f; positionsColorsAndTextures[++VAOindex] = 1.0f;
	positionsColorsAndTextures[++VAOindex] = 1.0f;	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 1.0f;
	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 0.0f;

	//Vertex 5 lower Right
	positionsColorsAndTextures[++VAOindex] = 0.5f;	positionsColorsAndTextures[++VAOindex] = -0.5f;	positionsColorsAndTextures[++VAOindex] = 0.0f; positionsColorsAndTextures[++VAOindex] = 1.0f;
	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 1.0f;	positionsColorsAndTextures[++VAOindex] = 1.0f;
	positionsColorsAndTextures[++VAOindex] = characterSpritesheetPercentageX;		positionsColorsAndTextures[++VAOindex] = characterSpritesheetPercentageY;

	//Vertex 6 upper Right
	positionsColorsAndTextures[++VAOindex] = 0.5f;	positionsColorsAndTextures[++VAOindex] = 0.5f;	positionsColorsAndTextures[++VAOindex] = 0.0f; positionsColorsAndTextures[++VAOindex] = 1.0f;
	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 1.0f;	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 1.0f;
	positionsColorsAndTextures[++VAOindex] = characterSpritesheetPercentageX;		positionsColorsAndTextures[++VAOindex] = 0.0f;

	//copy everything
	glBufferData(GL_ARRAY_BUFFER, vertexNumRectangle * (positionStride + colorStride + textureCoordinateStride), positionsColorsAndTextures, GL_STATIC_DRAW);//If things stop working after locking the computer or something, change this to something other than static draw.

	//Position information
	positionHandle = 0;
	glEnableVertexAttribArray(positionHandle);
	glVertexAttribPointer(positionHandle, coordinatesPerPosition, GL_FLOAT, false, (positionStride + colorStride + textureCoordinateStride), 0);

	//Color information
	colorHandle = 1;
	glEnableVertexAttribArray(colorHandle);
	glVertexAttribPointer(colorHandle, coordinatesPerColor, GL_FLOAT, false, (positionStride + colorStride + textureCoordinateStride), (void*)positionStride);

	//Texture coordinates
	textureCoordinatesHandle = 2;
	glEnableVertexAttribArray(textureCoordinatesHandle);
	glVertexAttribPointer(textureCoordinatesHandle, coordinatesPerTexturePosition, GL_FLOAT, false, (positionStride + colorStride + textureCoordinateStride), (void*)(positionStride + colorStride));

	glFinish();//the c++ code will be blocked until openGL finishes whatever it is doing.

	delete[] positionsColorsAndTextures;
	positionsColorsAndTextures = new GLfloat[vertexNumRectangle * (coordinatesPerPosition + coordinatesPerColor + coordinatesPerTexturePosition)];
	glBindBuffer(GL_ARRAY_BUFFER, bufferIDs[2]);//Binding the next buffer
	glBindVertexArray(vaoIDs[2]);//Binding the next vao
	VAOindex = -1;

	//Vertex 1 lower Left
	positionsColorsAndTextures[++VAOindex] = -0.5f;	positionsColorsAndTextures[++VAOindex] = -0.5f;	positionsColorsAndTextures[++VAOindex] = 0.0f; positionsColorsAndTextures[++VAOindex] = 1.0f;
	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 1.0f;	positionsColorsAndTextures[++VAOindex] = 1.0f;
	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 1;

	//Vertex 2 lower Right
	positionsColorsAndTextures[++VAOindex] = 0.5f;	positionsColorsAndTextures[++VAOindex] = -0.5f;	positionsColorsAndTextures[++VAOindex] = 0.0f; positionsColorsAndTextures[++VAOindex] = 1.0f;
	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 1.0f;	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 1.0f;
	positionsColorsAndTextures[++VAOindex] = 1;		positionsColorsAndTextures[++VAOindex] = 1;

	//Vertex 3 upper Left
	positionsColorsAndTextures[++VAOindex] = -0.5f;	positionsColorsAndTextures[++VAOindex] = 0.5f;	positionsColorsAndTextures[++VAOindex] = 0.0f; positionsColorsAndTextures[++VAOindex] = 1.0f;
	positionsColorsAndTextures[++VAOindex] = 1.0f;	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 1.0f;
	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 0.0f;

	//Vertex 4 upper Left
	positionsColorsAndTextures[++VAOindex] = -0.5f;	positionsColorsAndTextures[++VAOindex] = 0.5f;	positionsColorsAndTextures[++VAOindex] = 0.0f; positionsColorsAndTextures[++VAOindex] = 1.0f;
	positionsColorsAndTextures[++VAOindex] = 1.0f;	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 1.0f;
	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 0.0f;

	//Vertex 5 lower Right
	positionsColorsAndTextures[++VAOindex] = 0.5f;	positionsColorsAndTextures[++VAOindex] = -0.5f;	positionsColorsAndTextures[++VAOindex] = 0.0f; positionsColorsAndTextures[++VAOindex] = 1.0f;
	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 1.0f;	positionsColorsAndTextures[++VAOindex] = 1.0f;
	positionsColorsAndTextures[++VAOindex] = 1;		positionsColorsAndTextures[++VAOindex] = 1;

	//Vertex 6 upper Right
	positionsColorsAndTextures[++VAOindex] = 0.5f;	positionsColorsAndTextures[++VAOindex] = 0.5f;	positionsColorsAndTextures[++VAOindex] = 0.0f; positionsColorsAndTextures[++VAOindex] = 1.0f;
	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 1.0f;	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 1.0f;
	positionsColorsAndTextures[++VAOindex] = 1;		positionsColorsAndTextures[++VAOindex] = 0.0f;

	//copy everything
	glBufferData(GL_ARRAY_BUFFER, vertexNumRectangle* (positionStride + colorStride + textureCoordinateStride), positionsColorsAndTextures, GL_STATIC_DRAW);//If things stop working after locking the computer or something, change this to something other than static draw.

	//Position information
	positionHandle = 0;
	glEnableVertexAttribArray(positionHandle);
	glVertexAttribPointer(positionHandle, coordinatesPerPosition, GL_FLOAT, false, (positionStride + colorStride + textureCoordinateStride), 0);

	//Color information
	colorHandle = 1;
	glEnableVertexAttribArray(colorHandle);
	glVertexAttribPointer(colorHandle, coordinatesPerColor, GL_FLOAT, false, (positionStride + colorStride + textureCoordinateStride), (void*)positionStride);

	//Texture coordinates
	textureCoordinatesHandle = 2;
	glEnableVertexAttribArray(textureCoordinatesHandle);
	glVertexAttribPointer(textureCoordinatesHandle, coordinatesPerTexturePosition, GL_FLOAT, false, (positionStride + colorStride + textureCoordinateStride), (void*)(positionStride + colorStride));

	glFinish();//the c++ code will be blocked until openGL finishes whatever it is doing.

	/*
	//NOTES: position coordinates: lower Left (-1, -1) to upper right (1, 1).
					(-1, 1)			(1,1)
					(-1, -1)		(1,-1)

	//		 texture coordinates: upper Left (0,0) to lower right (1,1).
					(0,0)			(1,0)
					(0,1)			(1,1)
	*/
	glBindBuffer(GL_ARRAY_BUFFER, bufferIDs[3]);//Binding the next buffer
	glBindVertexArray(vaoIDs[3]);//Binding the next vao
	VAOindex = -1;


	delete [] positionsColorsAndTextures;
	positionsColorsAndTextures = new GLfloat[vertexNumCircle * (coordinatesPerPosition + coordinatesPerColor + coordinatesPerTexturePosition)];

	float angle = 0.0f;
	float xCoord = sin(angle) / 2;
	float yCoord = cos(angle) / 2;
	for (int i = 0; i < trianglesInCircle; i++)
	{
		positionsColorsAndTextures[++VAOindex] = xCoord;	positionsColorsAndTextures[++VAOindex] = yCoord;	positionsColorsAndTextures[++VAOindex] = 0.0f; positionsColorsAndTextures[++VAOindex] = 1.0f;
		positionsColorsAndTextures[++VAOindex] = 1.0f;	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 1.0f;
		positionsColorsAndTextures[++VAOindex] = xCoord;	positionsColorsAndTextures[++VAOindex] = -yCoord;

		positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 0.0f; positionsColorsAndTextures[++VAOindex] = 1.0f;
		positionsColorsAndTextures[++VAOindex] = 1.0f;	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 1.0f;
		positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 0.0f;

		angle += (6.28318 / trianglesInCircle);
		xCoord = sin(angle) / 2;
		yCoord = cos(angle) / 2;
		positionsColorsAndTextures[++VAOindex] = xCoord;	positionsColorsAndTextures[++VAOindex] = yCoord;	positionsColorsAndTextures[++VAOindex] = 0.0f; positionsColorsAndTextures[++VAOindex] = 1.0f;
		positionsColorsAndTextures[++VAOindex] = 1.0f;	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 1.0f;
		positionsColorsAndTextures[++VAOindex] = xCoord;	positionsColorsAndTextures[++VAOindex] = -yCoord;
	}

	//copy everything
	glBufferData(GL_ARRAY_BUFFER, vertexNumCircle * (positionStride + colorStride + textureCoordinateStride), positionsColorsAndTextures, GL_STATIC_DRAW);//If things stop working after locking the computer or something, change this to something other than static draw.

	//Position information
	positionHandle = 0;
	glEnableVertexAttribArray(positionHandle);
	glVertexAttribPointer(positionHandle, coordinatesPerPosition, GL_FLOAT, false, (positionStride + colorStride + textureCoordinateStride), 0);

	//Color information
	colorHandle = 1;
	glEnableVertexAttribArray(colorHandle);
	glVertexAttribPointer(colorHandle, coordinatesPerColor, GL_FLOAT, false, (positionStride + colorStride + textureCoordinateStride), (void*)positionStride);

	//Texture coordinates
	textureCoordinatesHandle = 2;
	glEnableVertexAttribArray(textureCoordinatesHandle);
	glVertexAttribPointer(textureCoordinatesHandle, coordinatesPerTexturePosition, GL_FLOAT, false, (positionStride + colorStride + textureCoordinateStride), (void*)(positionStride + colorStride));

	glFinish();//the c++ code will be blocked until openGL finishes whatever it is doing.

	/*
	//NOTES: position coordinates: lower Left (-1, -1) to upper right (1, 1).
					(-1, 1)			(1,1)
					(-1, -1)		(1,-1)

	//		 texture coordinates: upper Left (0,0) to lower right (1,1).
					(0,0)			(1,0)
					(0,1)			(1,1)
	*/

	delete[] positionsColorsAndTextures;
	positionsColorsAndTextures = new GLfloat[vertexNumRectangle * (coordinatesPerPosition + coordinatesPerColor + coordinatesPerTexturePosition)];
	glBindBuffer(GL_ARRAY_BUFFER, bufferIDs[4]);//Binding the next buffer
	glBindVertexArray(vaoIDs[4]);//Binding the next vao
	VAOindex = -1;

	//Vertex 1 lower Left
	positionsColorsAndTextures[++VAOindex] = -0.5f;	positionsColorsAndTextures[++VAOindex] = -0.5f;	positionsColorsAndTextures[++VAOindex] = 0.0f; positionsColorsAndTextures[++VAOindex] = 1.0f;
	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 1.0f;	positionsColorsAndTextures[++VAOindex] = 1.0f;
	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 1;

	//Vertex 2 lower Right
	positionsColorsAndTextures[++VAOindex] = 0.5f;	positionsColorsAndTextures[++VAOindex] = -0.5f;	positionsColorsAndTextures[++VAOindex] = 0.0f; positionsColorsAndTextures[++VAOindex] = 1.0f;
	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 1.0f;	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 1.0f;
	positionsColorsAndTextures[++VAOindex] = 1;		positionsColorsAndTextures[++VAOindex] = 1;

	//Vertex 3 upper Left
	positionsColorsAndTextures[++VAOindex] = -0.5f;	positionsColorsAndTextures[++VAOindex] = 0.5f;	positionsColorsAndTextures[++VAOindex] = 0.0f; positionsColorsAndTextures[++VAOindex] = 1.0f;
	positionsColorsAndTextures[++VAOindex] = 1.0f;	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 1.0f;
	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 0.0f;

	//Vertex 4 upper Left
	positionsColorsAndTextures[++VAOindex] = -0.5f;	positionsColorsAndTextures[++VAOindex] = 0.5f;	positionsColorsAndTextures[++VAOindex] = 0.0f; positionsColorsAndTextures[++VAOindex] = 1.0f;
	positionsColorsAndTextures[++VAOindex] = 1.0f;	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 1.0f;
	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 0.0f;

	//Vertex 5 lower Right
	positionsColorsAndTextures[++VAOindex] = 0.5f;	positionsColorsAndTextures[++VAOindex] = -0.5f;	positionsColorsAndTextures[++VAOindex] = 0.0f; positionsColorsAndTextures[++VAOindex] = 1.0f;
	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 1.0f;	positionsColorsAndTextures[++VAOindex] = 1.0f;
	positionsColorsAndTextures[++VAOindex] = 1;		positionsColorsAndTextures[++VAOindex] = 1;

	//Vertex 6 upper Right
	positionsColorsAndTextures[++VAOindex] = 0.5f;	positionsColorsAndTextures[++VAOindex] = 0.5f;	positionsColorsAndTextures[++VAOindex] = 0.0f; positionsColorsAndTextures[++VAOindex] = 1.0f;
	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 1.0f;	positionsColorsAndTextures[++VAOindex] = 0.0f;	positionsColorsAndTextures[++VAOindex] = 1.0f;
	positionsColorsAndTextures[++VAOindex] = 1;		positionsColorsAndTextures[++VAOindex] = 0.0f;

	//copy everything
	glBufferData(GL_ARRAY_BUFFER, vertexNumRectangle * (positionStride + colorStride + textureCoordinateStride), positionsColorsAndTextures, GL_STATIC_DRAW);//If things stop working after locking the computer or something, change this to something other than static draw.

	//Position information
	positionHandle = 0;
	glEnableVertexAttribArray(positionHandle);
	glVertexAttribPointer(positionHandle, coordinatesPerPosition, GL_FLOAT, false, (positionStride + colorStride + textureCoordinateStride), 0);

	//Color information
	colorHandle = 1;
	glEnableVertexAttribArray(colorHandle);
	glVertexAttribPointer(colorHandle, coordinatesPerColor, GL_FLOAT, false, (positionStride + colorStride + textureCoordinateStride), (void*)positionStride);

	//Texture coordinates
	textureCoordinatesHandle = 2;
	glEnableVertexAttribArray(textureCoordinatesHandle);
	glVertexAttribPointer(textureCoordinatesHandle, coordinatesPerTexturePosition, GL_FLOAT, false, (positionStride + colorStride + textureCoordinateStride), (void*)(positionStride + colorStride));


	glFinish();//the c++ code will be blocked until openGL finishes whatever it is doing.

}

GraphicsManager::~GraphicsManager()
{
	glUseProgram(0);
	glDeleteProgram(gLightingProgramID);
	glDeleteProgram(gUIProgramID);
	glDeleteProgram(gDebugProgramID);

	glDeleteVertexArrays(5, vaoIDs);
	glDeleteBuffers(5, bufferIDs);

	// Close and destroy the window
	SDL_DestroyWindow(pWindow);

	// Quit SDL subsystems
	SDL_Quit();
}

void GraphicsManager::Update()
{

}

void GraphicsManager::DrawWorld(bool debugMode)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (nullptr == playerTransform)
	{
		for (auto gameObject : gpGameObjectManager->mGameObjects)
		{
			if (gameObject->objectType == L"PLAYER")
			{
				playerTransform = static_cast<Transform*>(gameObject->GetComponent(L"Transform"));
				break;
			}
		}
	}

	if (nullptr == playerBody)
	{
		for (auto gameObject : gpGameObjectManager->mGameObjects)
		{
			if (gameObject->objectType == L"PLAYER")
			{
				playerBody = static_cast<Body*>(gameObject->GetComponent(L"Body"));
				break;
			}
		}
	}

	float targetX = 0;
	float targetY = 0;
	float targetZ = 0;

	if (nullptr != playerTransform)
	{
		targetX = playerTransform->positionX;
		targetY = playerTransform->positionY;
		targetZ = playerTransform->positionZ;
	}

	float cameraX = targetX;
	float cameraY = targetY;
	float cameraZ = targetZ + 100.0f;
	
	glm::vec3 cameraPosition = glm::vec3(cameraX, cameraY, cameraZ);//w coordinates?
	glm::vec3 targetPosition = glm::vec3(targetX, targetY, targetZ);
	glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 view = glm::lookAt(cameraPosition, targetPosition, upVector);

	glm::mat4 projection = glm::ortho(-(float)windowWidth / 2, (float)windowWidth / 2, -(float)windowHeight / 2, (float)windowHeight / 2, -1000.0f, 1000.0f);
	projection *= view;


	glUseProgram(gLightingProgramID);
	for (auto gameObject : gpGameObjectManager->mGameObjects)
	{
		if (gameObject->objectGroup == L"UI")
			continue;
		Transform* objectTransform = static_cast<Transform*>(gameObject->GetComponent(L"Transform"));
		Sprite* objectSprite = static_cast<Sprite*>(gameObject->GetComponent(L"Sprite"));
		if (nullptr == objectTransform || nullptr == objectSprite)
			continue;
		if (gameObject->objectGroup == L"AttackZone" && objectSprite->VAOID == 1)
			continue;
		float translationX = objectTransform->positionX;
		float translationY = objectTransform->positionY;
		float translationZ = objectTransform->positionZ;
		float rotation = objectTransform->rotation;
		float scaleX = objectTransform->scaleX;
		float scaleY = objectTransform->scaleY;
		float scaleZ = objectTransform->scaleZ;

		if (objectSprite->flipSprite)
		{
			scaleX = -scaleX;
		}

		float textureOffsetX = 0.5;
		float textureOffsetY = 0.5;

		if (gameObject->objectGroup == L"PlayerOnly" || gameObject->objectGroup == L"Scenery")
		{
			textureOffsetX = environmentSpritesheetPercentageX * (objectSprite->textureColumn - 1);
			textureOffsetY = environmentSpritesheetPercentageY * (objectSprite->textureRow - 1);
		}
		else if (gameObject->objectGroup == L"Character")
		{
			textureOffsetX = characterSpritesheetPercentageX * (objectSprite->textureColumn - 1);
			textureOffsetY = characterSpritesheetPercentageY * (objectSprite->textureRow - 1);
		}
		else if (gameObject->objectGroup == L"AttackZone")
		{
			textureOffsetX = 0.5;
			textureOffsetY = 0.5;
		}
		else if (gameObject->objectGroup == L"Arrow")
		{
			textureOffsetX = 0.0;
			textureOffsetY = 0.0;
		}

		glBindVertexArray(objectSprite->VAOID);//1 for environment rectangles, 2 for character rectangles, 3 UI rectangles, 4 for circles. 1 and 2 are different because of different spritesheet percentages.

		glm::mat4 translationRotationScale(1.0f);

		translationRotationScale = glm::translate(translationRotationScale, glm::vec3(translationX, translationY, translationZ));
		translationRotationScale = glm::rotate(translationRotationScale, glm::radians(rotation), glm::vec3(0.0f, 0.0f, -1.0f));  //Rotate in X, Y, or Z. (0,0,1). the -1 makes it a clockwise rotation.
		translationRotationScale = glm::scale(translationRotationScale, glm::vec3(scaleX, scaleY, scaleZ));

		//Transformation
		int transformationHandle = 3;
		glUniformMatrix4fv(transformationHandle, 1, GL_FALSE, glm::value_ptr(translationRotationScale));

		//Projection
		int projectionViewHandle = 4;
		glUniformMatrix4fv(projectionViewHandle, 1, GL_FALSE, glm::value_ptr(projection));

		//Texture code
		int textureOffsetHandle = 6;
		glUniform2f(textureOffsetHandle, textureOffsetX, textureOffsetY);

		glActiveTexture(GL_TEXTURE0);//We are using texture 0 for colors. In other situations, more textures could be used for things like normal maps. CS529 Nov10 2020
		glBindTexture(GL_TEXTURE_2D, objectSprite->spriteSheetTextureID);
		if (objectSprite->VAOID == 1 || objectSprite->VAOID == 2 || objectSprite->VAOID == 3)
		{
			glDrawArrays(GL_TRIANGLES, 0, vertexNumRectangle);
		}
		else if (objectSprite->VAOID == 4)
		{
			glDrawArrays(GL_TRIANGLES, 0, vertexNumCircle);
		}
		else if (objectSprite->VAOID == 5)
		{
			glDrawArrays(GL_TRIANGLES, 0, vertexNumRectangle);
		}

		glBindVertexArray(0);//Unbinding the VAO
		glBindTexture(GL_TEXTURE_2D, 0);//Unbinding the texture

		glFinish();//the c++ code will be blocked until openGL finishes whatever it is doing.

	}
		
	glUseProgram(gUIProgramID);
	for (auto gameObject : gpGameObjectManager->mGameObjects)
	{
		if (gameObject->objectGroup != L"UI")
			continue;
		Transform* objectTransform = static_cast<Transform*>(gameObject->GetComponent(L"Transform"));
		Sprite* objectSprite = static_cast<Sprite*>(gameObject->GetComponent(L"Sprite"));
		UIBar* objectUIBar = static_cast<UIBar*>(gameObject->GetComponent(L"UIBar"));
		if (nullptr == objectTransform || nullptr == objectSprite)
			continue;
		float translationX = objectTransform->positionX;
		float translationY = objectTransform->positionY;
		float translationZ = objectTransform->positionZ;
		float rotation = objectTransform->rotation;
		float scaleX = objectTransform->scaleX;
		float scaleY = objectTransform->scaleY;
		float scaleZ = objectTransform->scaleZ;
		if (gameObject->objectType == L"TitleScreen")
		{
			float scaleX = windowWidth;
			float scaleY = windowHeight;
		}

		if (gameObject->objectType == L"UIBar")
		{
			translationY = -0.9 - (0.05 * (objectUIBar->statToTrack - 1));
		}

		if (objectSprite->flipSprite)
		{
			scaleX = -scaleX;
		}

		float textureOffsetX = 0;
		float textureOffsetY = 0;
		if (gameObject->objectType == L"UIBar")
			textureOffsetY = (objectUIBar->statToTrack * 0.25);;


		glBindVertexArray(objectSprite->VAOID);

		glm::mat4 translationRotationScale(1.0f);

		translationRotationScale = glm::translate(translationRotationScale, glm::vec3(translationX, translationY, translationZ));
		translationRotationScale = glm::rotate(translationRotationScale, glm::radians(rotation), glm::vec3(0.0f, 0.0f, -1.0f));  //Rotate in X, Y, or Z. (0,0,1). the -1 makes it a clockwise rotation.
		translationRotationScale = glm::scale(translationRotationScale, glm::vec3(scaleX, scaleY, scaleZ));

		//Transformation
		int transformationHandle = 3;
		glUniformMatrix4fv(transformationHandle, 1, GL_FALSE, glm::value_ptr(translationRotationScale));

		//Texture code
		int textureOffsetHandle = 6;
		glUniform2f(textureOffsetHandle, textureOffsetX, textureOffsetY);

		glActiveTexture(GL_TEXTURE0);//We are using texture 0 for colors. In other situations, more textures could be used for things like normal maps. CS529 Nov10 2020
		glBindTexture(GL_TEXTURE_2D, objectSprite->spriteSheetTextureID);
		if (objectSprite->VAOID == 1 || objectSprite->VAOID == 2 || objectSprite->VAOID == 3 || objectSprite->VAOID == 5)
		{
			glDrawArrays(GL_TRIANGLES, 0, vertexNumRectangle);
		}
		else if (objectSprite->VAOID == 4)
		{
			glDrawArrays(GL_TRIANGLES, 0, vertexNumCircle);
		}

		glBindVertexArray(0);//Unbinding the VAO
		glBindTexture(GL_TEXTURE_2D, 0);//Unbinding the texture

		glFinish();//the c++ code will be blocked until openGL finishes whatever it is doing.
	}
	if (debugMode)
	{
		glUseProgram(gDebugProgramID);


		for (auto gameObject : gpGameObjectManager->mGameObjects)
		{
			Transform* objectTransform = static_cast<Transform*>(gameObject->GetComponent(L"Transform"));
			Body* objectBody = static_cast<Body*>(gameObject->GetComponent(L"Body"));
			if (nullptr == objectTransform || nullptr == objectBody)
				continue;
			float BodyX = objectBody->positionX;
			float BodyY = objectBody->positionY;
			float BodyZ = -1;
			
			float rotation = objectTransform->rotation;

			float scaleX = 1;
			float scaleY = 1;
			float scaleZ = 1;

			if (objectBody->shape->type == Shape::ShapeType::AABB)
			{
				ShapeAABB* objectShape = static_cast<ShapeAABB*>(objectBody->shape);
				scaleX = objectShape->right - objectShape->left;
				scaleY = objectShape->top - objectShape->bottom;
			}
			else if (objectBody->shape->type == Shape::ShapeType::CIRCLE)
			{
				ShapeCircle* objectShape = static_cast<ShapeCircle*>(objectBody->shape);
				scaleX = objectShape->radius * 2;
				scaleY = objectShape->radius * 2;
			}



			glBindVertexArray(objectBody->VAOID);

			glm::mat4 translationRotationScale(1.0f);

			translationRotationScale = glm::translate(translationRotationScale, glm::vec3(BodyX, BodyY, BodyZ));
			translationRotationScale = glm::rotate(translationRotationScale, glm::radians(rotation), glm::vec3(0.0f, 0.0f, -1.0f));  //Rotate in X, Y, or Z. (0,0,1). the -1 makes it a clockwise rotation.
			translationRotationScale = glm::scale(translationRotationScale, glm::vec3(scaleX, scaleY, scaleZ));



			//Transformation
			int transformationHandle = 3;
			glUniformMatrix4fv(transformationHandle, 1, GL_FALSE, glm::value_ptr(translationRotationScale));

			//Projection
			int projectionViewHandle = 4;
			glUniformMatrix4fv(projectionViewHandle, 1, GL_FALSE, glm::value_ptr(projection));


			if (objectBody->VAOID == 1 || objectBody->VAOID == 2 || objectBody->VAOID == 3 || objectBody->VAOID == 5)
			{
				glDrawArrays(GL_TRIANGLES, 0, vertexNumRectangle);
			}
			else if (objectBody->VAOID == 4)
			{
				glDrawArrays(GL_TRIANGLES, 0, vertexNumCircle);
			}

			glBindVertexArray(0);//Unbinding the VAO

			glFinish();//the c++ code will be blocked until openGL finishes whatever it is doing.


		}
	}

	SDL_GL_SwapWindow(pWindow);
}


void GraphicsManager::LoadShaders()
{
	{
		gLightingProgramID = glCreateProgram();

		//Vertex Shader

		std::ifstream vertexIn("lighting.vert");
		std::string vertContents((std::istreambuf_iterator<char>(vertexIn)), std::istreambuf_iterator<char>());
		const char* vertexShaderCode = vertContents.c_str();
		vertexIn.close();

		GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShaderID, 1, &vertexShaderCode, NULL);
		glCompileShader(vertexShaderID);
		GLint vertexShaderCompilationStatus = 0;
		glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &vertexShaderCompilationStatus);
		if (vertexShaderCompilationStatus == GL_TRUE)
		{
			glAttachShader(gLightingProgramID, vertexShaderID);
		}
		else
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(vertexShaderID, maxLength, &maxLength, &errorLog[0]);

			// Error Output
			std::string vertexShaderErrorLog(errorLog.begin(), errorLog.end());
			std::cout << "Vertex shader errror in program " << gLightingProgramID << ": " << vertexShaderErrorLog << std::endl;

			// Don't leak the shader.
			glDeleteShader(vertexShaderID);

			// Exit with failure.
			return;
		}

		//Fragment Shader

		std::ifstream fragmentIn("lighting.frag");
		std::string fragmentContents((std::istreambuf_iterator<char>(fragmentIn)), std::istreambuf_iterator<char>());
		fragmentIn.close();
		const char* fragmentShaderCode = fragmentContents.c_str();

		GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShaderID, 1, &fragmentShaderCode, NULL);
		glCompileShader(fragmentShaderID);

		GLint fragmentShaderCompilationStatus = 0;
		glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &fragmentShaderCompilationStatus);
		if (fragmentShaderCompilationStatus == GL_TRUE)
		{
			glAttachShader(gLightingProgramID, fragmentShaderID);
		}
		else
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(fragmentShaderID, maxLength, &maxLength, &errorLog[0]);

			// Error Output
			std::string fragmentShaderErrorLog(errorLog.begin(), errorLog.end());
			std::cout << "Fragment shader errror in program " << gLightingProgramID << ": " << fragmentShaderErrorLog << std::endl;

			// Don't leak the shader.
			glDeleteShader(fragmentShaderID);

			// Exit with failure.
			return;
		}
		glLinkProgram(gLightingProgramID);
		GLint programLinkingStatus = 0;
		glGetProgramiv(gLightingProgramID, GL_LINK_STATUS, &programLinkingStatus);
		if (programLinkingStatus == GL_TRUE)
		{
			//glUseProgram(gLightingProgramID);
		}
		else
		{
			GLint maxLength = 0;
			glGetProgramiv(gLightingProgramID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			glGetProgramInfoLog(gLightingProgramID, maxLength, &maxLength, &errorLog[0]);

			// Error Output
			std::string programErrorLog(errorLog.begin(), errorLog.end());
			std::cout << "Program error in program " << gLightingProgramID << ": " << programErrorLog << std::endl;

			// The program is useless now. So delete it.
			glDeleteProgram(gLightingProgramID);

			// Exit with failure.
			return;
		}
	}




	{
		gUIProgramID = glCreateProgram();

		//Vertex Shader

		std::ifstream vertexIn("UI.vert");
		std::string vertContents((std::istreambuf_iterator<char>(vertexIn)), std::istreambuf_iterator<char>());
		const char* vertexShaderCode = vertContents.c_str();
		vertexIn.close();

		GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShaderID, 1, &vertexShaderCode, NULL);
		glCompileShader(vertexShaderID);
		GLint vertexShaderCompilationStatus = 0;
		glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &vertexShaderCompilationStatus);
		if (vertexShaderCompilationStatus == GL_TRUE)
		{
			glAttachShader(gUIProgramID, vertexShaderID);
		}
		else
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(vertexShaderID, maxLength, &maxLength, &errorLog[0]);

			// Error Output
			std::string vertexShaderErrorLog(errorLog.begin(), errorLog.end());
			std::cout << "Vertex shader errror in program " << gUIProgramID << ": " << vertexShaderErrorLog << std::endl;

			// Don't leak the shader.
			glDeleteShader(vertexShaderID);

			// Exit with failure.
			return;
		}

		//Fragment Shader

		std::ifstream fragmentIn("UI.frag");
		std::string fragmentContents((std::istreambuf_iterator<char>(fragmentIn)), std::istreambuf_iterator<char>());
		fragmentIn.close();
		const char* fragmentShaderCode = fragmentContents.c_str();

		GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShaderID, 1, &fragmentShaderCode, NULL);
		glCompileShader(fragmentShaderID);

		GLint fragmentShaderCompilationStatus = 0;
		glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &fragmentShaderCompilationStatus);
		if (fragmentShaderCompilationStatus == GL_TRUE)
		{
			glAttachShader(gUIProgramID, fragmentShaderID);
		}
		else
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(fragmentShaderID, maxLength, &maxLength, &errorLog[0]);

			// Error Output
			std::string fragmentShaderErrorLog(errorLog.begin(), errorLog.end());
			std::cout << "Fragment shader errror in program " << gUIProgramID << ": " << fragmentShaderErrorLog << std::endl;

			// Don't leak the shader.
			glDeleteShader(fragmentShaderID);

			// Exit with failure.
			return;
		}
		glLinkProgram(gUIProgramID);
		GLint programLinkingStatus = 0;
		glGetProgramiv(gUIProgramID, GL_LINK_STATUS, &programLinkingStatus);
		if (programLinkingStatus == GL_TRUE)
		{
			//glUseProgram(gLightingProgramID);
		}
		else
		{
			GLint maxLength = 0;
			glGetProgramiv(gUIProgramID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			glGetProgramInfoLog(gUIProgramID, maxLength, &maxLength, &errorLog[0]);

			// Error Output
			std::string programErrorLog(errorLog.begin(), errorLog.end());
			std::cout << "Program error in program " << gUIProgramID << ": " << programErrorLog << std::endl;

			// The program is useless now. So delete it.
			glDeleteProgram(gUIProgramID);

			// Exit with failure.
			return;
		}
	}



	{
		gDebugProgramID = glCreateProgram();

		//Vertex Shader

		std::ifstream vertexIn("debug.vert");
		std::string vertContents((std::istreambuf_iterator<char>(vertexIn)), std::istreambuf_iterator<char>());
		const char* vertexShaderCode = vertContents.c_str();
		vertexIn.close();

		GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShaderID, 1, &vertexShaderCode, NULL);
		glCompileShader(vertexShaderID);
		GLint vertexShaderCompilationStatus = 0;
		glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &vertexShaderCompilationStatus);
		if (vertexShaderCompilationStatus == GL_TRUE)
		{
			glAttachShader(gDebugProgramID, vertexShaderID);
		}
		else
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(vertexShaderID, maxLength, &maxLength, &errorLog[0]);

			// Error Output
			std::string vertexShaderErrorLog(errorLog.begin(), errorLog.end());
			std::cout << "Vertex shader errror in program " << gDebugProgramID << ": " << vertexShaderErrorLog << std::endl;

			// Don't leak the shader.
			glDeleteShader(vertexShaderID);

			// Exit with failure.
			return;
		}

		//Fragment Shader

		std::ifstream fragmentIn("debug.frag");
		std::string fragmentContents((std::istreambuf_iterator<char>(fragmentIn)), std::istreambuf_iterator<char>());
		fragmentIn.close();
		const char* fragmentShaderCode = fragmentContents.c_str();

		GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShaderID, 1, &fragmentShaderCode, NULL);
		glCompileShader(fragmentShaderID);

		GLint fragmentShaderCompilationStatus = 0;
		glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &fragmentShaderCompilationStatus);
		if (fragmentShaderCompilationStatus == GL_TRUE)
		{
			glAttachShader(gDebugProgramID, fragmentShaderID);
		}
		else
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(fragmentShaderID, maxLength, &maxLength, &errorLog[0]);

			// Error Output
			std::string fragmentShaderErrorLog(errorLog.begin(), errorLog.end());
			std::cout << "Fragment shader errror in program " << gDebugProgramID << ": " << fragmentShaderErrorLog << std::endl;

			// Don't leak the shader.
			glDeleteShader(fragmentShaderID);

			// Exit with failure.
			return;
		}
		glLinkProgram(gDebugProgramID);
		GLint programLinkingStatus = 0;
		glGetProgramiv(gDebugProgramID, GL_LINK_STATUS, &programLinkingStatus);
		if (programLinkingStatus == GL_TRUE)
		{
			//glUseProgram(gDebugProgramID);
		}
		else
		{
			GLint maxLength = 0;
			glGetProgramiv(gDebugProgramID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			glGetProgramInfoLog(gDebugProgramID, maxLength, &maxLength, &errorLog[0]);

			// Error Output
			std::string programErrorLog(errorLog.begin(), errorLog.end());
			std::cout << "Program error in program " << gDebugProgramID << ": " << programErrorLog << std::endl;

			// The program is useless now. So delete it.
			glDeleteProgram(gDebugProgramID);

			// Exit with failure.
			return;
		}
	}


}