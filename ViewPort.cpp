#include "ViewPort.h"


void ViewPort::setXMin(int x_min)
{
	this->x_min = x_min;
}

void ViewPort::setYMin(int y_min)
{
	this->y_min = y_min;
}

void ViewPort::setXMax(int x_max)
{
	this->x_max = x_max;
}

void ViewPort::setYMax(int y_max)
{
	this->y_max = y_max;
}

int ViewPort::getXMin()
{
	return x_min;
}

int ViewPort::getXMax()
{
	return x_max;
}

int ViewPort::getYMin()
{
	return y_min;
}

int ViewPort::getYMax()
{
	return y_max;
}
