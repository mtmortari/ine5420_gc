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



void DrawableObject::rotate(double angle)
{
	MatrixUtils utils;
	Point3D center = this->calcGeometricCenter();


	std::cout << "x center" << center.getX() << "\n";
	std::cout << "y center" << center.getY() << "\n";	


	Matrix3D translateToOrigin = utils.createTranslationMatrix(center);
	Matrix3D rotateAroundOrigin = utils.createRotationMatrix(angle);
	center.scalarProduct(-1.0);
	Matrix3D translateBack = utils.createTranslationMatrix(center);

	Matrix3D result = (translateToOrigin * rotateAroundOrigin) * translateBack;

	std::cout << "t1 " << translateToOrigin.getMatrix().matrix[0][0] << "\n";
	std::cout << "t2 " << rotateAroundOrigin.getMatrix().matrix[0][0] << "\n";
	std::cout << "t3 " << translateBack.getMatrix().matrix[0][0] << "\n";
	std::cout << "t4 " << result.getMatrix().matrix[0][0] << "\n";

	std::list<Point3D>::iterator it;
    for (it= points.begin(); it != points.end(); ++it)
    {        
    	Point3D point = it->multiply(result);
     	it->setX(point.getX());
     	it->setY(point.getY());
     	it->setZ(point.getZ());

     	std::cout << "x " << it->getX() << " point " << point.getX() << "\n";
		std::cout << "y " << it->getY() << " point " << point.getY() << "\n";
    }    

}