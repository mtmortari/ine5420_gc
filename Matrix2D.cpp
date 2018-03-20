#include "Matrix2D.h"

void Matrix2D::setMatrix(Matrix2DStruct matrix)
{
	this->matrix = matrix;
}


Matrix2DStruct Matrix2D::getMatrix()
{
	return this->matrix;
}


Point2D Matrix2D::translation(Point2D point)
{
	Point2D newPoint;
	newPoint.setX(point.getX() * this->matrix.matrix[0][0]);
	newPoint.setY(point.getX() * this->matrix.matrix[1][1]);
}