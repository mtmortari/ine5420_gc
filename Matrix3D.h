/*
 represents a 3x3 matrix for 2D calculations
*/

#include "Point3D.h"
#include "Matrix3DStruct.cpp"

#ifndef MARTIX3D_H
#define MATRIX3D_H

class Matrix3D{
	private:
		Matrix3DStruct matrix;

	public:
		void setMatrix(Matrix3DStruct);
		Matrix3DStruct getMatrix();

		Point3D translation(Point3D point);


};

#endif