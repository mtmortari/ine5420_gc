#include "MatrixUtils.h"

Matrix3D MatrixUtils::createRotationMatrix(double angle)
{
	Matrix3DStruct matrixStruct;
	matrixStruct.matrix[0][0] = cos(angle);
	matrixStruct.matrix[0][1] = -sin(angle);
	matrixStruct.matrix[0][2] = 0;

	matrixStruct.matrix[1][0] = sin(angle);
	matrixStruct.matrix[1][1] = cos(angle);
	matrixStruct.matrix[1][2] = 0;

	matrixStruct.matrix[2][0] = 0;
	matrixStruct.matrix[2][1] = 0;
	matrixStruct.matrix[2][2] = 1;

	Matrix3D matrix;						    
	matrix.setMatrix(matrixStruct);
	return matrix;
}


Matrix3D MatrixUtils::createTranslationMatrix(Point3D point)
{
	Matrix3DStruct matrixStruct;
	matrixStruct.matrix[0][0] = 1;
	matrixStruct.matrix[0][1] = 0;
	matrixStruct.matrix[0][2] = 0;

	matrixStruct.matrix[1][0] = 0;
	matrixStruct.matrix[1][1] = 1;
	matrixStruct.matrix[1][2] = 0;

	matrixStruct.matrix[2][0] = point.getX();
	matrixStruct.matrix[2][1] = point.getY();
	matrixStruct.matrix[2][2] = 1;

	Matrix3D matrix;						    
	matrix.setMatrix(matrixStruct);
	return matrix;
}