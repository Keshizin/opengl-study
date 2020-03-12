#ifndef GAME_ENGINE_WINDOW_API_WRAPPER_H
#define GAME_ENGINE_WINDOW_API_WRAPPER_H

#include <windows.h>
#include <string>

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

class GEApiWrapper
{
public:
	GEApiWrapper();

	int initWindow();
	int createWindow(int width, int height, std::string);
	int destroyWindow();
	int showWindow();

	HDC getHDC() { return hDC; }

private:
	std::string windowClassName;
	HWND hWindow;
	HDC hDC;
	HGLRC hRC;
};

#endif