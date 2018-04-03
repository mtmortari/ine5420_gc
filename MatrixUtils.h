/*
 represents a 3x3 matrix for 3D calculations
*/

#include <math.h>  

#include "Point3D.h"

#ifndef MARTIXUTILS_H
#define MATRIXUTILS_H

class MatrixUtils{
	public:		
		Matrix3D createRotationMatrix(double angle);
		Matrix3D createTranslationMatrix(Point3D point);

};

#endif