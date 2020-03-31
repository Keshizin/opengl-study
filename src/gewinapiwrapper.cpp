#include <gewinapiwrapper.h>
#include <iostream>
#include <GL/gl.h>

GEWinApiWrapper::GEWinApiWrapper()
{
}

int GEWinApiWrapper::initializeWindow()
{
	WNDCLASSEX windowClass;
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_DBLCLKS | CS_HREDRAW | CS_OWNDC | CS_VREDRAW;
	windowClass.lpfnWndProc = windowProcedure; // obrigatório
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	//windowClass.hInstance = hInstance; // obrigatório
	windowClass.hInstance = GetModuleHandle(NULL); // obrigatório
	//windowClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYICON)); 
	windowClass.hIcon = 0;
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.hbrBackground = 0;
	windowClass.lpszMenuName = 0;
	windowClass.lpszClassName = LPCSTR(windowClassName.c_str()); // obrigatório
	windowClass.hIconSm = 0;

	if(!RegisterClassEx(&windowClass))
	{
		DWORD error = GetLastError();
		std::cout << "(!) Nao foi possivel registrar uma Window Class: " << error << "\n" << std::endl;
		return 0;
	}

	return 1;
}

int GEWinApiWrapper::initializeRenderingSystem()
{
	BOOL ret;

	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR), // nSize
		1, // nVersion
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, // dwFlags
		PFD_TYPE_RGBA, // iPixelType
		32, // cColorBits
		0, // cRedBits
		0, // cRedShift
		0, // cGreenBits
		0, // cGreenShift
		0, // cBlueBits
		0, // cBlueShift
		0, // cAlphaBits
		0, // cAlphaShift
		0, // cAccumBits
		0, // cAccumRedBits
		0, // cAccumGreenBits
		0, // cAccumBlueBits
		0, // cAccumAlphaBits
		32, // cDepthBits
		0, // cStencilBits
		0, // cAuxBuffers
		PFD_MAIN_PLANE, // iLayerType
		0, // bReserved
		0, // dwLayerMask
		0, // dwVisibleMask
		0 // dwDamageMask
	};

	hDC = GetDC(hWindow);

	if(hDC == NULL)
		return 0;

	GLuint PixelFormat = ChoosePixelFormat(hDC, &pfd);

	if(!PixelFormat)
	{
		// GetLastError - checar motivo do erro
		int r = ReleaseDC(hWindow, hDC);

		if(!r)
			std::cout << "(!) Nao foi possivel liberar DC.\n" << std::endl;

		ret = DestroyWindow(hWindow);

		if(ret == 0)
		{
			// GetLastError - checar motivo do erro
			std::cout << "(!) Nao foi possivel destruir a janela.\n" << std::endl;
		}

		ret = UnregisterClass(LPCSTR(windowClassName.c_str()), GetModuleHandle(NULL));

		if(ret == 0)
		{
			// GetLastError - checar motivo do erro
			std::cout << "(!) Nao foi possivel desregistrar a janela.\n" << std::endl;
		}

		return 0;
	}

	ret = SetPixelFormat(hDC, PixelFormat, &pfd);

	if(ret == FALSE)
	{
		// GetLastError - checar motivo do erro
		int r = ReleaseDC(hWindow, hDC);

		if(!r)
			std::cout << "(!) Nao foi possivel liberar DC.\n" << std::endl;

		ret = DestroyWindow(hWindow);

		if(ret == 0)
		{
			// GetLastError - checar motivo do erro
			std::cout << "(!) Nao foi possivel destruir a janela.\n" << std::endl;
		}

		ret = UnregisterClass(LPCSTR(windowClassName.c_str()), GetModuleHandle(NULL));

		if(ret == 0)
		{
			// GetLastError - checar motivo do erro
			std::cout << "(!) Nao foi possivel desregistrar a janela.\n" << std::endl;
		}

		return 0;
	}

	hRC = wglCreateContext(hDC);

	if(hRC == NULL)
	{
		// GetLastError - checar motivo do erro
		int ret = ReleaseDC(hWindow, hDC);

		if(!ret)
			std::cout << "(!) Nao foi possivel liberar DC.\n" << std::endl;
	}

	ret = wglMakeCurrent(hDC, hRC);

	if(ret == FALSE)
	{
		// GetLastError - checar motivo do erro
		ret = wglDeleteContext(hRC);

		if(ret == FALSE)
		{
			// GetLastError - checar motivo do erro
			std::cout << "(!) Nao foi possivel deletar Rendering Context.\n" << std::endl;
		}

		int r = ReleaseDC(hWindow, hDC);

		if(!r)
			std::cout << "(!) Nao foi possivel liberar DC.\n" << std::endl;

		ret = DestroyWindow(hWindow);

		if(ret == 0)
		{
			// GetLastError - checar motivo do erro
			std::cout << "(!) Nao foi possivel destruir a janela.\n" << std::endl;
		}

		ret = UnregisterClass(LPCSTR(windowClassName.c_str()), GetModuleHandle(NULL));

		if(ret == 0)
		{
			// GetLastError - checar motivo do erro
			std::cout << "(!) Nao foi possivel desregistrar a janela.\n" << std::endl;
		}

		return 0;
	}

	return 1;
}

