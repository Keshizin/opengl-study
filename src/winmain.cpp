#include <windows.h>
#include <GL/gl.h>

#include <iostream>
// #include <iomanip>
// #include <GL/glu.h>

// #include <gewin.h>
// #include <gewinapiwrapper.h>

#include <eventhandler.h>

// ----------------------------------------------------------------------------
//  SYMBOLIC CONSTANTS
// ----------------------------------------------------------------------------
#define MOUSE_BUTTON_LEFT   1
#define MOUSE_BUTTON_MIDDLE 2
#define MOUSE_BUTTON_RIGHT  3

// ----------------------------------------------------------------------------
//  GLOBAL VARIABLES
// ----------------------------------------------------------------------------
HGLRC hRC = NULL;
HWND hWindow = NULL;
HDC hDC = NULL;

// ----------------------------------------------------------------------------
//  FUNCTION PROTOTYPE DECLARATION
// ----------------------------------------------------------------------------
LRESULT CALLBACK windowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int createWindow(int width, int height, int x, int y);
int destroyWindow();

void frameEvent();
void mouseEvent(int button, int state, int x, int y);
void mouseMotionEvent(int x, int y);
void keyboardEvent(unsigned char key, int state);
void keyboardSpecialEvent(unsigned char key, int state);
void resizeWindowEvent(int width, int height);

// ----------------------------------------------------------------------------
//  MAIN FUNCTION
// ----------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int ret;

	std::cout << "> Welcome to OpenGL Application" << std::endl;

	ret = createWindow(640, 480, 0, 0);
	std::cout << "> window created: " << ret << std::endl;

	ret = ShowWindow(hWindow, SW_SHOW);
	std::cout << "> show window: " << ret << std::endl;

	// ret = destroyWindow();
	// std::cout << "> window destroyed: " << ret << std::endl;

	MSG msg;
	bool isDone = false;

	LARGE_INTEGER frameTime;
	LARGE_INTEGER startTime;
	LARGE_INTEGER endTime;

	QueryPerformanceCounter(&endTime);

	while(!isDone)
	{
		// --------------------------------------------------------------------
		// START OF FRAME
		// --------------------------------------------------------------------
		QueryPerformanceCounter(&startTime);

		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{
				// ret = destroyWindow();
				// std::cout << "> window destroyed: " << ret << std::endl;
				isDone = true;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// --------------------------------------------------------------------
		// RENDERING STUFF HERE
		// --------------------------------------------------------------------

		// --------------------------------------------------------------------
		// END OF FRAME
		// --------------------------------------------------------------------
		frameTime.QuadPart = startTime.QuadPart - endTime.QuadPart;
		QueryPerformanceCounter(&endTime);
		frameTime.QuadPart += endTime.QuadPart - startTime.QuadPart;
	}

	std::cout << "> End of OpenGL Application!" << std::endl;
	return 0;
}

// ----------------------------------------------------------------------------
//  FUNCTION DEFINITION
// ----------------------------------------------------------------------------
void frameEvent()
{
}

void mouseEvent(int button, int state, int x, int y)
{
}

void mouseMotionEvent(int x, int y)
{
}

void keyboardEvent(unsigned char key, int state)
{
}

void keyboardSpecialEvent(unsigned char key, int state)
{
}

void resizeWindowEvent(int width, int height)
{
}

