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
	// CS_BYTEALIGNCLIENT (?)
	// CS_BYTEALIGNWINDOW (?)
	// CS_CLASSDC
	// CS_DBLCLKS
	// CS_GLOBALCLASS
	// CS_HREDRAW
	// CS_NOCLOSE
	// CS_OWNDC
	// CS_PARENTDC
	// CS_SAVEBITS
	// CS_VREDRAW

	myWindowClass.style =
		CS_DBLCLKS |
		CS_HREDRAW |
		CS_OWNDC |
		CS_VREDRAW;

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

	// WS_EX_ACCEPTFILES - The window accepts drag-drop files.
	// WS_EX_APPWINDOW - Forces a top-level window onto the taskbar when the window is visible.
	// WS_EX_CLIENTEDGE - The window has a border with a sunken edge.
	// WS_EX_COMPOSITED
	// WS_EX_CONTEXTHELP
	// WS_EX_CONTROLPARENT
	// WS_EX_DLGMODALFRAME
	// WS_EX_LAYERED
	// WS_EX_LAYOUTRTL
	// WS_EX_LEFT
	// WS_EX_LEFTSCROLLBAR
	// WS_EX_LTRREADING
	// WS_EX_MDICHILD
	// WS_EX_NOACTIVATE
	// WS_EX_NOINHERITLAYOUT
	// WS_EX_NOPARENTNOTIFY
	// WS_EX_NOREDIRECTIONBITMAP
	// WS_EX_OVERLAPPEDWINDOW
	// WS_EX_PALETTEWINDOW
	// WS_EX_RIGHT
	// WS_EX_RIGHTSCROLLBAR
	// WS_EX_RTLREADING
	// WS_EX_STATICEDGE
	// WS_EX_TOOLWINDOW
	// WS_EX_TOPMOST
	// WS_EX_TRANSPARENT
	// WS_EX_WINDOWEDGE

	DWORD dwStyle = WS_OVERLAPPEDWINDOW;

	// WS_BORDER
	// WS_CAPTION
	// WS_CHILD
	// WS_CHILDWINDOW
	// WS_CLIPCHILDREN
	// WS_CLIPSIBLINGS
	// WS_DISABLED
	// WS_DLGFRAME
	// WS_GROUP
	// WS_HSCROLL
	// WS_ICONIC
	// WS_MAXIMIZE
	// WS_MAXIMIZEBOX
	// WS_MINIMIZE
	// WS_MINIMIZEBOX
	// WS_OVERLAPPED
	// WS_OVERLAPPEDWINDOW
	// WS_POPUP
	// WS_POPUPWINDOW
	// WS_SIZEBOX
	// WS_SYSMENU
	// WS_TABSTOP
	// WS_THICKFRAME
	// WS_TILED
	// WS_TILEDWINDOW
	// WS_VISIBLE
	// WS_VSCROLL

	HWND hMyCustomWindow = CreateWindowEx(
		dwExStyle,
		myCustomClassName,
		myCustomWindowName,
		dwStyle,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		640,
		480,
		NULL,
		NULL,
		hInstance,
		NULL);

	if(hMyCustomWindow)
	{
		ShowWindow(hMyCustomWindow, nCmdShow);
		//UpdateWindow(hMyCustomWindow);
	}

	while(!isDone)
	{
		ret = PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
		// ret = GetMessage(&msg, NULL, 0, 0); // Testar novamente quando o contador de FPS estiver pronto

		// std::cout << "> GetMessage (" << counter << ") call..."
		// 	<< "\n   Ret: " << ret
		// 	<< "\n   Msg: " << msg.message
		// 	<< std::endl;

		if(ret)
		{
			// std::cout << "> PeekMessage (" << counter << ") call..."
			// 	<< "\n   Ret: " << ret
			// 	<< "\n   Msg: " << msg.message
			// 	<< std::endl;

			if(msg.message == WM_QUIT)
			{
				DestroyWindow(hMyCustomWindow);
				UnregisterClass(myCustomClassName, hInstance);
				return 0;
			}

			// std::cout << ">    tratamento de mensagens..." << std::endl;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// loop principal
			// std::cout << ">    loop principal..." << std::endl;
		}

		counter++;
	}

	std::cout << ">   END OF APPLICATION!" << std::endl;
	return 0;
}

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	std::cout << "> WindowProcedure call..."
		<< "\n   MSG: " << uMsg
		<< "\n   WPARAM: " << wParam
		<< "\n   LPARAM: " << lParam
		<< std::endl;

	switch(uMsg)
	{
		// case WM_NULL: // no operation
		// 	std::cout << ">   WM_NULL: " << uMsg << std::endl; 
		// 	break;

		// case WM_CREATE:
		// 	std::cout << ">   WM_CREATE: " << uMsg << std::endl;
		// 	break;

		// case WM_DESTROY:
		// 	std::cout << ">   WM_DESTROY: " << uMsg << std::endl;
		// 	PostQuitMessage(0); // send a quit message
		// 	break;

		// case WM_MOVE:
		// 	std::cout
		// 		<< ">   WM_MOVE: " << uMsg
		// 		<< "\n    X: " << LOWORD(lParam)
		// 		<< "\n    Y: " << HIWORD(lParam)
		// 		<< std::endl;

		// 	break;

		// case WM_SIZE:
		// 	std::cout
		// 		<< ">   WM_SIZE: " << uMsg
		// 		<< "\n    TYPE: " << wParam
		// 		<< "\n    WIDTH: " << LOWORD(lParam) // client area
		// 		<< "\n    HEIGHT: " << HIWORD(lParam) // client area
		// 		<< std::endl;

		// 	break;

		// case WM_ACTIVATE:
		// 	std::cout
		// 		<< ">   WM_ACTIVATE: " << uMsg
		// 		<< "\n    STATE: " << LOWORD(wParam)
		// 		<< "\n    IS MINIMIZED: " << HIWORD(wParam)
		// 		<< std::endl;

		// 	break;

		// case WM_SETFOCUS: // keyboard focus
		// 	std::cout << ">    WM_SETFOCUS: " << uMsg << std::endl;
		// 	break;

		// case WM_KILLFOCUS: // keyboard focus
		// 	std::cout << ">    WM_KILLFOCUS: " << uMsg << std::endl;
		// 	break;

		// case WM_ENABLE: // (APP) relacionado ao teclado e mouse
		// 	// BOOL EnableWindow(HWND hWnd, BOOL bEnable);
		// 	std::cout
		// 		<< ">   WM_ENABLE: " << uMsg
		// 		<< "\n    STATE: " << wParam
		// 		<< std::endl;

		// 	break;

		// case WM_SETREDRAW: // (APP)
		// 	// SendMessage((HWND) hWnd, WM_SETREDRAW, (WPARAM) wParam, (LPARAM) lParam);
		// 	std::cout
		// 		<< ">   WM_SETREDRAW: " << uMsg
		// 		<< "\n    STATE: " << wParam
		// 		<< std::endl;

		// 	break;

		// case WM_SETTEXT:
		// 	std::cout
		// 		<< ">   WM_SETTEXT: " << uMsg
		// 		// << "\n    TEXT: " << lParam
		// 		<< std::endl;

		// 	break;

		// case WM_GETTEXT:
		// 	std::cout
		// 		<< ">   WM_GETTEXT: " << uMsg
		// 		<< "\n    MAX: " << wParam
		// 		// << "\n    TEXT: " << lParam
		// 		<< std::endl;

		// 	break;

		// case WM_PAINT: // sys or another app request a paint
		// 	// UpdateWindow, RedrawWindow, DispatchMessage
		// 	std::cout << ">   WM_PAINT: " << uMsg << std::endl;
		// 	break;

		// case WM_SYSCOMMAND:
		// 	std::cout
		// 		<< ">   WM_SYSCOMMAND: " << uMsg
		// 		<< "\n    TYPE: " << wParam
		// 		// SC_CLOSE
		// 		// SC_CONTEXTHELP
		// 		// SC_DEFAULT
		// 		// SC_HOTKEY
		// 		// SC_HSCROLL
		// 		// SCF_ISSECURE
		// 		// SC_KEYMENU
		// 		// SC_MAXIMIZE
		// 		// SC_MINIMIZE
		// 		// SC_MONITORPOWER
		// 		// SC_MOUSEMENU
		// 		// SC_MOVE
		// 		// SC_NEXTWINDOW
		// 		// SC_PREVWINDOW
		// 		// SC_RESTORE
		// 		// SC_SCREENSAVE
		// 		// SC_SIZE
		// 		// SC_TASKLIST
		// 		// SC_VSCROLL
		// 		<< "\n    (mouse) x: " << LOWORD(lParam)
		// 		<< "\n    (mouse) y: " << HIWORD(lParam)
		// 		<< std::endl;
		// 	break;

		// case WM_CLOSE:
		// 	std::cout << ">   WM_CLOSE: " << uMsg << std::endl;
		// 	break;

		// case WM_KEYDOWN: // nonsystem key | A nonsystem key is a key that is pressed when the ALT key is not pressed.
		// 	std::cout
		// 		<< ">   WM_KEYDOWN: " << uMsg
		// 		<< "\n    Virtual code: " << wParam
		// 		<< "\n    Code bit set: " << lParam
		// 		<< std::endl;
		// 	break;

		// case WM_KEYUP:
		// 	std::cout
		// 		<< ">   WM_KEYUP: " << uMsg
		// 		<< "\n    Virtual code: " << wParam
		// 		<< "\n    Code bit set: " << lParam
		// 		<< std::endl;

		// 	break;

		// case WM_QUIT:
		// 	// PostQuitMessage
		// 	std::cout
		// 		<< ">   WM_QUIT: " << uMsg
		// 		<< "\n    Exit code: " << wParam
		// 		<< std::endl;

		// 	break;

		default:
			break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}