int GEWinApiWrapper::createWindow(int width, int height, std::string name)
{
	RECT windowSize;
	windowSize.left = (LONG)0;
	windowSize.right = (LONG)width;
	windowSize.top = (LONG)0;
	windowSize.bottom = (LONG)height;

	// Janela default (janela com barra de título)
	// DWORD dwExStyle = 0;
	// DWORD dwStyle = 0;

	// Janela sem bordas (sem barra de títulos)
	// DWORD dwExStyle = WS_EX_APPWINDOW;
	// DWORD dwStyle = WS_POPUP;

	// Janela sem bordas (somente com a barra de títulos)
	// DWORD dwExStyle = WS_EX_APPWINDOW;
	// DWORD dwStyle = WS_POPUP | WS_CAPTION;

	// Janela com várias opções do sistema
	// DWORD dwExStyle = WS_EX_APPWINDOW;
	// DWORD dwStyle = WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU;
	// DWORD dwStyle = WS_MINIMIZEBOX | WS_SYSMENU;
	// DWORD dwStyle = WS_SYSMENU;

	// Janela redimensionável (somente com a barra de títulos)
	// DWORD dwExStyle = WS_EX_APPWINDOW;
	// DWORD dwStyle = WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME;

	// Janela não redimensionável (somente com a barra de títulos)
	// DWORD dwExStyle = WS_EX_APPWINDOW;
	// DWORD dwStyle = WS_POPUP | WS_CAPTION | WS_SYSMENU;

	// Janela com todas as opções
	DWORD dwExStyle = WS_EX_APPWINDOW;
	DWORD dwStyle = WS_OVERLAPPEDWINDOW;

	AdjustWindowRectEx(&windowSize, dwStyle, FALSE, dwExStyle);

	hWindow = CreateWindowEx(
		dwExStyle,
		LPCSTR(windowClassName.c_str()),
		LPCSTR(name.c_str()),
		dwStyle,
		//CW_USEDEFAULT,
		//CW_USEDEFAULT,
		100,
		100,
		windowSize.right - windowSize.left,
		windowSize.bottom - windowSize.top,
		NULL,
		NULL,
		GetModuleHandle(NULL),
		NULL);

	return 1;
}

int GEWinApiWrapper::destroyWindow()
{
	BOOL ret = wglMakeCurrent(NULL, NULL);

	if(ret == FALSE)
	{
		// GetLastError - checar motivo do erro
		std::cout << "(!) Nao foi possivel liberar Rendering Context.\n" << std::endl;
	}

	ret = wglDeleteContext(hRC);

	if(ret == FALSE)
	{
		// GetLastError - checar motivo do erro
		std::cout << "(!) Nao foi possivel deletar Rendering Context.\n" << std::endl;
	}

	int r = ReleaseDC(hWindow, hDC);

	if(!r)
		std::cout << "(!) Nao foi possivel liberar DC.\n" << std::endl;

	ret = DestroyWindow(hWindow);

	if(ret == 0)
	{
		// GetLastError - checar motivo do erro
		std::cout << "(!) Nao foi possivel destruir a janela.\n" << std::endl;
	}

	ret = UnregisterClass(LPCSTR(windowClassName.c_str()), GetModuleHandle(NULL));

	if(ret == 0)
	{
		// GetLastError - checar motivo do erro
		std::cout << "(!) Nao foi possivel desregistrar a janela.\n" << std::endl;
	}

	return 1;
}

int GEWinApiWrapper::showWindow()
{
	if(hWindow)
	{
		ShowWindow(hWindow, SW_SHOW);
		UpdateWindow(hWindow);
	}

	return 1;
}

void GEWinApiWrapper::setWindowClassName(std::string windowClassName)
{
	this->windowClassName = windowClassName;
}

std::string GEWinApiWrapper::getWindowClassName()
{
	return windowClassName;
}

HDC GEWinApiWrapper::getDC()
{
	return hDC;
}

