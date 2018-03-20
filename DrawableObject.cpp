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

Point3D DrawableObject::calcGeometricCenter()
{
	double x_center = 0.0;
	double y_center = 0.0;
	int number_points = 0;

	std::list<Point3D>::iterator it;
    for (it= points.begin(); it != points.end(); ++it)
    {                  
      x_center += it->getX();
      y_center += it->getY();
      number_points++;
    }    

    Point3D point;
    point.setX(x_center/number_points);
    point.setY(y_center/number_points);

    return point;
}
