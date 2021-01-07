#pragma once
#include <list>


class Body; //Forward declaring rather than including.

class Shape
{
public:
	enum ShapeType
	{
		CIRCLE,
		AABB,

		NUM    //Just to keep track of how many there are CS529 November 16 2020 0h11m
	};

	Shape(ShapeType Type);
	virtual ~Shape() { }

	virtual bool TestPoint(float PointX, float PointY) = 0;

public:
	Body* ownerBody;
	ShapeType type;

private:

private:

};

class ShapeCircle : public Shape
{
public:
	ShapeCircle(float radius);
	~ShapeCircle() { }

	bool TestPoint(float PointX, float PointY);


public:
	float radius;

private:

private:
};

class ShapeAABB : public Shape
{
public:
	ShapeAABB(float top, float right, float bottom, float left);
	~ShapeAABB() { }

	bool TestPoint(float PointX, float PointY);


public:
	float top;
	float right;
	float bottom;
	float left;

private:

private:
};

class Contact
{
public:
	Contact()
	{
		bodies[0] = nullptr;
		bodies[1] = nullptr;
	}
	~Contact() { }
public:
	Body* bodies[2];

private:

private:

};



class CollisionManager
{
public:
	CollisionManager();
	~CollisionManager();

	void Reset();//deletes all contacts. Only processing contacts in current game loop.

	bool CheckCollisionAndGenerateContact(Shape* shape1, float pos1x, float pos1y, Shape* shape2, float pos2x, float pos2y);

public:
	std::list<Contact*> contacts;

	//2D array of collision function pointers.
	bool (*CollisionFunctions[Shape::ShapeType::NUM][Shape::ShapeType::NUM])(Shape* shape1, float pos1x, float pos1y, Shape* shape2, float pos2x, float pos2y, std::list<Contact*>& contacts);

private:

private:

};