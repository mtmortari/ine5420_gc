#include "View.h"

double View::transformX(double x_w, View window)
{
	return ((x_w - window.getXMin()) / ( window.getXMax() - window.getXMin() ))*(this->x_max - this->x_min);
}

double View::transformY(double y_w, View window)
{
	return ( 1 - ( (y_w - window.getYMin()) / ( window.getYMax() - window.getYMin()) ) )*(this->y_max - this->y_min);
}

void View::setXMin(double x_min)
{
	this->x_min = x_min;
}

void View::setYMin(double y_min)
{
	this->y_min = y_min;
}

void View::setXMax(double x_max)
{
	this->x_max = x_max;
}

void View::setYMax(double y_max)
{
	this->y_max = y_max;
}

double View::getXMin()
{
	return x_min;
}

double View::getXMax()
{
	return x_max;
}

double View::getYMin()
{
	return y_min;
}

double View::getYMax()
{
	return y_max;
}
