#include "Matrix3D.h"

void Matrix3D::setMatrix(Matrix3DStruct matrix)
{
	this->matrix = matrix;
}


Matrix3DStruct Matrix3D::getMatrix()
{
	return this->matrix;
}


Matrix3D Matrix3D::scalarProduct(double value)
{
	Matrix3DStruct newMatrix;

	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)	
		{
			newMatrix.matrix[i][j] = value * this->matrix.matrix[i][j];
		}
	}

	Matrix3D newMatrix3D;
	newMatrix3D.setMatrix(newMatrix);
	return newMatrix3D;
}

Matrix3D Matrix3D::operator+(Matrix3D other)
{
	Matrix3DStruct newMatrix;

	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)	
		{
			newMatrix.matrix[i][j] = this->matrix.matrix[i][j] + other.getMatrix().matrix[i][j];
		}
	}

	Matrix3D newMatrix3D;
	newMatrix3D.setMatrix(newMatrix);
	return newMatrix3D;
}


Matrix3D Matrix3D::operator-(Matrix3D other)
{

	Matrix3DStruct newMatrix;

	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)	
		{
			newMatrix.matrix[i][j] = this->matrix.matrix[i][j] - other.getMatrix().matrix[i][j];
		}
	}

	Matrix3D newMatrix3D;
	newMatrix3D.setMatrix(newMatrix);
	return newMatrix3D;
}

Matrix3D Matrix3D::operator*(Matrix3D other)
{
	Matrix3DStruct newMatrix;


	// newMatrix = this * other
	for(int i = 0; i<=3; i++)
	{
		for(int j = 0; j <= 3; j++)
		{
			double soma = 0;
			for(int k = 0; k <=3 ; k++)
			{
				soma += this->matrix.matrix[i][k] * other.getMatrix().matrix[k][j];
				
			} 
			newMatrix.matrix[i][j] = soma;	
		}
	}

	Matrix3D newMatrix3D;
	newMatrix3D.setMatrix(newMatrix);
	return newMatrix3D;
}