int createWindow(int width, int height, int x, int y)
{
	int ret;

	WNDCLASSEX windowClass;

	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_DBLCLKS | CS_HREDRAW | CS_OWNDC | CS_VREDRAW;
	windowClass.lpfnWndProc = windowProcedure; // mandatory
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = GetModuleHandle(NULL); // mandatory
	windowClass.hIcon = 0;
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.hbrBackground = 0;
	windowClass.lpszMenuName = 0;
	windowClass.lpszClassName = LPCSTR("GLWNDCLASS"); // mandatory
	windowClass.hIconSm = 0;

	if(!RegisterClassEx(&windowClass))
	{
		DWORD error = GetLastError();
		std::cout << "(!) 1 - Nao foi possivel registrar uma Window Class: " << error << "\n" << std::endl;
		return 0;
	}

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
		LPCSTR("GLWNDCLASS"),
		LPCSTR("OPENGL APP"),
		dwStyle,
		x,
		y,
		windowSize.right - windowSize.left,
		windowSize.bottom - windowSize.top,
		NULL,
		NULL,
		GetModuleHandle(NULL),
		NULL);

	if(hWindow == NULL)
	{
		DWORD error = GetLastError();
		std::cout << "(!) 2 - Nao foi possivel criar a janela: " << error << "\n" << std::endl;

		ret = UnregisterClass(LPCSTR("GLWNDCLASS"), GetModuleHandle(NULL));

		if(ret == 0)
		{
			DWORD error = GetLastError();
			std::cout << "(!) 3 -  Nao foi possivel desregistrar a janela: " << error << "\n" << std::endl;
		}

		return 0;
	}

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
	{
		DWORD error = GetLastError();
		std::cout << "(!) 4 - Nao foi possivel obter device context: " << error << "\n" << std::endl;
		return 0;
	}

	GLuint PixelFormat = ChoosePixelFormat(hDC, &pfd);

	if(!PixelFormat)
	{
		DWORD error = GetLastError();
		std::cout << "(!) 5 - Nao foi possivel obter pixel format: " << error << "\n" << std::endl;

		// Como criamos uma janela com CS_OWNDC (DC específico para a janela), não
		// podemos liberá-lo utilizando ReleaseDC. A questão é quando o DC é
		// liberado pelo Windows.

		ret = ReleaseDC(hWindow, hDC);

		if(!ret)
		{
			DWORD error = GetLastError();
			std::cout << "(!) 6 - Nao foi possivel liberar o device context: " << error << "\n" << std::endl;
		}

		ret = DestroyWindow(hWindow);

		if(ret == 0)
		{
			DWORD error = GetLastError();
			std::cout << "(!) 7 - Nao foi possivel destruir a janela: " << error << "\n" << std::endl;
		}

		ret = UnregisterClass(LPCSTR("GLWNDCLASS"), GetModuleHandle(NULL));

		if(ret == 0)
		{
			DWORD error = GetLastError();
			std::cout << "(!) 8 - Nao foi possivel desregistrar a janela: " << error << "\n" << std::endl;
		}

		return 0;
	}

	ret = SetPixelFormat(hDC, PixelFormat, &pfd);

	if(ret == FALSE)
	{
		DWORD error = GetLastError();
		std::cout << "(!) 9 - Nao foi possivel configurar o pixel format: " << error << "\n" << std::endl;

		// Como criamos uma janela com CS_OWNDC (DC específico para a janela), não
		// podemos liberá-lo utilizando ReleaseDC. A questão é quando o DC é
		// liberado pelo Windows.

		ret = ReleaseDC(hWindow, hDC);

		if(!ret)
		{
			DWORD error = GetLastError();
			std::cout << "(!) 10 - Nao foi possivel liberar o device context: " << error << "\n" << std::endl;
		}

		ret = DestroyWindow(hWindow);

		if(ret == 0)
		{
			DWORD error = GetLastError();
			std::cout << "(!) 11 - Nao foi possivel destruir a janela: " << error << "\n" << std::endl;
		}

		ret = UnregisterClass(LPCSTR("GLWNDCLASS"), GetModuleHandle(NULL));

		if(ret == 0)
		{
			DWORD error = GetLastError();
			std::cout << "(!) 12 - Nao foi possivel desregistrar a janela: " << error << "\n" << std::endl;
		}

		return 0;
	}

	hRC = wglCreateContext(hDC);

	if(hRC == NULL)
	{
		DWORD error = GetLastError();
		std::cout << "(!) 13 - Nao foi possivel criar rendering context: " << error << "\n" << std::endl;

		// Como criamos uma janela com CS_OWNDC (DC específico para a janela), não
		// podemos liberá-lo utilizando ReleaseDC. A questão é quando o DC é
		// liberado pelo Windows.

		ret = ReleaseDC(hWindow, hDC);

		if(!ret)
		{
			DWORD error = GetLastError();
			std::cout << "(!) 14 - Nao foi possivel liberar o device context: " << error << "\n" << std::endl;
		}

		ret = DestroyWindow(hWindow);

		if(ret == 0)
		{
			DWORD error = GetLastError();
			std::cout << "(!) 15 - Nao foi possivel destruir a janela: " << error << "\n" << std::endl;
		}

		ret = UnregisterClass(LPCSTR("GLWNDCLASS"), GetModuleHandle(NULL));

		if(ret == 0)
		{
			DWORD error = GetLastError();
			std::cout << "(!) 16 - Nao foi possivel desregistrar a janela: " << error << "\n" << std::endl;
		}

		return 0;
	}

	ret = wglMakeCurrent(hDC, hRC);

	if(ret == FALSE)
	{
		DWORD error = GetLastError();
		std::cout << "(!) 17 - Nao foi possivel configurar o rendering context: " << error << "\n" << std::endl;

		ret = wglDeleteContext(hRC);

		if(ret == FALSE)
		{
			DWORD error = GetLastError();
			std::cout << "(!) 18 - Nao foi possivel deletar o rendering context: " << error << "\n" << std::endl;
		}

		// Como criamos uma janela com CS_OWNDC (DC específico para a janela), não
		// podemos liberá-lo utilizando ReleaseDC. A questão é quando o DC é
		// liberado pelo Windows.

		ret = ReleaseDC(hWindow, hDC);

		if(!ret)
		{
			DWORD error = GetLastError();
			std::cout << "(!) 19 - Nao foi possivel liberar o device context: " << error << "\n" << std::endl;
		}

		ret = DestroyWindow(hWindow);

		if(ret == 0)
		{
			DWORD error = GetLastError();
			std::cout << "(!) 20 - Nao foi possivel destruir a janela: " << error << "\n" << std::endl;
		}

		ret = UnregisterClass(LPCSTR("GLWNDCLASS"), GetModuleHandle(NULL));

		if(ret == 0)
		{
			DWORD error = GetLastError();
			std::cout << "(!) 21 - Nao foi possivel desregistrar a janela: " << error << "\n" << std::endl;
		}

		return 0;
	}

	return 1;
}

