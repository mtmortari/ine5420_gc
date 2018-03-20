/*
 represents a 2x2 matrix for 2D calculations
*/

#include "Point2D.h"
#include "Matrix2DStruct.cpp"

#ifndef MARTIX2D_H
#define MATRIX2D_H

class Matrix2D{
	private:
		Matrix2DStruct matrix;

	public:
		void setMatrix(Matrix2DStruct);
		Matrix2DStruct getMatrix();

		Point2D translation(Point2D point);


};

#endif