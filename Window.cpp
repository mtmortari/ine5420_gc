#include "Window.h"


void Window::setXMin(int x_min)
{
	this->x_min = x_min;
}

void Window::setYMin(int y_min)
{
	this->y_min = y_min;
}

void Window::setXMax(int x_max)
{
	this->x_max = x_max;
}

void Window::setYMax(int y_max)
{
	this->y_max = y_max;
}

int Window::getXMin()
{
	return x_min;
}

int Window::getXMax()
{
	return x_max;
}

int Window::getYMin()
{
	return y_min;
}

int Window::getYMax()
{
	return y_max;
}