int destroyWindow()
{
	int ret;
	int error = 1;

	ret = wglMakeCurrent(NULL, NULL);

	if(ret == FALSE)
	{
		DWORD error = GetLastError();
		std::cout << "(!) 1 - Nao foi possivel liberar o rendering context: " << error << "\n" << std::endl;
		error = 0;
	}

	ret = wglDeleteContext(hRC);

	if(ret == FALSE)
	{
		DWORD error = GetLastError();
		std::cout << "(!) Nao foi possivel deletar o rendering context: " << error << "\n" << std::endl;
		error = 0;
	}

	// Como criamos uma janela com CS_OWNDC (DC específico para a janela), não
	// podemos liberá-lo utilizando ReleaseDC. A questão é quando o DC é
	// liberado pelo Windows.

	ret = ReleaseDC(hWindow, hDC);

	if(!ret)
	{
		DWORD error = GetLastError();
		std::cout << "(!) Nao foi possivel liberar o device context: " << error << "\n" << std::endl;
		error = 0;
	}

	ret = DestroyWindow(hWindow);

	if(ret == 0)
	{
		DWORD error = GetLastError();
		std::cout << "(!) Nao foi possivel destruir a janela: " << error << "\n" << std::endl;
		error = 0;
	}

	ret = UnregisterClass(LPCSTR("GLWNDCLASS"), GetModuleHandle(NULL));

	if(ret == 0)
	{
		DWORD error = GetLastError();
		std::cout << "(!) Nao foi possivel desregistrar a janela: " << error << "\n" << std::endl;
		error = 0;
	}

	return error;
}


LRESULT CALLBACK windowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		// --------------------------------------------------------------------
		// WINDOW EVENTS
		// --------------------------------------------------------------------
		case WM_CREATE:
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		case WM_MOVE:
			break;

		case WM_SIZE:
			break;

		case WM_CLOSE:
			break;

		// --------------------------------------------------------------------
		// MOUSE EVENTS
		// --------------------------------------------------------------------
		case WM_LBUTTONDOWN:
			mouseEvent(MOUSE_BUTTON_LEFT, 1, LOWORD(lParam), HIWORD(lParam));
			break;

		case WM_LBUTTONUP:
			mouseEvent(MOUSE_BUTTON_LEFT, 0, LOWORD(lParam), HIWORD(lParam));
			break;

		case WM_MBUTTONDOWN:
			mouseEvent(MOUSE_BUTTON_MIDDLE, 1, LOWORD(lParam), HIWORD(lParam));
			break;

		case WM_MBUTTONUP:
			mouseEvent(MOUSE_BUTTON_MIDDLE, 0, LOWORD(lParam), HIWORD(lParam));
			break;

		case WM_RBUTTONDOWN:
			mouseEvent(MOUSE_BUTTON_RIGHT, 1, LOWORD(lParam), HIWORD(lParam));
			break;

		case WM_RBUTTONUP:
			mouseEvent(MOUSE_BUTTON_RIGHT, 0, LOWORD(lParam), HIWORD(lParam));
			break;

		case WM_MOUSEMOVE:
			mouseMotionEvent(LOWORD(lParam), HIWORD(lParam));
			break;

		// --------------------------------------------------------------------
		// KEYBOARDS EVENTS
		// --------------------------------------------------------------------
		case WM_SYSKEYDOWN:
			keyboardSpecialEvent(wParam, 1);
			break;

		case WM_KEYDOWN:
			keyboardEvent(wParam, 1);
			break;

		case WM_SYSKEYUP:
			keyboardSpecialEvent(wParam, 0);
			break;

		case WM_KEYUP:
			keyboardEvent(wParam, 0);
			break;

		default:
			break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}