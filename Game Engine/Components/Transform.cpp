#include "Transform.h"

Transform::Transform() : Component(L"Transform")
{
	positionX = 0.0f;
	positionY = 0.0f;
	positionZ = 0.0f;
	rotation = 0.0f;
	scaleX = 100.0f;
	scaleY = 100.0f;
	scaleZ = 100.0f;
}
Transform::~Transform()
{

}
void Transform::Update()
{

}

void Transform::Serialize(std::wfstream& InputStream)
{
	InputStream >> positionX;
	InputStream >> positionY;
	InputStream >> positionZ;
	InputStream >> rotation;
	InputStream >> scaleX;
	InputStream >> scaleY;
	InputStream >> scaleZ;

}