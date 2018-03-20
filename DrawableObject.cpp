#include "DrawableObject.h"

void DrawableObject::setName(std::string name)
{
	this->name = name;
}
		
std::string DrawableObject::getName()
{
	return name;
}
		
void DrawableObject::setType(ObjectType type)
{
	this->type = type;
}
		
ObjectType DrawableObject::geType()
{
	return type;
}

void DrawableObject::setPoints(std::list<Point3D> points)
{
	this->points = points;
}

std::list<Point3D> DrawableObject::getPoints()
{
	return points;
}
