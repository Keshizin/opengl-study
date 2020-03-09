#include <gewin.h>
#include <geapiwrapper.h>

GEWindow::GEWindow(GEApiWrapper *apiWrapper)
{
	this->apiWrapper = apiWrapper;
}

int GEWindow::createWindow()
{
	if(!apiWrapper->initWindow())
		return 0;

	if(!apiWrapper->createWindow(getWidth(), getHeight(), getWindowName()))
		return 0;

	apiWrapper->showWindow();

	return 1;
}

int GEWindow::destroyWindow()
{
	apiWrapper->destroyWindow();
}

void GEWindow::setWindowName(std::string windowName)
{
	this->windowName = windowName;
}

void GEWindow::setWidth(int width)
{
	this->width = width;
}

void GEWindow::setHeight(int height)
{
	this->height = height;
}

void GEWindow::setWindowStyle(int windowStyle)
{
	this->windowStyle = windowStyle;
}

void GEWindow::setXPosition(int xPosition)
{
	this->xPosition = xPosition;
}

void GEWindow::setTPosition(int yPosition)
{
	this->yPosition = yPosition;
}

std::string GEWindow::getWindowName()
{
	return windowName;
}

int GEWindow::getWidth()
{
	return width;
}

int GEWindow::getHeight()
{
	return height;
}

int GEWindow::getWindowStyle()
{
	return windowStyle;
}

int GEWindow::getXPosition()
{
	return xPosition;
}

int GEWindow::getTPosition()
{
	return yPosition;
}
