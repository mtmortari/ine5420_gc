/*
 represents a 2D point
*/

#ifndef POINT2D_H
#define POINT2D_H

class Point2D{
	private:
		double x, y;

	public:
		void setX(double x);
		void setY(double y);
		double getX();
		double getY();
};

#endif