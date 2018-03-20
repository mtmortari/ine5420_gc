/* 
	represents an object that is gonna be drawn in the window
*/

#include <list>
#include <string>
#include "ObjectType.cpp"
#include "Point3D.h"

#ifndef DRAWABLEOBJECT_H
#define DRAWABLEOBJECT_H

class DrawableObject{
	private:
		std::string name;
		ObjectType type;
		std::list<Point3D> points;

	public:		
		void setName(std::string name);
		std::string getName();
		void setType(ObjectType type);
		ObjectType geType();
		void setPoints(std::list<Point3D> points);
		std::list<Point3D> getPoints();
};

#endif