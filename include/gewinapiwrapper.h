#ifndef GAME_ENGINE_WINDOW_API_WRAPPER_H
#define GAME_ENGINE_WINDOW_API_WRAPPER_H

#include <GEApiWrapper.h>
#include <windows.h>
#include <string>

class GEWinApiWrapper : public GEApiWrapper
{
public:
	GEWinApiWrapper();

	// Redifined methods from GEApiWrapper
	int initializeWindow();
	int initializeRenderingSystem();
	int createWindow(int width, int height, std::string name);
	int destroyWindow();
	int showWindow();

	// Getters and Setters
	void setWindowClassName(std::string windowClassName);
	std::string getWindowClassName();

	HDC getDC();

private:
	static LRESULT CALLBACK windowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	std::string windowClassName;
	HWND hWindow;
	HDC hDC;
	HGLRC hRC;
};

#endif