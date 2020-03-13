#include <gemouse.h>

GEMouse::GEMouse()
{
}

void GEMouse::setXPosition(int xPosition)
{
	this->xPosition = xPosition;
}

void GEMouse::setYPosition(int yPosition)
{
	this->yPosition = yPosition;
}

void GEMouse::setState(int state)
{
	this->state = state;
}

void GEMouse::setButton(int button)
{
	this->button = button;
}

int GEMouse::getXPosition()
{
	return xPosition;
}

int GEMouse::getYPosition()
{
	return yPosition;
}

int GEMouse::getState()
{
	return state;
}

int GEMouse::getButton()
{
	return button;
}
