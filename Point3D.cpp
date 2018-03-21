#include "Point3D.h"

void Point3D::setX(double x)
{
	this->x = x;
}

void Point3D::setY(double y)
{
	this->y = y;
}

void Point3D::setZ(double z)
{
	this->z = z;
}

double Point3D::getX()
{
	return x;
}

double Point3D::getY()
{
	return y;
}


double Point3D::getZ()
{
	return z;
}


Point3D Point3D::multiply(Matrix3D matrix)
{
	
	double p[3] = {this->x, this-> y, this->z};
	double c[3] = {0, 0,0};
	// C = A * B	
	for(int j = 0; j <= 3; j++)
	{		
		for(int k = 0; k <=3 ; k++)
		{
			c[j] += p[j] * matrix.getMatrix().matrix[k][j];			
		} 		
	}

	Point3D point;
	point.setX(c[0]);
	point.setY(c[1]);
	point.setZ(c[2]);
}

void Point3D::scalarProduct(double value)
{
	this->x = this->x * value;
	this->y = this->y * value;
	this->y = this->y * value;
}