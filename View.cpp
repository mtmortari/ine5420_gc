#include "View.h"


void View::setXMin(int x_min)
{
	this->x_min = x_min;
}

void View::setYMin(int y_min)
{
	this->y_min = y_min;
}

void View::setXMax(int x_max)
{
	this->x_max = x_max;
}

void View::setYMax(int y_max)
{
	this->y_max = y_max;
}

int View::getXMin()
{
	return x_min;
}

int View::getXMax()
{
	return x_max;
}

int View::getYMin()
{
	return y_min;
}

int View::getYMax()
{
	return y_max;
}
