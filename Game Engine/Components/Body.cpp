#include "Body.h"
#include "Transform.h"
#include "../GameObject.h"
#include "../CollisionManager.h"

Body::Body() : Component(L"Body")
{
	shape = nullptr;

	xScale = 100;
	yScale = 100;

	positionX = 0.0f;
	positionY = 0.0f;
	positionZ = 0.0f;

	previousPositionX = 0.0f;
	previousPositionY = 0.0f;

	velocityX = 0.0f;
	velocityY = 0.0f;

	accelerationX = 0.0f;
	accelerationY = 0.0f;

	totalForceX = 0.0f;
	totalForceY = 0.0f;

	mass = 1.0f;
	inverseMass = 1.0f;// Inverse mass is useful for calculating acceleration from force. Force = Mass * Accelertion, so Acceleration = Force / Mass. (Force is the sum of all forces acting on the object. Multiplication is faster than division.) CS529 October 26, 2020

}

void Body::Initialize(float newXScale, float newYScale, float newMass, std::wstring shapeName, int newVAOID)
{
	xScale = newXScale;
	yScale = newYScale;
	zScale = 1;

	previousPositionX = 0.0f;
	previousPositionY = 0.0f;

	velocityX = 0.0f;
	velocityY = 0.0f;

	accelerationX = 0.0f;
	accelerationY = 0.0f;

	totalForceX = 0.0f;
	totalForceY = 0.0f;

	mass = newMass;

	if (newMass != 0.0f)
		inverseMass = 1.0f / newMass;
	else
		inverseMass = 0.0f; //An object with an inverse mass of 0 will never move.

	shape = nullptr;

	if (L"Circle" == shapeName)
	{
		shape = new ShapeCircle(xScale/2);
		shape->ownerBody = this;

	}
	else if (L"AABB" == shapeName)
	{
		shape = new ShapeAABB(yScale/2, xScale/2, -yScale/2, -xScale/2);
		shape->ownerBody = this;
	}

	VAOID = newVAOID;
}

Body::~Body()
{
	if (shape != nullptr)
		delete shape;
}

void Body::Update()
{

}
void Body::Serialize(std::wfstream& InputStream)
{
	InputStream >> xScale;
	InputStream >> yScale;
	zScale = 1;
	InputStream >> mass;
	if (mass != 0.0f)
		inverseMass = 1.0f / mass;
	else
		inverseMass = 0.0f; //An object with an inverse mass of 0 will never move.
	
	std::wstring shapeName;
	InputStream >> shapeName;
	if (L"Circle" == shapeName)
	{
		shape = new ShapeCircle(xScale/2);
		shape->ownerBody = this;
		
	}
	else if (L"AABB" == shapeName)
	{
		shape = new ShapeAABB(yScale/2, xScale/2, yScale/2, xScale/2);
		shape->ownerBody = this;
	}
	InputStream >> VAOID;
}

void Body::Integrate(float gravity, float deltaTime)
{
	deltaTime = std::min(deltaTime, 0.4f); //attempt to keep the physics from going wild when the game is paused.
	Transform* transform = static_cast<Transform*>(mpOwner->GetComponent(L"Transform"));
	if (nullptr != transform)
	{
		positionX = transform->positionX;
		positionY = transform->positionY;
	}



	previousPositionX = positionX;
	previousPositionY = positionY;

	//Compute the acceleration
	accelerationX = totalForceX * inverseMass;
	accelerationY = totalForceY * inverseMass;

	totalForceX = 0.0f; //Forces are usually only applied for one game loop. CS529 October 26 2020.
	totalForceY = 0.0f;

	velocityX *= 0.98;//friction
	velocityY *= 0.98;//friction

	//Integrate the velocity  V1 = a*t + V0
	velocityX += accelerationX * deltaTime;
	velocityY += accelerationY * deltaTime;

	//Integrate the position  P1 = v*t + P0
	positionX += velocityX * deltaTime;
	positionY += velocityY * deltaTime;

	if (nullptr != transform)
	{
		transform->positionX = positionX;
		transform->positionY = positionY;
	}
}
