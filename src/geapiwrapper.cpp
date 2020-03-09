#include <geapiwrapper.h>

GEApiWrapper::GEApiWrapper()
{
	windowClassName = "GEWINDOWCLASS";
}

int GEApiWrapper::initWindow()
{
	WNDCLASSEX windowClass;

	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_DBLCLKS | CS_HREDRAW | CS_OWNDC | CS_VREDRAW;
	windowClass.lpfnWndProc = WindowProcedure; // obrigatório
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
		// ERRO
		return 0;
	}
}

int GEApiWrapper::createWindow(int width, int height, std::string windowName)
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
	DWORD dwExStyle = WS_EX_APPWINDOW;
	DWORD dwStyle = WS_POPUP | WS_CAPTION | WS_SYSMENU;

	// Janela com todas as opções
	// DWORD dwExStyle = WS_EX_APPWINDOW;
	// DWORD dwStyle = WS_OVERLAPPEDWINDOW;

	AdjustWindowRectEx(&windowSize, dwStyle, FALSE, dwExStyle);

	hWindow = CreateWindowEx(
		dwExStyle,
		LPCSTR(windowClassName.c_str()),
		LPCSTR(windowName.c_str()),
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

	// static PIXELFORMATDESCRIPTOR pfd =
	// {
	// 	sizeof(PIXELFORMATDESCRIPTOR), // nSize
	// 	1, // nVersion
	// 	PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, // dwFlags
	// 	PFD_TYPE_RGBA, // iPixelType
	// 	32, // cColorBits
	// 	0, // cRedBits
	// 	0, // cRedShift
	// 	0, // cGreenBits
	// 	0, // cGreenShift
	// 	0, // cBlueBits
	// 	0, // cBlueShift
	// 	0, // cAlphaBits
	// 	0, // cAlphaShift
	// 	0, // cAccumBits
	// 	0, // cAccumRedBits
	// 	0, // cAccumGreenBits
	// 	0, // cAccumBlueBits
	// 	0, // cAccumAlphaBits
	// 	32, // cDepthBits
	// 	0, // cStencilBits
	// 	0, // cAuxBuffers
	// 	PFD_MAIN_PLANE, // iLayerType
	// 	0, // bReserved
	// 	0, // dwLayerMask
	// 	0, // dwVisibleMask
	// 	0 // dwDamageMask
	// };

	// hDC = GetDC(hWindow);
	// GLuint PixelFormat = ChoosePixelFormat(hDC, &pfd);
	// SetPixelFormat(hDC, PixelFormat, &pfd);
	// hRC = wglCreateContext(hDC);
	// wglMakeCurrent(hDC, hRC);
}

int GEApiWrapper::destroyWindow()
{
	// wglMakeCurrent(NULL, NULL);
	// wglDeleteContext(hRC);
	// ReleaseDC(hMyCustomWindow, hDC);
	DestroyWindow(hWindow);
	UnregisterClass(LPCSTR(windowClassName.c_str()), GetModuleHandle(NULL));
}

