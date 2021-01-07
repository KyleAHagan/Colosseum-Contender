#include "CollisionManager.h"
#include "Components/Body.h"

Shape::Shape(ShapeType Type)
{
	ownerBody = nullptr;
	type = Type;
}

ShapeCircle::ShapeCircle(float _radius) : Shape(Shape::ShapeType::CIRCLE)
{
	radius = _radius;
}


bool ShapeCircle::TestPoint(float PointX, float PointY)
{
	//Center point squared distance
	float squaredDistance = (PointX - ownerBody->positionX) * (PointX - ownerBody->positionX) + (PointY - ownerBody->positionY) * (PointY - ownerBody->positionY);

	//Compare to squared radius
	if (squaredDistance > (radius * radius))
		return false;

	return true;
}

ShapeAABB::ShapeAABB(float _top, float _right, float _bottom, float _left) : Shape(Shape::ShapeType::AABB) //AABB means Axis Aligned Bounding Box
{
	top = _top;
	right = _right;
	bottom = _bottom;
	left = _left;
}

bool ShapeAABB::TestPoint(float PointX, float PointY) //CS529 November 16
{
	float currentTop, currentRight, currentLeft, currentBottom;

	currentTop = ownerBody->previousPositionX + top;
	currentRight = ownerBody->previousPositionX + right;
	currentBottom = ownerBody->previousPositionX + bottom;
	currentLeft = ownerBody->previousPositionX + left;


	if (PointX < currentLeft || PointX > currentRight || PointY > currentTop || PointY < currentBottom)
		return false;

	return true;
}

//"These 4 functions are hidden here [not in the header. Only collision manager can see them.] on purpose, since they manipulate the collision manager's contacts array." CS 529 November 16 2020 0h52m30s

bool CheckCollisionCircleCircle(Shape* shape1, float pos1x, float pos1y, Shape* shape2, float pos2x, float pos2y, std::list<Contact*>& contacts)
{
	float circleToCircle2DistanceSquared;
	float radius1, radius2;

	circleToCircle2DistanceSquared = (pos1x - pos2x) * (pos1x - pos2x) + (pos1y - pos2y) * (pos1y - pos2y);
	radius1 = ((ShapeCircle*)shape1)->radius;
	radius2 = ((ShapeCircle*)shape2)->radius;

	if (circleToCircle2DistanceSquared > ((radius1 + radius2) * (radius1 + radius2))) 
	{
		return false;
	}

	//Add new contact
	Contact* newContact = new Contact();
	newContact->bodies[0] = shape1->ownerBody;
	newContact->bodies[1] = shape2->ownerBody;
	contacts.push_back(newContact);

	return true;
}

bool CheckCollisionAABBAABB(Shape* shape1, float pos1x, float pos1y, Shape* shape2, float pos2x, float pos2y, std::list<Contact*>& contacts)
{
	float top1, right1, bottom1, left1;
	float top2, right2, bottom2, left2;

	ShapeAABB* AABB1 = (ShapeAABB*)shape1;
	ShapeAABB* AABB2 = (ShapeAABB*)shape2;

	top1 = pos1y + AABB1->top;
	right1 = pos1x + AABB1->right;
	bottom1 = pos1y + AABB1->bottom;
	left1 = pos1x + AABB1->left;

	top2 = pos2y + AABB2->top;
	right2 = pos2x + AABB2->right;
	bottom2 = pos2y + AABB2->bottom;
	left2 = pos2x + AABB2->left;

	if (left1 > right2 || left2 > right1 || top1 < bottom2 || top2 < bottom1)
		return false;

	//Add new contact
	Contact* newContact = new Contact();
	newContact->bodies[0] = shape1->ownerBody;
	newContact->bodies[1] = shape2->ownerBody;
	contacts.push_back(newContact);

	return true;
}

//November 16 49m16s
bool CheckCollisionCircleAABB(Shape* shape1, float circleCenterX, float circleCenterY, Shape* shape2, float boxCenterX, float boxCenterY, std::list<Contact*>& contacts)
{
	ShapeCircle* Circle1 = (ShapeCircle*)shape1;
	ShapeAABB* AABB2 = (ShapeAABB*)shape2;
	float boxLeft, boxRight, boxTop, boxBottom;

	boxTop = boxCenterY + AABB2->top;
	boxRight = boxCenterX + AABB2->right;
	boxBottom = boxCenterY + AABB2->bottom; //plus a negative number
	boxLeft = boxCenterX + AABB2->left; //plus a negative number

	float closestX, closestY;

	if (circleCenterX < boxLeft)
		closestX = boxLeft;
	else if (circleCenterX > boxRight)
		closestX = boxRight;
	else
		closestX = circleCenterX;

	if (circleCenterY > boxTop)
		closestY = boxTop;
	else if (circleCenterY < boxBottom)
		closestY = boxBottom;
	else
		closestY = circleCenterY;

	if (false == Circle1->TestPoint(closestX, closestY))
		return false;



	//Add new contact
	Contact* newContact = new Contact();
	newContact->bodies[0] = shape1->ownerBody;
	newContact->bodies[1] = shape2->ownerBody;
	contacts.push_back(newContact);

	return true;

}

bool CheckCollisionAABBCircle(Shape* shape1, float pos1x, float pos1y, Shape* shape2, float pos2x, float pos2y, std::list<Contact*>& contacts)
{
	return CheckCollisionCircleAABB(shape2, pos2x, pos2y, shape1, pos1x, pos1y, contacts);
}

bool CollisionManager::CheckCollisionAndGenerateContact(Shape* shape1, float pos1x, float pos1y, Shape* shape2, float pos2x, float pos2y)
{
	return CollisionFunctions[shape1->type][shape2->type](shape1, pos1x, pos1y, shape2, pos2x, pos2y, contacts);
}


CollisionManager::CollisionManager()
{
	CollisionFunctions[Shape::ShapeType::CIRCLE][Shape::ShapeType::CIRCLE] = CheckCollisionCircleCircle;
	CollisionFunctions[Shape::ShapeType::AABB][Shape::ShapeType::AABB] = CheckCollisionAABBAABB;
	CollisionFunctions[Shape::ShapeType::CIRCLE][Shape::ShapeType::AABB] = CheckCollisionCircleAABB;
	CollisionFunctions[Shape::ShapeType::AABB][Shape::ShapeType::CIRCLE] = CheckCollisionAABBCircle;
}

void CollisionManager::Reset()
{
	//clear dynamically allocated contacts between game loops.
	for (auto contact : contacts)
	{
		delete contact;
	}
	contacts.clear();
}

CollisionManager::~CollisionManager()
{
	Reset();
}