/*
 represents a 3D point
*/

#ifndef POINT3D_H
#define POINT3D_H

class Point3D{
	private:
		double x, y, z;

	public:
		void setX(double x);
		void setY(double y);
		void setZ(double z);
		double getX();
		double getY();
		double getZ();
};

#endif