#include "Matrix3D.h"

void Matrix3D::setMatrix(Matrix3DStruct matrix)
{
	this->matrix = matrix;
}


Matrix3DStruct Matrix3D::getMatrix()
{
	return this->matrix;
}


Point3D Matrix3D::translation(Point3D point)
{
	Point3D newPoint;
	newPoint.setX(point.getX() + this->matrix.matrix[2][0]);
	newPoint.setY(point.getX() + this->matrix.matrix[2][1]);
	newPoint.setZ(1);	
}