LRESULT CALLBACK GEWinApiWrapper::windowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		// ----------------------------------------------------------------------
		//    TRATAMENTO DA JANELA WINDOW
		// ----------------------------------------------------------------------
		case WM_CREATE:
			// std::cout << ">   WM_CREATE: " << uMsg << std::endl;
			break;

		case WM_DESTROY:
			// std::cout << ">   WM_DESTROY: " << uMsg << std::endl;
			PostQuitMessage(0); // send a quit message
			break;

		case WM_MOVE:
			// std::cout
			// 	<< ">   WM_MOVE: " << uMsg
			// 	<< "\n    X: " << LOWORD(lParam)
			// 	<< "\n    Y: " << HIWORD(lParam)
			// 	<< std::endl;

			break;

		case WM_SIZE:
			eventHandler->resizeWindow(LOWORD(lParam), HIWORD(lParam));
			break;

		case WM_CLOSE:
			//std::cout << ">   WM_CLOSE: " << uMsg << std::endl;
			break;

		case WM_QUIT:
			// PostQuitMessage
			// std::cout
			// << ">   WM_QUIT: " << uMsg
			// << "\n    Exit code: " << wParam
			// << std::endl;

			break;

		// ----------------------------------------------------------------------
		//    TRATAMENTO DO MOUSE
		//       - WM_MOUSEFIRST *filter
		//       - WM_MOUSELAST *filter
		// ----------------------------------------------------------------------
		case WM_LBUTTONDOWN:
			eventHandler->mouseEvent(1, 1, LOWORD(lParam), HIWORD(lParam));
			break;

		case WM_LBUTTONUP:
			eventHandler->mouseEvent(1, 0, LOWORD(lParam), HIWORD(lParam));
			break;

		case WM_LBUTTONDBLCLK:
			break;

		case WM_NCLBUTTONDOWN:
			break;

		case WM_NCLBUTTONUP:
			break;

		case WM_NCLBUTTONDBLCLK:
			break;

		case WM_MBUTTONDOWN:
			eventHandler->mouseEvent(2, 1, LOWORD(lParam), HIWORD(lParam));
			break;

		case WM_MBUTTONUP:
			eventHandler->mouseEvent(2, 0, LOWORD(lParam), HIWORD(lParam));
			break;

		case WM_MBUTTONDBLCLK:
			break;
		
		case WM_NCMBUTTONDOWN:
			break;

		case WM_NCMBUTTONUP:
			break;

		case WM_NCMBUTTONDBLCLK:
			break;

		case WM_RBUTTONDOWN:
			eventHandler->mouseEvent(3, 0, LOWORD(lParam), HIWORD(lParam));
			break;

		case WM_RBUTTONUP:
			eventHandler->mouseEvent(3, 0, LOWORD(lParam), HIWORD(lParam));
			break;

		case WM_RBUTTONDBLCLK:
			break;

		case WM_NCRBUTTONDOWN:
			break;

		case WM_NCRBUTTONUP:
			break;

		case WM_NCRBUTTONDBLCLK:
			break;

		case WM_XBUTTONDOWN:
			break;

		case WM_XBUTTONUP:
			break;

		case WM_XBUTTONDBLCLK:
			break;

		case WM_NCXBUTTONDOWN:
			break;

		case WM_NCXBUTTONUP:
			break;

		case WM_NCXBUTTONDBLCLK:
			break;

		// ----------------------------------------------------------------------

		case WM_MOUSEMOVE:
			eventHandler->mouseMotionEvent(LOWORD(lParam), HIWORD(lParam));
			break;

		case WM_NCMOUSEMOVE:
			// std::cout
			// 	<< ">   WM_NCMOUSEMOVE: " << uMsg
			// 	<< "\n    Virtual Key: " << wParam
			// 	<< "\n    (mouse) x: " << LOWORD(lParam)
			// 	<< "\n    (mouse) y: " << HIWORD(lParam)
			// 	<< std::endl;

			break;

		case WM_MOUSEHOVER:
			// TrackMouseEvent()
			// Posts messages when the mouse pointer leaves a window or hovers
			// over a window for a specified amount of time.
			// 	TRACKMOUSEEVENT tme;
			//		tme.cbSize = sizeof(tme);
			//		tme.hwndTrack = hwnd;
			//		tme.dwFlags = TME_HOVER | TME_LEAVE;
			//		tme.dwHoverTime = HOVER_DEFAULT;
			//		TrackMouseEvent(&tme);

			// std::cout
			// 	<< ">   WM_MOUSEHOVER: " << uMsg
			// 	<< "\n    Virtual Key: " << wParam
			// 	<< "\n    (mouse) x: " << LOWORD(lParam)
			// 	<< "\n    (mouse) y: " << HIWORD(lParam)
			// 	<< std::endl;

			break;

		case WM_NCMOUSEHOVER:
			// std::cout
			// 	<< ">   WM_NCMOUSEHOVER: " << uMsg
			// 	<< "\n    Virtual Key: " << wParam
			// 	<< "\n    (mouse) x: " << LOWORD(lParam)
			// 	<< "\n    (mouse) y: " << HIWORD(lParam)
			// 	<< std::endl;

			break;

		case WM_MOUSELEAVE:
			// std::cout
			// 	<< ">   WM_MOUSELEAVE: " << uMsg
			// 	<< "\n    Virtual Key: " << wParam
			// 	<< "\n    (mouse) x: " << LOWORD(lParam)
			// 	<< "\n    (mouse) y: " << HIWORD(lParam)
			// 	<< std::endl;

			break;

		case WM_NCMOUSELEAVE:
			// std::cout
			// 	<< ">   WM_NCMOUSELEAVE: " << uMsg
			// 	<< "\n    Virtual Key: " << wParam
			// 	<< "\n    (mouse) x: " << LOWORD(lParam)
			// 	<< "\n    (mouse) y: " << HIWORD(lParam)
			// 	<< std::endl;

			break;

		case WM_MOUSEWHEEL:
			// std::cout
			// 	<< ">   WM_MOUSEWHEEL: " << uMsg
			// 	<< "\n    Virtual Key: " << wParam
			// 	<< "\n    (mouse) x: " << LOWORD(lParam)
			// 	<< "\n    (mouse) y: " << HIWORD(lParam)
			// 	<< std::endl;

			break;

		// ----------------------------------------------------------------------

		case WM_MOUSEACTIVATE:
			// std::cout
			// 	<< ">   WM_MOUSEACTIVATE: " << uMsg
			// 	<< "\n    hit test: " << LOWORD(lParam)
			// 	<< "\n    id: " << HIWORD(lParam)
			// 	<< std::endl;

			break;

		// ----------------------------------------------------------------------
		//    TRATAMENTO DO TECLADO
		// ----------------------------------------------------------------------
		case WM_SYSKEYDOWN:
			// std::cout
			// 	<< ">   WM_SYSKEYDOWN: " << uMsg
			// 	<< "\n    key code: " << wParam
			// 	<< "\n    binary: " << lParam
			// 	<< std::endl;

			break;

		case WM_KEYDOWN:
			// std::cout
			// 	<< ">   WM_KEYDOWN: " << uMsg
			// 	<< "\n    key code: " << wParam
			// 	<< "\n    binary: " << lParam
			// 	<< std::endl;

			break;

		case WM_SYSKEYUP:
			// std::cout
			// 	<< ">   WM_SYSKEYUP: " << uMsg
			// 	<< "\n    key code: " << wParam
			// 	<< "\n    binary: " << lParam
			// 	<< std::endl;

			break;

		case WM_KEYUP:
			// std::cout
			// 	<< ">   WM_KEYUP: " << uMsg
			// 	<< "\n    key code: " << wParam
			// 	<< "\n    binary: " << lParam
			// 	<< std::endl;

			break;

		case WM_CHAR:
			// std::cout
			// 	<< ">   WM_CHAR: " << uMsg
			// 	<< "\n    key code: " << wParam
			// 	<< "\n    binary: " << lParam
			// 	<< std::endl;

			break;

		case WM_SYSCHAR:
			// std::cout
			// 	<< ">   WM_SYSCHAR: " << uMsg
			// 	<< "\n    key code: " << wParam
			// 	<< "\n    binary: " << lParam
			// 	<< std::endl;

			break;

		case WM_DEADCHAR:
			// std::cout
			// 	<< ">   WM_DEADCHAR: " << uMsg
			// 	<< "\n    key code: " << wParam
			// 	<< "\n    binary: " << lParam
			// 	<< std::endl;

			break;

		case WM_IME_CHAR:
			// std::cout
			// 	<< ">   WM_IME_CHAR: " << uMsg
			// 	<< "\n    key code: " << wParam
			// 	<< "\n    binary: " << lParam
			// 	<< std::endl;

			break;

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

		// case WM_PAINT:
		// {
		// PAINTSTRUCT ps;
		// HDC hdc = BeginPaint(hwnd, &ps);
		// FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));
		// EndPaint(hwnd, &ps);
		// }

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

		default:
			break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}