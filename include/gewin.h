#ifndef GAME_ENGINE_WINDOW_CLASS_H
#define GAME_ENGINE_WINDOW_CLASS_H

#include <string>
#include <geapiwrapper.h>

class GEWindow
{
public:
	GEWindow(GEApiWrapper *apiWrapper);

	int createWindow();
	int showWindow();
	int destroyWindow();

	// Getters and Setters
	void setWindowName(std::string windowName);
	void setWidth(int width);
	void setHeight(int height);
	void setWindowStyle(int windowStyle);
	void setXPosition(int xPosition);
	void setTPosition(int yPosition);

	std::string getWindowName();
	int getWidth();
	int getHeight();
	int getWindowStyle();
	int getXPosition();
	int getTPosition();

private:
	GEApiWrapper *apiWrapper;
	std::string windowName;
	int width;
	int height;
	int windowStyle;
	int xPosition;
	int yPosition;
};

#endif
