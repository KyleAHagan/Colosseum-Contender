#pragma once

#include "Component.h"

class GameObject;
class Shape;

class Body : public Component
{
public:

	Body();
	~Body();


	void Initialize(float newXScale, float newYScale, float newMass, std::wstring shapeName, int newVAOID);
	void Update();
	void Serialize(std::wfstream& InputStream);
	void Integrate(float gravity, float deltaTime);

public:
	float xScale, yScale, zScale;
	float positionX, positionY, positionZ; //Things have a z position that does not change (depth).
	float previousPositionX, previousPositionY;
	float velocityX, velocityY;
	float accelerationX, accelerationY;
	float totalForceX, totalForceY;
	float mass, inverseMass;// Inverse mass is useful for calculating acceleration from force. Force = Mass * Accelertion, so Acceleration = Force / Mass. (Force is the sum of all forces acting on the object. Multiplication is faster than division.) CS529 October 26, 2020
	Shape* shape;
	int VAOID;

private:
private:
};