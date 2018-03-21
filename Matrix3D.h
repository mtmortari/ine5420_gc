/*
 represents a 3x3 matrix for 3D calculations
*/


#include "Matrix3DStruct.cpp"

#ifndef MATRIX3D_H
#define MATRIX3D_H

class Matrix3D{
	private:
		Matrix3DStruct matrix;

	public:
		void setMatrix(Matrix3DStruct);
		Matrix3DStruct getMatrix();

		Matrix3D scalarProduct(double value);
		Matrix3D operator+(Matrix3D other);
		Matrix3D operator-(Matrix3D other);
		Matrix3D operator*(Matrix3D other);  //matrix multiplication


};

#endif