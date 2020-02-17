#include <windows.h>
#include <iostream>

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	const char myCustomClassName[] = "MY-CUSTOM-WINDOW-CLASS";
	const char myCustomWindowName[] = "MY-CUSTOM-WINDOW-NAME";

	unsigned int counter = 0;
	MSG msg;
	int ret;
	bool isDone = false;

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
	{
		ShowWindow(hMyCustomWindow, nCmdShow);
		UpdateWindow(hMyCustomWindow);
	}

	while(!isDone)
	{
		ret = PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
		//ret = GetMessage(&msg, NULL, 0, 0);

		std::cout << "> PeekMessage (" << counter << ") call..."
			<< "\n   Ret: " << ret
			<< "\n   Msg: " << msg.message
			<< std::endl;

		if(msg.message == WM_QUIT)
		{
			DestroyWindow(hMyCustomWindow);
			UnregisterClass(myCustomClassName, hInstance);
		}

		if(ret)
		{
			std::cout << ">    tratamento de mensagens..." << std::endl;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// loop principal
			std::cout << ">    loop principal..." << std::endl;
		}

		counter++;
	}

	return 0;
}

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	std::cout << "> WindowProcedure call..."
		<< "\n   MSG: " << uMsg
		<< "\n   WPARAM: " << wParam
		<< "\n   LPARAM: " << lParam
		<< std::endl;

	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}