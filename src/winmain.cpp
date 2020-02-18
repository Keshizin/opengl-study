#include <windows.h>
#include <iostream>

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	const char myCustomClassName[] = "MY-CUSTOM-WINDOW-CLASS";
	const char myCustomWindowName[] = "MY-CUSTOM-WINDOW-NAME";

	WNDCLASSEX myWindowClass;

	myWindowClass.cbSize = sizeof(WNDCLASSEX);
	myWindowClass.style = 0;
	myWindowClass.lpfnWndProc = WindowProcedure; // obrigatório
	myWindowClass.cbClsExtra = 0;
	myWindowClass.cbWndExtra = 0;
	myWindowClass.hInstance = hInstance;// obrigatório
	myWindowClass.hIcon = 0;
	myWindowClass.hCursor = 0;
	myWindowClass.hbrBackground = 0;
	myWindowClass.lpszMenuName = 0;
	myWindowClass.lpszClassName = myCustomClassName; // obrigatório
	myWindowClass.hIconSm = 0;

	RegisterClassEx(&myWindowClass);

	DWORD dwExStyle = 0;
	DWORD dwStyle = 0;

	HWND hMyCustomWindow = CreateWindowEx(
		dwExStyle,
		myCustomClassName,
		myCustomWindowName,
		dwStyle,
		0, // CW_USEDEFAULT
		0, // CW_USEDEFAULT
		640,
		480,
		NULL,
		NULL,
		hInstance,
		NULL);

	if(hMyCustomWindow)
		ShowWindow(hMyCustomWindow, nCmdShow);

	return 0;
}

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	std::cout << "> WindowProcedure call..."
		<< "\nMSG: " << uMsg
		<< "\nWPARAM: " << wParam
		<< "\nLPARAM: " << lParam
		<< std::endl;

	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}