int GEApiWrapper::showWindow()
{
	if(hWindow)
	{
		ShowWindow(hWindow, SW_SHOW);
		UpdateWindow(hWindow);
	}
}

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// std::cout << "> WindowProcedure call..."
	// 	<< "\n   MSG: " << uMsg
	// 	<< "\n   WPARAM: " << wParam
	// 	<< "\n   LPARAM: " << lParam
	// 	<< std::endl;

	switch(uMsg)
	{
		// case WM_NULL: // no operation
		// 	std::cout << ">   WM_NULL: " << uMsg << std::endl; 
		// 	break;

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
			// std::cout
			// 	<< ">   WM_SIZE: " << uMsg
			// 	<< "\n    TYPE: " << wParam
			// 	<< "\n    WIDTH: " << LOWORD(lParam) // client area
			// 	<< "\n    HEIGHT: " << HIWORD(lParam) // client area
			// 	<< std::endl;

			//resizeGLWindow(LOWORD(lParam), HIWORD(lParam));
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
			// std::cout
			// 	<< ">   WM_LBUTTONDOWN: " << uMsg
			// 	<< "\n    Virtual Key: " << wParam
			// 	<< "\n    (mouse) x: " << LOWORD(lParam)
			// 	<< "\n    (mouse) y: " << HIWORD(lParam)
			// 	<< std::endl;

			break;

		case WM_LBUTTONUP:
			// std::cout
			// 	<< ">   WM_LBUTTONUP: " << uMsg
			// 	<< "\n    Virtual Key: " << wParam
			// 	<< "\n    (mouse) x: " << LOWORD(lParam)
			// 	<< "\n    (mouse) y: " << HIWORD(lParam)
			// 	<< std::endl;

			break;

		case WM_LBUTTONDBLCLK:
			// std::cout
			// 	<< ">   WM_LBUTTONDBLCLK: " << uMsg
			// 	<< "\n    Virtual Key: " << wParam
			// 	<< "\n    (mouse) x: " << LOWORD(lParam)
			// 	<< "\n    (mouse) y: " << HIWORD(lParam)
			// 	<< std::endl;

			break;

		case WM_NCLBUTTONDOWN:
			// std::cout
			// 	<< ">   WM_NCLBUTTONDOWN: " << uMsg
			// 	<< "\n    Virtual Key: " << wParam
			// 	<< "\n    (mouse) x: " << LOWORD(lParam)
			// 	<< "\n    (mouse) y: " << HIWORD(lParam)
			// 	<< std::endl;

			break;

		case WM_NCLBUTTONUP:
			// std::cout
			// 	<< ">   WM_NCLBUTTONUP: " << uMsg
			// 	<< "\n    Virtual Key: " << wParam
			// 	<< "\n    (mouse) x: " << LOWORD(lParam)
			// 	<< "\n    (mouse) y: " << HIWORD(lParam)
			// 	<< std::endl;

			break;

		case WM_NCLBUTTONDBLCLK:
			// std::cout
			// 	<< ">   WM_NCLBUTTONDBLCLK: " << uMsg
			// 	<< "\n    Virtual Key: " << wParam
			// 	<< "\n    (mouse) x: " << LOWORD(lParam)
			// 	<< "\n    (mouse) y: " << HIWORD(lParam)
			// 	<< std::endl;

			break;

		case WM_MBUTTONDOWN:
			// std::cout
			// 	<< ">   WM_MBUTTONDOWN: " << uMsg
			// 	<< "\n    Virtual Key: " << wParam
			// 	<< "\n    (mouse) x: " << LOWORD(lParam)
			// 	<< "\n    (mouse) y: " << HIWORD(lParam)
			// 	<< std::endl;

			break;

		case WM_MBUTTONUP:
			// std::cout
			// 	<< ">   WM_MBUTTONUP: " << uMsg
			// 	<< "\n    Virtual Key: " << wParam
			// 	<< "\n    (mouse) x: " << LOWORD(lParam)
			// 	<< "\n    (mouse) y: " << HIWORD(lParam)
			// 	<< std::endl;

			break;

		case WM_MBUTTONDBLCLK:
			// std::cout
			// 	<< ">   WM_MBUTTONDBLCLK: " << uMsg
			// 	<< "\n    Virtual Key: " << wParam
			// 	<< "\n    (mouse) x: " << LOWORD(lParam)
			// 	<< "\n    (mouse) y: " << HIWORD(lParam)
			// 	<< std::endl;

			break;
		
		case WM_NCMBUTTONDOWN:
			// std::cout
			// 	<< ">   WM_NCMBUTTONDOWN: " << uMsg
			// 	<< "\n    Virtual Key: " << wParam
			// 	<< "\n    (mouse) x: " << LOWORD(lParam)
			// 	<< "\n    (mouse) y: " << HIWORD(lParam)
			// 	<< std::endl;

			break;

		case WM_NCMBUTTONUP:
			// std::cout
			// 	<< ">   WM_NCMBUTTONUP: " << uMsg
			// 	<< "\n    Virtual Key: " << wParam
			// 	<< "\n    (mouse) x: " << LOWORD(lParam)
			// 	<< "\n    (mouse) y: " << HIWORD(lParam)
			// 	<< std::endl;

			break;

		case WM_NCMBUTTONDBLCLK:
			// std::cout
			// 	<< ">   WM_NCMBUTTONDBLCLK: " << uMsg
			// 	<< "\n    Virtual Key: " << wParam
			// 	<< "\n    (mouse) x: " << LOWORD(lParam)
			// 	<< "\n    (mouse) y: " << HIWORD(lParam)
			// 	<< std::endl;

			break;

		case WM_RBUTTONDOWN:
			// std::cout
			// 	<< ">   WM_RBUTTONDOWN: " << uMsg
			// 	<< "\n    Virtual Key: " << wParam
			// 	<< "\n    (mouse) x: " << LOWORD(lParam)
			// 	<< "\n    (mouse) y: " << HIWORD(lParam)
			// 	<< std::endl;

			break;

		case WM_RBUTTONUP:
			// std::cout
			// 	<< ">   WM_RBUTTONUP: " << uMsg
			// 	<< "\n    Virtual Key: " << wParam
			// 	<< "\n    (mouse) x: " << LOWORD(lParam)
			// 	<< "\n    (mouse) y: " << HIWORD(lParam)
			// 	<< std::endl;

			break;

		case WM_RBUTTONDBLCLK:
			// std::cout
			// 	<< ">   WM_RBUTTONDBLCLK: " << uMsg
			// 	<< "\n    Virtual Key: " << wParam
			// 	<< "\n    (mouse) x: " << LOWORD(lParam)
			// 	<< "\n    (mouse) y: " << HIWORD(lParam)
			// 	<< std::endl;

			break;

		case WM_NCRBUTTONDOWN:
			// std::cout
			// 	<< ">   WM_NCRBUTTONDOWN: " << uMsg
			// 	<< "\n    Virtual Key: " << wParam
			// 	<< "\n    (mouse) x: " << LOWORD(lParam)
			// 	<< "\n    (mouse) y: " << HIWORD(lParam)
			// 	<< std::endl;

			break;

		case WM_NCRBUTTONUP:
			// std::cout
			// 	<< ">   WM_NCRBUTTONUP: " << uMsg
			// 	<< "\n    Virtual Key: " << wParam
			// 	<< "\n    (mouse) x: " << LOWORD(lParam)
			// 	<< "\n    (mouse) y: " << HIWORD(lParam)
			// 	<< std::endl;

			break;

		case WM_NCRBUTTONDBLCLK:
			// std::cout
			// 	<< ">   WM_NCRBUTTONDBLCLK: " << uMsg
			// 	<< "\n    Virtual Key: " << wParam
			// 	<< "\n    (mouse) x: " << LOWORD(lParam)
			// 	<< "\n    (mouse) y: " << HIWORD(lParam)
			// 	<< std::endl;

			break;

		case WM_XBUTTONDOWN:
			// std::cout
			// 	<< ">   WM_XBUTTONDOWN: " << uMsg
			// 	<< "\n    Virtual Key: " << wParam
			// 	<< "\n    (mouse) x: " << LOWORD(lParam)
			// 	<< "\n    (mouse) y: " << HIWORD(lParam)
			// 	<< std::endl;

			break;

		case WM_XBUTTONUP:
			// std::cout
			// 	<< ">   WM_XBUTTONUP: " << uMsg
			// 	<< "\n    Virtual Key: " << wParam
			// 	<< "\n    (mouse) x: " << LOWORD(lParam)
			// 	<< "\n    (mouse) y: " << HIWORD(lParam)
			// 	<< std::endl;

			break;

		case WM_XBUTTONDBLCLK:
			// std::cout
			// 	<< ">   WM_XBUTTONDBLCLK: " << uMsg
			// 	<< "\n    Virtual Key: " << wParam
			// 	<< "\n    (mouse) x: " << LOWORD(lParam)
			// 	<< "\n    (mouse) y: " << HIWORD(lParam)
			// 	<< std::endl;

			break;

		case WM_NCXBUTTONDOWN:
			// std::cout
			// 	<< ">   WM_NCXBUTTONDOWN: " << uMsg
			// 	<< "\n    Virtual Key: " << wParam
			// 	<< "\n    (mouse) x: " << LOWORD(lParam)
			// 	<< "\n    (mouse) y: " << HIWORD(lParam)
			// 	<< std::endl;

			break;

		case WM_NCXBUTTONUP:
			// std::cout
			// 	<< ">   WM_NCXBUTTONUP: " << uMsg
			// 	<< "\n    Virtual Key: " << wParam
			// 	<< "\n    (mouse) x: " << LOWORD(lParam)
			// 	<< "\n    (mouse) y: " << HIWORD(lParam)
			// 	<< std::endl;

			break;

		case WM_NCXBUTTONDBLCLK:
			// std::cout
			// 	<< ">   WM_NCXBUTTONDBLCLK: " << uMsg
			// 	<< "\n    Virtual Key: " << wParam
			// 	<< "\n    (mouse) x: " << LOWORD(lParam)
			// 	<< "\n    (mouse) y: " << HIWORD(lParam)
			// 	<< std::endl;

			break;

		// ----------------------------------------------------------------------

		case WM_MOUSEMOVE:
			// SetCapture(HWND hWnd)
		   // Sets the mouse capture to the specified window belonging to the
		   // current thread. SetCapture captures mouse input either when the
		   // mouse is over the capturing window, or when the mouse button was
		   // pressed while the mouse was over the capturing window and the
		   // button is still down. Only one window at a time can capture the
		   // mouse.

			// ReleaseCapture()
			// Releases the mouse capture from a window in the current thread and
		   // restores normal mouse input processing. A window that has captured
		   // the mouse receives all mouse input, regardless of the position of
		   // the cursor, except when a mouse button is clicked while the cursor
		   // hot spot is in the window of another thread.

			// DragDetect()
			// Captures the mouse and tracks its movement until the user releases
			// the left button, presses the ESC key, or moves the mouse outside
			// the drag rectangle around the specified point. The width and height
			// of the drag rectangle are specified by the SM_CXDRAG and SM_CYDRAG
			// values returned by the GetSystemMetrics function.

			// ClipCursor()
			// Confines the cursor to a rectangular area on the screen. If a
			// subsequent cursor position (set by the SetCursorPos function or the
			// mouse) lies outside the rectangle, the system automatically adjusts
			// the position to keep the cursor inside the rectangular area.

			// ClientToScreen()
			// The ClientToScreen function converts the client-area coordinates of
			// a specified point to screen coordinates.

			// std::cout
			// 	<< ">   WM_MOUSEMOVE: " << uMsg
			// 	<< "\n    Virtual Key: " << wParam
			// 	<< "\n    (mouse) x: " << LOWORD(lParam)
			// 	<< "\n    (mouse) y: " << HIWORD(lParam)
			// 	<< std::endl;

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

// Complete list of messages
// WM_ACTIVATE
// WM_SETFOCUS
// WM_KILLFOCUS
// WM_ENABLE
// WM_SETREDRAW
// WM_SETTEXT
// WM_GETTEXT
// WM_GETTEXTLENGTH
// WM_PAINT
// WM_QUERYENDSESSION
// WM_QUERYOPEN
// WM_ERASEBKGND
// WM_SYSCOLORCHANGE
// WM_ENDSESSION
// WM_SYSTEMERROR
// WM_SHOWWINDOW
// WM_CTLCOLOR
// WM_WININICHANGE
// WM_SETTINGCHANGE
// WM_DEVMODECHANGE
// WM_ACTIVATEAPP
// WM_FONTCHANGE
// WM_TIMECHANGE
// WM_CANCELMODE
// WM_SETCURSOR
// WM_CHILDACTIVATE
// WM_QUEUESYNC
// WM_GETMINMAXINFO
// WM_PAINTICON
// WM_ICONERASEBKGND
// WM_NEXTDLGCTL
// WM_SPOOLERSTATUS
// WM_DRAWITEM
// WM_MEASUREITEM
// WM_DELETEITEM
// WM_VKEYTOITEM
// WM_CHARTOITEM
// WM_SETFONT
// WM_GETFONT
// WM_SETHOTKEY
// WM_GETHOTKEY
// WM_QUERYDRAGICON
// WM_COMPAREITEM
// WM_COMPACTING
// WM_GETOBJECT
// WM_COMMNOTIFY
// WM_WINDOWPOSCHANGING
// WM_WINDOWPOSCHANGED
// WM_POWER
// WM_COPYGLOBALDATA
// WM_COPYDATA
// WM_CANCELJOURNAL
// WM_NOTIFY
// WM_INPUTLANGCHANGEREQUEST
// WM_INPUTLANGCHANGE
// WM_TCARD
// WM_HELP
// WM_USERCHANGED
// WM_NOTIFYFORMAT
// WM_CONTEXTMENU
// WM_STYLECHANGING
// WM_STYLECHANGED
// WM_DISPLAYCHANGE
// WM_GETICON
// WM_SETICON
// WM_NCCREATE
// WM_NCDESTROY
// WM_NCCALCSIZE
// WM_NCHITTEST
// WM_NCPAINT
// WM_NCACTIVATE
// WM_GETDLGCODE
// WM_SYNCPAINT
// EM_GETSEL
// EM_SETSEL
// EM_GETRECT
// EM_SETRECT
// EM_SETRECTNP
// EM_SCROLL
// EM_LINESCROLL
// EM_SCROLLCARET
// EM_GETMODIFY
// EM_SETMODIFY
// EM_GETLINECOUNT
// EM_LINEINDEX
// EM_SETHANDLE
// EM_GETHANDLE
// EM_GETTHUMB
// EM_LINELENGTH
// EM_REPLACESEL
// EM_SETFONT
// EM_GETLINE
// EM_LIMITTEXT
// EM_SETLIMITTEXT
// EM_CANUNDO
// EM_UNDO
// EM_FMTLINES
// EM_LINEFROMCHAR
// EM_SETWORDBREAK
// EM_SETTABSTOPS
// EM_SETPASSWORDCHAR
// EM_EMPTYUNDOBUFFER
// EM_GETFIRSTVISIBLELINE
// EM_SETREADONLY
// EM_SETWORDBREAKPROC
// EM_GETWORDBREAKPROC
// EM_GETPASSWORDCHAR
// EM_SETMARGINS
// EM_GETMARGINS
// EM_GETLIMITTEXT
// EM_POSFROMCHAR
// EM_CHARFROMPOS
// EM_SETIMESTATUS
// EM_GETIMESTATUS
// SBM_SETPOS
// SBM_GETPOS
// SBM_SETRANGE
// SBM_GETRANGE
// SBM_ENABLE_ARROWS
// SBM_SETRANGEREDRAW
// SBM_SETSCROLLINFO
// SBM_GETSCROLLINFO
// SBM_GETSCROLLBARINFO
// BM_GETCHECK
// BM_SETCHECK
// BM_GETSTATE
// BM_SETSTATE
// BM_SETSTYLE
// BM_CLICK
// BM_GETIMAGE
// BM_SETIMAGE
// BM_SETDONTCLICK
// WM_INPUT
// WM_KEYFIRST
// WM_SYSDEADCHAR
// WM_UNICHAR / WM_KEYLAST
// WM_WNT_CONVERTREQUESTEX
// WM_CONVERTREQUEST
// WM_CONVERTRESULT
// WM_INTERIM
// WM_IME_STARTCOMPOSITION
// WM_IME_ENDCOMPOSITION
// WM_IME_COMPOSITION
// WM_IME_KEYLAST
// WM_INITDIALOG
// WM_COMMAND
// WM_SYSCOMMAND
// WM_TIMER
// WM_HSCROLL
// WM_VSCROLL
// WM_INITMENU
// WM_INITMENUPOPUP
// WM_SYSTIMER
// WM_GESTURE
// WM_MENUCHAR
// WM_ENTERIDLE
// WM_MENURBUTTONUP
// WM_MENUDRAG
// WM_MENUGETOBJECT
// WM_UNINITMENUPOPUP
// WM_MENUCOMMAND
// WM_CHANGEUISTATE
// WM_UPDATEUISTATE
// WM_QUERYUISTATE
// WM_CTLCOLORMSGBOX
// WM_CTLCOLOREDIT
// WM_CTLCOLORLISTBOX
// WM_CTLCOLORBTN
// WM_CTLCOLORDLG
// WM_CTLCOLORSCROLLBAR
// WM_CTLCOLORSTATIC
// WM_PARENTNOTIFY
// WM_ENTERMENULOOP
// WM_EXITMENULOOP
// WM_NEXTMENU
// WM_SIZING
// WM_CAPTURECHANGED
// WM_MOVING
// WM_POWERBROADCAST
// WM_DEVICECHANGE
// WM_MDICREATE
// WM_MDIDESTROY
// WM_MDIACTIVATE
// WM_MDIRESTORE
// WM_MDINEXT
// WM_MDIMAXIMIZE
// WM_MDITILE
// WM_MDICASCADE
// WM_MDIICONARRANGE
// WM_MDIGETACTIVE
// WM_MDISETMENU
// WM_ENTERSIZEMOVE
// WM_EXITSIZEMOVE
// WM_DROPFILES
// WM_MDIREFRESHMENU
// WM_IME_REPORT
// WM_IME_SETCONTEXT
// WM_IME_NOTIFY
// WM_IME_CONTROL
// WM_IME_COMPOSITIONFULL
// WM_IME_SELECT
// WM_IME_REQUEST
// WM_IMEKEYDOWN
// WM_IME_KEYDOWN
// WM_IMEKEYUP
// WM_IME_KEYUP
// WM_CUT
// WM_COPY
// WM_PASTE
// WM_CLEAR
// WM_UNDO
// WM_RENDERFORMAT
// WM_RENDERALLFORMATS
// WM_DESTROYCLIPBOARD
// WM_DRAWCLIPBOARD
// WM_PAINTCLIPBOARD
// WM_VSCROLLCLIPBOARD
// WM_SIZECLIPBOARD
// WM_ASKCBFORMATNAME
// WM_CHANGECBCHAIN
// WM_HSCROLLCLIPBOARD
// WM_QUERYNEWPALETTE
// WM_PALETTEISCHANGING
// WM_PALETTECHANGED
// WM_HOTKEY
// WM_PRINT
// WM_PRINTCLIENT
// WM_APPCOMMAND
// WM_HANDHELDFIRST
// WM_HANDHELDLAST
// WM_AFXFIRST
// WM_AFXLAST
// WM_PENWINFIRST
// WM_RCRESULT
// WM_HOOKRCRESULT
// WM_GLOBALRCCHANGE
// WM_PENMISCINFO
// WM_SKB
// WM_HEDITCTL
// WM_PENCTL
// WM_PENMISC
// WM_CTLINIT
// WM_PENEVENT
// WM_PENWINLAST
// DDM_SETFMT
// DM_GETDEFID
// NIN_SELECT
// TBM_GETPOS
// WM_PSD_PAGESETUPDLG
// WM_USER
// CBEM_INSERTITEMA
// DDM_DRAW
// DM_SETDEFID
// HKM_SETHOTKEY
// PBM_SETRANGE
// RB_INSERTBANDA
// SB_SETTEXTA
// TB_ENABLEBUTTON
// TBM_GETRANGEMIN
// TTM_ACTIVATE
// WM_CHOOSEFONT_GETLOGFONT
// WM_PSD_FULLPAGERECT
// CBEM_SETIMAGELIST
// DDM_CLOSE
// DM_REPOSITION
// HKM_GETHOTKEY
// PBM_SETPOS
// RB_DELETEBAND
// SB_GETTEXTA
// TB_CHECKBUTTON
// TBM_GETRANGEMAX
// WM_PSD_MINMARGINRECT
// CBEM_GETIMAGELIST
// DDM_BEGIN
// HKM_SETRULES
// PBM_DELTAPOS
// RB_GETBARINFO
// SB_GETTEXTLENGTHA
// TBM_GETTIC
// TB_PRESSBUTTON
// TTM_SETDELAYTIME
// WM_PSD_MARGINRECT
// CBEM_GETITEMA
// DDM_END
// PBM_SETSTEP
// RB_SETBARINFO
// SB_SETPARTS
// TB_HIDEBUTTON
// TBM_SETTIC
// TTM_ADDTOOLA
// WM_PSD_GREEKTEXTRECT
// CBEM_SETITEMA
// PBM_STEPIT
// TB_INDETERMINATE
// TBM_SETPOS
// TTM_DELTOOLA
// WM_PSD_ENVSTAMPRECT
// CBEM_GETCOMBOCONTROL
// PBM_SETRANGE32
// RB_SETBANDINFOA
// SB_GETPARTS
// TB_MARKBUTTON
// TBM_SETRANGE
// TTM_NEWTOOLRECTA
// WM_PSD_YAFULLPAGERECT
// CBEM_GETEDITCONTROL
// PBM_GETRANGE
// RB_SETPARENT
// SB_GETBORDERS
// TBM_SETRANGEMIN
// TTM_RELAYEVENT
// CBEM_SETEXSTYLE
// PBM_GETPOS
// RB_HITTEST
// SB_SETMINHEIGHT
// TBM_SETRANGEMAX
// TTM_GETTOOLINFOA
// CBEM_GETEXSTYLE
// CBEM_GETEXTENDEDSTYLE
// PBM_SETBARCOLOR
// RB_GETRECT
// SB_SIMPLE
// TB_ISBUTTONENABLED
// TBM_CLEARTICS
// TTM_SETTOOLINFOA
// CBEM_HASEDITCHANGED
// RB_INSERTBANDW
// SB_GETRECT
// TB_ISBUTTONCHECKED
// TBM_SETSEL
// TTM_HITTESTA
// WIZ_QUERYNUMPAGES
// CBEM_INSERTITEMW
// RB_SETBANDINFOW
// SB_SETTEXTW
// TB_ISBUTTONPRESSED
// TBM_SETSELSTART
// TTM_GETTEXTA
// WIZ_NEXT
// CBEM_SETITEMW
// RB_GETBANDCOUNT
// SB_GETTEXTLENGTHW
// TB_ISBUTTONHIDDEN
// TBM_SETSELEND
// TTM_UPDATETIPTEXTA
// WIZ_PREV
// CBEM_GETITEMW
// RB_GETROWCOUNT
// SB_GETTEXTW
// TB_ISBUTTONINDETERMINATE
// TTM_GETTOOLCOUNT
// CBEM_SETEXTENDEDSTYLE
// RB_GETROWHEIGHT
// SB_ISSIMPLE
// TB_ISBUTTONHIGHLIGHTED
// TBM_GETPTICS
// TTM_ENUMTOOLSA
// SB_SETICON
// TBM_GETTICPOS
// TTM_GETCURRENTTOOLA
// RB_IDTOINDEX
// SB_SETTIPTEXTA
// TBM_GETNUMTICS
// TTM_WINDOWFROMPOINT
// RB_GETTOOLTIPS
// SB_SETTIPTEXTW
// TBM_GETSELSTART
// TB_SETSTATE
// TTM_TRACKACTIVATE
// RB_SETTOOLTIPS
// SB_GETTIPTEXTA
// TB_GETSTATE
// TBM_GETSELEND
// TTM_TRACKPOSITION
// RB_SETBKCOLOR
// SB_GETTIPTEXTW
// TB_ADDBITMAP
// TBM_CLEARSEL
// TTM_SETTIPBKCOLOR
// RB_GETBKCOLOR
// SB_GETICON
// TB_ADDBUTTONSA
// TBM_SETTICFREQ
// TTM_SETTIPTEXTCOLOR
// RB_SETTEXTCOLOR
// TB_INSERTBUTTONA
// TBM_SETPAGESIZE
// TTM_GETDELAYTIME
// RB_GETTEXTCOLOR
// TB_DELETEBUTTON
// TBM_GETPAGESIZE
// TTM_GETTIPBKCOLOR
// RB_SIZETORECT
// TB_GETBUTTON
// TBM_SETLINESIZE
// TTM_GETTIPTEXTCOLOR
// RB_BEGINDRAG
// TB_BUTTONCOUNT
// TBM_GETLINESIZE
// TTM_SETMAXTIPWIDTH
// RB_ENDDRAG
// TB_COMMANDTOINDEX
// TBM_GETTHUMBRECT
// TTM_GETMAXTIPWIDTH
// RB_DRAGMOVE
// TBM_GETCHANNELRECT
// TB_SAVERESTOREA
// TTM_SETMARGIN
// RB_GETBARHEIGHT
// TB_CUSTOMIZE
// TBM_SETTHUMBLENGTH
// TTM_GETMARGIN
// RB_GETBANDINFOW
// TB_ADDSTRINGA
// TBM_GETTHUMBLENGTH
// TTM_POP
// RB_GETBANDINFOA
// TB_GETITEMRECT
// TBM_SETTOOLTIPS
// TTM_UPDATE
// RB_MINIMIZEBAND
// TB_BUTTONSTRUCTSIZE
// TBM_GETTOOLTIPS
// TTM_GETBUBBLESIZE
// RB_MAXIMIZEBAND
// TBM_SETTIPSIDE
// TB_SETBUTTONSIZE
// TTM_ADJUSTRECT
// TBM_SETBUDDY
// TB_SETBITMAPSIZE
// TTM_SETTITLEA
// MSG_FTS_JUMP_VA
// TB_AUTOSIZE
// TBM_GETBUDDY
// TTM_SETTITLEW
// RB_GETBANDBORDERS
// MSG_FTS_JUMP_QWORD
// RB_SHOWBAND
// TB_GETTOOLTIPS
// MSG_REINDEX_REQUEST
// TB_SETTOOLTIPS
// MSG_FTS_WHERE_IS_IT
// RB_SETPALETTE
// TB_SETPARENT
// RB_GETPALETTE
// RB_MOVEBAND
// TB_SETROWS
// TB_GETROWS
// TB_GETBITMAPFLAGS
// TB_SETCMDID
// RB_PUSHCHEVRON
// TB_CHANGEBITMAP
// TB_GETBITMAP
// MSG_GET_DEFFONT
// TB_GETBUTTONTEXTA
// TB_REPLACEBITMAP
// TB_SETINDENT
// TB_SETIMAGELIST
// TB_GETIMAGELIST
// TB_LOADIMAGES
// EM_CANPASTE
// TTM_ADDTOOLW
// EM_DISPLAYBAND
// TB_GETRECT
// TTM_DELTOOLW
// EM_EXGETSEL
// TB_SETHOTIMAGELIST
// TTM_NEWTOOLRECTW
// EM_EXLIMITTEXT
// TB_GETHOTIMAGELIST
// TTM_GETTOOLINFOW
// EM_EXLINEFROMCHAR
// TB_SETDISABLEDIMAGELIST
// TTM_SETTOOLINFOW
// EM_EXSETSEL
// TB_GETDISABLEDIMAGELIST
// TTM_HITTESTW
// EM_FINDTEXT
// TB_SETSTYLE
// TTM_GETTEXTW
// EM_FORMATRANGE
// TB_GETSTYLE
// TTM_UPDATETIPTEXTW
// EM_GETCHARFORMAT
// TB_GETBUTTONSIZE
// TTM_ENUMTOOLSW
// EM_GETEVENTMASK
// TB_SETBUTTONWIDTH
// TTM_GETCURRENTTOOLW
// EM_GETOLEINTERFACE
// TB_SETMAXTEXTROWS
// EM_GETPARAFORMAT
// TB_GETTEXTROWS
// EM_GETSELTEXT
// TB_GETOBJECT
// EM_HIDESELECTION
// TB_GETBUTTONINFOW
// EM_PASTESPECIAL
// TB_SETBUTTONINFOW
// EM_REQUESTRESIZE
// TB_GETBUTTONINFOA
// EM_SELECTIONTYPE
// TB_SETBUTTONINFOA
// EM_SETBKGNDCOLOR
// TB_INSERTBUTTONW
// EM_SETCHARFORMAT
// TB_ADDBUTTONSW
// EM_SETEVENTMASK
// TB_HITTEST
// EM_SETOLECALLBACK
// TB_SETDRAWTEXTFLAGS
// EM_SETPARAFORMAT
// TB_GETHOTITEM
// EM_SETTARGETDEVICE
// TB_SETHOTITEM
// EM_STREAMIN
// TB_SETANCHORHIGHLIGHT
// EM_STREAMOUT
// TB_GETANCHORHIGHLIGHT
// EM_GETTEXTRANGE
// TB_GETBUTTONTEXTW
// EM_FINDWORDBREAK
// TB_SAVERESTOREW
// EM_SETOPTIONS
// TB_ADDSTRINGW
// EM_GETOPTIONS
// TB_MAPACCELERATORA
// EM_FINDTEXTEX
// TB_GETINSERTMARK
// EM_GETWORDBREAKPROCEX
// TB_SETINSERTMARK
// EM_SETWORDBREAKPROCEX
// TB_INSERTMARKHITTEST
// EM_SETUNDOLIMIT
// TB_MOVEBUTTON
// TB_GETMAXSIZE
// EM_REDO
// TB_SETEXTENDEDSTYLE
// EM_CANREDO
// TB_GETEXTENDEDSTYLE
// EM_GETUNDONAME
// TB_GETPADDING
// EM_GETREDONAME
// TB_SETPADDING
// EM_STOPGROUPTYPING
// TB_SETINSERTMARKCOLOR
// EM_SETTEXTMODE
// TB_GETINSERTMARKCOLOR
// EM_GETTEXTMODE
// TB_MAPACCELERATORW
// EM_AUTOURLDETECT
// TB_GETSTRINGW
// EM_GETAUTOURLDETECT
// TB_GETSTRINGA
// EM_SETPALETTE
// EM_GETTEXTEX
// EM_GETTEXTLENGTHEX
// EM_SHOWSCROLLBAR
// EM_SETTEXTEX
// TAPI_REPLY
// ACM_OPENA
// BFFM_SETSTATUSTEXTA
// CDM_FIRST
// CDM_GETSPEC
// EM_SETPUNCTUATION
// IPM_CLEARADDRESS
// WM_CAP_UNICODE_START
// ACM_PLAY
// BFFM_ENABLEOK
// CDM_GETFILEPATH
// EM_GETPUNCTUATION
// IPM_SETADDRESS
// PSM_SETCURSEL
// UDM_SETRANGE
// WM_CHOOSEFONT_SETLOGFONT
// ACM_STOP
// BFFM_SETSELECTIONA
// CDM_GETFOLDERPATH
// EM_SETWORDWRAPMODE
// IPM_GETADDRESS
// PSM_REMOVEPAGE
// UDM_GETRANGE
// WM_CAP_SET_CALLBACK_ERRORW
// WM_CHOOSEFONT_SETFLAGS
// ACM_OPENW
// BFFM_SETSELECTIONW
// CDM_GETFOLDERIDLIST
// EM_GETWORDWRAPMODE
// IPM_SETRANGE
// PSM_ADDPAGE
// UDM_SETPOS
// WM_CAP_SET_CALLBACK_STATUSW
// BFFM_SETSTATUSTEXTW
// CDM_SETCONTROLTEXT
// EM_SETIMECOLOR
// IPM_SETFOCUS
// PSM_CHANGED
// UDM_GETPOS
// CDM_HIDECONTROL
// EM_GETIMECOLOR
// IPM_ISBLANK
// PSM_RESTARTWINDOWS
// UDM_SETBUDDY
// CDM_SETDEFEXT
// EM_SETIMEOPTIONS
// PSM_REBOOTSYSTEM
// UDM_GETBUDDY
// EM_GETIMEOPTIONS
// PSM_CANCELTOCLOSE
// UDM_SETACCEL
// EM_CONVPOSITION
// EM_CONVPOSITION
// PSM_QUERYSIBLINGS
// UDM_GETACCEL
// MCIWNDM_GETZOOM
// PSM_UNCHANGED
// UDM_SETBASE
// PSM_APPLY
// UDM_GETBASE
// PSM_SETTITLEA
// UDM_SETRANGE32
// PSM_SETWIZBUTTONS
// UDM_GETRANGE32
// WM_CAP_DRIVER_GET_NAMEW
// PSM_PRESSBUTTON
// UDM_SETPOS32
// WM_CAP_DRIVER_GET_VERSIONW
// PSM_SETCURSELID
// UDM_GETPOS32
// PSM_SETFINISHTEXTA
// PSM_GETTABCONTROL
// PSM_ISDIALOGMESSAGE
// MCIWNDM_REALIZE
// PSM_GETCURRENTPAGEHWND
// MCIWNDM_SETTIMEFORMATA
// PSM_INSERTPAGE
// EM_SETLANGOPTIONS
// MCIWNDM_GETTIMEFORMATA
// PSM_SETTITLEW
// WM_CAP_FILE_SET_CAPTURE_FILEW
// EM_GETLANGOPTIONS
// MCIWNDM_VALIDATEMEDIA
// PSM_SETFINISHTEXTW
// WM_CAP_FILE_GET_CAPTURE_FILEW
// EM_GETIMECOMPMODE
// EM_FINDTEXTW
// MCIWNDM_PLAYTO
// WM_CAP_FILE_SAVEASW
// EM_FINDTEXTEXW
// MCIWNDM_GETFILENAMEA
// EM_RECONVERSION
// MCIWNDM_GETDEVICEA
// PSM_SETHEADERTITLEA
// WM_CAP_FILE_SAVEDIBW
// EM_SETIMEMODEBIAS
// MCIWNDM_GETPALETTE
// PSM_SETHEADERTITLEW
// EM_GETIMEMODEBIAS
// MCIWNDM_SETPALETTE
// PSM_SETHEADERSUBTITLEA
// MCIWNDM_GETERRORA
// PSM_SETHEADERSUBTITLEW
// PSM_HWNDTOINDEX
// PSM_INDEXTOHWND
// MCIWNDM_SETINACTIVETIMER
// PSM_PAGETOINDEX
// PSM_INDEXTOPAGE
// DL_BEGINDRAG
// MCIWNDM_GETINACTIVETIMER
// PSM_IDTOINDEX
// DL_DRAGGING
// PSM_INDEXTOID
// DL_DROPPED
// PSM_GETRESULT
// DL_CANCELDRAG
// PSM_RECALCPAGESIZES
// MCIWNDM_GET_SOURCE
// MCIWNDM_PUT_SOURCE
// MCIWNDM_GET_DEST
// MCIWNDM_PUT_DEST
// MCIWNDM_CAN_PLAY
// MCIWNDM_CAN_WINDOW
// MCIWNDM_CAN_RECORD
// MCIWNDM_CAN_SAVE
// MCIWNDM_CAN_EJECT
// MCIWNDM_CAN_CONFIG
// IE_GETINK
// IE_MSGFIRST
// MCIWNDM_PALETTEKICK
// IE_SETINK
// IE_GETPENTIP
// IE_SETPENTIP
// IE_GETERASERTIP
// IE_SETERASERTIP
// IE_GETBKGND
// IE_SETBKGND
// IE_GETGRIDORIGIN
// IE_SETGRIDORIGIN
// IE_GETGRIDPEN
// IE_SETGRIDPEN
// IE_GETGRIDSIZE
// IE_SETGRIDSIZE
// IE_GETMODE
// IE_SETMODE
// IE_GETINKRECT
// WM_CAP_SET_MCI_DEVICEW
// WM_CAP_GET_MCI_DEVICEW
// WM_CAP_PAL_OPENW
// WM_CAP_PAL_SAVEW
// IE_GETAPPDATA
// IE_SETAPPDATA
// IE_GETDRAWOPTS
// IE_SETDRAWOPTS
// IE_GETFORMAT
// IE_SETFORMAT
// IE_GETINKINPUT
// IE_SETINKINPUT
// IE_GETNOTIFY
// IE_SETNOTIFY
// IE_GETRECOG
// IE_SETRECOG
// IE_GETSECURITY
// IE_SETSECURITY
// IE_GETSEL
// IE_SETSEL
// CDM_LAST
// EM_SETBIDIOPTIONS
// IE_DOCOMMAND
// MCIWNDM_NOTIFYMODE
// EM_GETBIDIOPTIONS
// IE_GETCOMMAND
// EM_SETTYPOGRAPHYOPTIONS
// IE_GETCOUNT
// EM_GETTYPOGRAPHYOPTIONS
// IE_GETGESTURE
// MCIWNDM_NOTIFYMEDIA
// EM_SETEDITSTYLE
// IE_GETMENU
// EM_GETEDITSTYLE
// IE_GETPAINTDC
// MCIWNDM_NOTIFYERROR
// IE_GETPDEVENT
// IE_GETSELCOUNT
// IE_GETSELITEMS
// IE_GETSTYLE
// MCIWNDM_SETTIMEFORMATW
// EM_OUTLINE
// MCIWNDM_GETTIMEFORMATW
// EM_GETSCROLLPOS
// EM_SETSCROLLPOS
// EM_SETSCROLLPOS
// EM_SETFONTSIZE
// EM_GETZOOM
// MCIWNDM_GETFILENAMEW
// EM_SETZOOM
// MCIWNDM_GETDEVICEW
// EM_GETVIEWKIND
// EM_SETVIEWKIND
// EM_GETPAGE
// MCIWNDM_GETERRORW
// EM_SETPAGE
// EM_GETHYPHENATEINFO
// EM_SETHYPHENATEINFO
// EM_GETPAGEROTATE
// EM_SETPAGEROTATE
// EM_GETCTFMODEBIAS
// EM_SETCTFMODEBIAS
// EM_GETCTFOPENSTATUS
// EM_SETCTFOPENSTATUS
// EM_GETIMECOMPTEXT
// EM_ISIME
// EM_GETIMEPROPERTY
// EM_GETQUERYRTFOBJ
// EM_SETQUERYRTFOBJ
// FM_GETFOCUS
// FM_GETDRIVEINFOA
// FM_GETSELCOUNT
// FM_GETSELCOUNTLFN
// FM_GETFILESELA
// FM_GETFILESELLFNA
// FM_REFRESH_WINDOWS
// FM_RELOAD_EXTENSIONS
// FM_GETDRIVEINFOW
// FM_GETFILESELW
// FM_GETFILESELLFNW
// WLX_WM_SAS
// SM_GETSELCOUNT
// UM_GETSELCOUNT
// WM_CPL_LAUNCH
// SM_GETSERVERSELA
// UM_GETUSERSELA
// WM_CPL_LAUNCHED
// SM_GETSERVERSELW
// UM_GETUSERSELW
// SM_GETCURFOCUSA
// UM_GETGROUPSELA
// SM_GETCURFOCUSW
// UM_GETGROUPSELW
// SM_GETOPTIONS
// UM_GETCURFOCUSA
// UM_GETCURFOCUSW
// UM_GETOPTIONS
// UM_GETOPTIONS2
// LVM_FIRST
// LVM_GETBKCOLOR
// LVM_SETBKCOLOR
// LVM_GETIMAGELIST
// LVM_SETIMAGELIST
// LVM_GETITEMCOUNT
// LVM_GETITEMA
// LVM_SETITEMA
// LVM_INSERTITEMA
// LVM_DELETEITEM
// LVM_DELETEALLITEMS
// LVM_GETCALLBACKMASK
// LVM_SETCALLBACKMASK
// LVM_GETNEXTITEM
// LVM_FINDITEMA
// LVM_GETITEMRECT
// LVM_SETITEMPOSITION
// LVM_GETITEMPOSITION
// LVM_GETSTRINGWIDTHA
// LVM_HITTEST
// LVM_ENSUREVISIBLE
// LVM_SCROLL
// LVM_REDRAWITEMS
// LVM_ARRANGE
// LVM_EDITLABELA
// LVM_GETEDITCONTROL
// LVM_GETCOLUMNA
// LVM_SETCOLUMNA
// LVM_INSERTCOLUMNA
// LVM_DELETECOLUMN
// LVM_GETCOLUMNWIDTH
// LVM_SETCOLUMNWIDTH
// LVM_GETHEADER
// LVM_CREATEDRAGIMAGE
// LVM_GETVIEWRECT
// LVM_GETTEXTCOLOR
// LVM_SETTEXTCOLOR
// LVM_GETTEXTBKCOLOR
// LVM_SETTEXTBKCOLOR
// LVM_GETTOPINDEX
// LVM_GETCOUNTPERPAGE
// LVM_GETORIGIN
// LVM_UPDATE
// LVM_SETITEMSTATE
// LVM_GETITEMSTATE
// LVM_GETITEMTEXTA
// LVM_SETITEMTEXTA
// LVM_SETITEMCOUNT
// LVM_SORTITEMS
// LVM_SETITEMPOSITION32
// LVM_GETSELECTEDCOUNT
// LVM_GETITEMSPACING
// LVM_GETISEARCHSTRINGA
// LVM_SETICONSPACING
// LVM_SETEXTENDEDLISTVIEWSTYLE
// LVM_GETEXTENDEDLISTVIEWSTYLE
// LVM_GETSUBITEMRECT
// LVM_SUBITEMHITTEST
// LVM_SETCOLUMNORDERARRAY
// LVM_GETCOLUMNORDERARRAY
// LVM_SETHOTITEM
// LVM_GETHOTITEM
// LVM_SETHOTCURSOR
// LVM_GETHOTCURSOR
// LVM_APPROXIMATEVIEWRECT
// LVM_SETWORKAREAS
// LVM_GETSELECTIONMARK
// LVM_SETSELECTIONMARK
// LVM_SETBKIMAGEA
// LVM_GETBKIMAGEA
// LVM_GETWORKAREAS
// LVM_SETHOVERTIME
// LVM_GETHOVERTIME
// LVM_GETNUMBEROFWORKAREAS
// LVM_SETTOOLTIPS
// LVM_GETITEMW
// LVM_SETITEMW
// LVM_INSERTITEMW
// LVM_GETTOOLTIPS
// LVM_FINDITEMW
// LVM_GETSTRINGWIDTHW
// LVM_GETCOLUMNW
// LVM_SETCOLUMNW
// LVM_INSERTCOLUMNW
// LVM_GETITEMTEXTW
// LVM_SETITEMTEXTW
// LVM_GETISEARCHSTRINGW
// LVM_EDITLABELW
// LVM_GETBKIMAGEW
// LVM_SETSELECTEDCOLUMN
// LVM_SETTILEWIDTH
// LVM_SETVIEW
// LVM_GETVIEW
// LVM_INSERTGROUP
// LVM_SETGROUPINFO
// LVM_GETGROUPINFO
// LVM_REMOVEGROUP
// LVM_MOVEGROUP
// LVM_MOVEITEMTOGROUP
// LVM_SETGROUPMETRICS
// LVM_GETGROUPMETRICS
// LVM_ENABLEGROUPVIEW
// LVM_SORTGROUPS
// LVM_INSERTGROUPSORTED
// LVM_REMOVEALLGROUPS
// LVM_HASGROUP
// LVM_SETTILEVIEWINFO
// LVM_GETTILEVIEWINFO
// LVM_SETTILEINFO
// LVM_GETTILEINFO
// LVM_SETINSERTMARK
// LVM_GETINSERTMARK
// LVM_INSERTMARKHITTEST
// LVM_GETINSERTMARKRECT
// LVM_SETINSERTMARKCOLOR
// LVM_GETINSERTMARKCOLOR
// LVM_SETINFOTIP
// LVM_GETSELECTEDCOLUMN
// LVM_ISGROUPVIEWENABLED
// LVM_GETOUTLINECOLOR
// LVM_SETOUTLINECOLOR
// LVM_CANCELEDITLABEL
// LVM_MAPINDEXTOID
// LVM_MAPIDTOINDEX
// LVM_ISITEMVISIBLE
// OCM__BASE
// LVM_SETUNICODEFORMAT
// LVM_GETUNICODEFORMAT
// OCM_CTLCOLOR
// OCM_DRAWITEM
// OCM_MEASUREITEM
// OCM_DELETEITEM
// OCM_VKEYTOITEM
// OCM_CHARTOITEM
// OCM_COMPAREITEM
// OCM_NOTIFY
// OCM_COMMAND
// OCM_HSCROLL
// OCM_VSCROLL
// OCM_CTLCOLORMSGBOX
// OCM_CTLCOLOREDIT
// OCM_CTLCOLORLISTBOX
// OCM_CTLCOLORBTN
// OCM_CTLCOLORDLG
// OCM_CTLCOLORSCROLLBAR
// OCM_CTLCOLORSTATIC
// OCM_PARENTNOTIFY
// WM_APP
// WM_RASDIALEVENT
// WM_COALESCE_FIRST := 0x390
// WM_COALESCE_LAST := 0x39F
// WM_DDE_FIRST := 0x3E0
// WM_DDE_INITIATE := 0x3E0
// WM_DDE_TERMINATE := 0x3E1
// WM_DDE_ADVISE := 0x3E2
// WM_DDE_UNADVISE := 0x3E3
// WM_DDE_ACK := 0x3E4
// WM_DDE_DATA := 0x3E5
// WM_DDE_REQUEST := 0x3E6
// WM_DDE_POKE := 0x3E7
// WM_DDE_EXECUTE := 0x3E8
// WM_DDE_LAST := 0x3E8
// WM_INPUT_DEVICE_CHANGE
// WM_GESTURENOTIFY
// WM_MENUSELECT
// WM_POINTERDEVICECHANGE
// WM_POINTERDEVICEINRANGE
// WM_TOUCH
// WM_NCPOINTERUPDATE
// WM_NCPOINTERDOWN
// WM_NCPOINTERUP
// WM_POINTERUPDATE
// WM_POINTERDOWN
// WM_POINTERUP
// WM_POINTERENTER
// WM_POINTERLEAVE
// WM_POINTERACTIVATE
// WM_POINTERCAPTURECHANGED
// WM_TOUCHHITTESTING
// WM_POINTERWHEEL
// WM_WTSSESSION_CHANGE
// WM_TABLET_FIRST
// WM_TABLET_LAST
// WM_THEMECHANGED
// WM_CLIPBOARDUPDATE
// WM_DWMCOMPOSITIONCHANGED
// WM_DWMNCRENDERINGCHANGED
// WM_DWMCOLORIZATIONCOLORCHANGED
// WM_DWMWINDOWMAXIMIZEDCHANGE
// WM_DWMSENDICONICTHUMBNAIL
// WM_NCUAHDRAWCAPTION
// WM_NCUAHDRAWFRAME
// WM_OTHERWINDOWCREATED
// WM_OTHERWINDOWDESTROYED
