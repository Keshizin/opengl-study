// ExemploPontos2D.cpp
// ExemploLinhas2D.cpp
// ExemploPoligono2D.cpp
// ExemploTriangulos2D.cpp
// ExemploTriangulos2DStrip.cpp
// ExemploTriangulos2DFan.cpp
// ExemploQuad2D.cpp
// ExemploQuad2DStrip.cpp
// TriangulosColoridos.cpp
// QuadradoColorido.cpp
// Linhas.cpp
// PadroesDePoligonos.cpp
// ExemploGLUT-texto.cpp
// ExemploCurva2D.cpp
// CasaComTranslacao.cpp
// CasaComEscala.cpp
// CasaComRotacao.cpp
// CasaComTransRotEsc.cpp
// CasaComRotTransEsc.cpp
// CasaEscopoTransforms.cpp
// Exemplo3DComTransformacoes.cpp
// ObjetoComTransformacoesHierarquicas.cpp
// CasaComAnimacao.cpp
// Exemplo3DComAnimacao.cpp
// CasaComZoom.cpp
// Exemplo3DComZoom.cpp
// Exemplo3DOrtoZoom.cpp
// CasaComZoomEPan.cpp
// Exemplo3DComZoomEPan.cpp
// ExemploBitmap.cpp
// ExemploImagem.cpp
// ExemploImagemCopia.cpp
// ExemploImagemTransfer.cpp
// ExemploImagemZoom.cpp

#include <windows.h>
#include <mmsystem.h>

#include <GL/gl.h>
#include <GL/glu.h>
// #include <wglext.h>
#include <glext.h>

#include <iostream>
#include <string>
#include <math.h>
// #include <iomanip>

#include <eventhandler.h>
#include <diblib.h>
#include <obj.h>

// ----------------------------------------------------------------------------
//  SYMBOLIC CONSTANTS
// ----------------------------------------------------------------------------
#define CONTEXT_2D 1
#define CONTEXT_3D 2

#define MOUSE_BUTTON_LEFT   1
#define MOUSE_BUTTON_MIDDLE 2
#define MOUSE_BUTTON_RIGHT  3

// ----------------------------------------------------------------------------
//  GLOBAL VARIABLES
// ----------------------------------------------------------------------------
int global_rendering_context = CONTEXT_2D;

int global_window_width  = 640;
int global_window_height = 480;

GLdouble global_world_left   = -50;
GLdouble global_world_right  =  50;
GLdouble global_world_top    =  50;
GLdouble global_world_bottom = -50;

GLdouble global_aspect_correction;

bool global_aspect_correction_state = false;

GLfloat camX = 0.0f;
GLfloat camY = 0.0f;
GLfloat camZ = 0.0f;

GLdouble projection_angle = 60.0;
GLdouble projection_zNear = 1;
GLdouble projection_zFar = 30;

HGLRC hRC = NULL;
HWND hWindow = NULL;
HDC hDC = NULL;
LARGE_INTEGER frequency;

// ----------------------------------------------------------------------------
//  APPLICATION GLOBAL VARIABLES
// ----------------------------------------------------------------------------
GLfloat spin = 0.0;

// ----------------------------------------------------------------------------
//  FUNCTION PROTOTYPE DECLARATION
// ----------------------------------------------------------------------------
LRESULT CALLBACK windowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int createWindow(int width, int height, int x, int y);
int destroyWindow();

void frameEvent(unsigned long long frameTime);
void mouseEvent(int button, int state, int x, int y);
void mouseMotionEvent(int x, int y);
void keyboardEvent(unsigned char key, int state);
void keyboardSpecialEvent(unsigned char key, int state);
void resizeWindowEvent(int width, int height);
void initGL();

void setProjection();

// ----------------------------------------------------------------------------
//  MAIN FUNCTION
// ----------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// test.loadFile("assets/24bpp_test.bmp", true);

	int ret;

	std::cout << "> Welcome to OpenGL Application" << std::endl;

	ret = createWindow(global_window_width, global_window_height, 5, 5);
	std::cout << "> window created: " << ret << std::endl;
	ret = ShowWindow(hWindow, SW_SHOW);
	std::cout << "> show window: " << ret << std::endl;

	MSG msg;
	bool isDone = false;

	LARGE_INTEGER frameTime;
	LARGE_INTEGER startTime;
	LARGE_INTEGER endTime;
	unsigned long long timer = 0;
	unsigned long long fps = 0;

	typedef BOOL(APIENTRY *PFNWGLSWAPINTERVALPROC)(int);
	PFNWGLSWAPINTERVALPROC wglSwapIntervalEXT = NULL;

	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALPROC) wglGetProcAddress("wglSwapIntervalEXT");

	if (wglSwapIntervalEXT) {
	    wglSwapIntervalEXT(0);
	}

	initGL();

	QueryPerformanceCounter(&endTime);

	while(!isDone)
	{
		// --------------------------------------------------------------------
		// START OF FRAME
		// --------------------------------------------------------------------
		QueryPerformanceCounter(&startTime);
		timer += frameTime.QuadPart;

		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{
				std::cout << "> WM_QUIT message" << std::endl;
				isDone = true;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// --------------------------------------------------------------------
		// RENDERING STUFF HERE
		// --------------------------------------------------------------------
		frameEvent(frameTime.QuadPart);
		SwapBuffers(hDC);

		QueryPerformanceFrequency(&frequency);
		
		if(timer >= frequency.QuadPart)
		{
			std::cout << "> FPS: " << fps << std::endl;
			fps = 0;
			timer = 0;
		}

		fps++;
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

// GLubyte img[400 * 400 * 3] = {0};

// ----------------------------------------------------------------------------
//  FUNCTION DEFINITION
// ----------------------------------------------------------------------------
void initGL()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
}

void frameEvent(unsigned long long frameTime)
{
	// GLfloat frameTimeSec = static_cast<double>(frameTime) / static_cast<double>(frequency.QuadPart);
	// std::cout << "frameTime: " << frameTime << std::endl;
	// std::cout << "frameTimeSec: " << frameTimeSec << std::endl;
	// std::cout << "spin: " << spin << std::endl;
	// std::cout << "spin add: " << spin + (2.0 * frameTimeSec) << std::endl << std::endl;

	// spin = spin + (2.0 * frameTimeSec);
	spin = spin + (2.0);

	if(spin > 360.0)
		spin = spin - 360.0;

	glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();
	glRotatef(spin, 0.0, 0.0, 1.0);
	glColor3f(1.0, 1.0, 1.0);
	glRectf(-25.0, -25.0, 25.0, 25.0);
	glPopMatrix();
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
	global_window_width = width;
	global_window_height = height;

	glViewport(0, 0, global_window_width, global_window_height);
	setProjection();

	// VISUALIZAÇÃO 3D
	// fAspect = (GLfloat)width / (GLfloat)height;
	// gluPerspective(angle, fAspect, 0.1, 1200);
	// glOrtho(-win, win, -win, win, -200.0, 200.0);
	
	// glMatrixMode(GL_MODELVIEW);
	// glLoadIdentity();
	// gluLookAt(0+deslocamentoX,0+deslocamentoY,150+deslocamentoZ,0+deslocamentoX,0+deslocamentoY,0+deslocamentoZ, 0,1,0);

	// glMatrixMode(GL_MODELVIEW);
	// Inicializa sistema de coordenadas do modelo
	// glLoadIdentity();
	// Posiciona e orienta o observador
	// glTranslatef(-obsX,-obsY,-obsZ);
	// glRotatef(rotX,1,0,0);
	// glRotatef(rotY,0,1,0);
}

void setProjection()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if(global_rendering_context == CONTEXT_2D)
	{
		GLdouble left = global_world_left;
		GLdouble right = global_world_right;
		GLdouble bottom = global_world_bottom;
		GLdouble top = global_world_top;

		if(global_aspect_correction_state)
		{
			if(global_window_width <= global_window_height)
			{
				global_aspect_correction = GLdouble(global_window_height) / GLdouble(global_window_width);
				bottom *= global_aspect_correction;
				top *= global_aspect_correction;
			}
			else
			{
				global_aspect_correction = GLdouble(global_window_width) / GLdouble(global_window_height);
				left *= global_aspect_correction;
				right *= global_aspect_correction;
			}
		}

		// std::cout << "(2D) global aspect correction: " << global_aspect_correction << std::endl;
		// std::cout << "(2D) global word left: " << left << std::endl;
		// std::cout << "(2D) global word right: " << right << std::endl;
		// std::cout << "(2D) global word bottom: " << bottom << std::endl;
		// std::cout << "(2D) global word top: " << top << std::endl;

		//gluOrtho2D(-win + deslocamentoX, win + deslocamentoX, -win * height / width + deslocamentoY, win * height / width + deslocamentoY);
		gluOrtho2D(left, right, bottom, top);
	}
	else if(global_rendering_context == CONTEXT_3D)
	{
		global_aspect_correction = GLdouble(global_window_width) / GLdouble(global_window_height);

		// std::cout << "(3D) global aspect correction: " << global_aspect_correction << std::endl;
		// std::cout << "(3D) global projection angle: " << projection_angle << std::endl;
		// std::cout << "(3D) global projection zNear: " << projection_zNear << std::endl;
		// std::cout << "(3D) global projection zFar: " << projection_zFar << std::endl;

		gluPerspective(projection_angle, global_aspect_correction, projection_zNear, projection_zFar);
	}
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
		// 32, // cDepthBits
		0, // cDepthBits
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
		std::cout << "(!) Nao foi possivel liberar o rendering context: " << error << "\n" << std::endl;
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
	int ret;

	switch(uMsg)
	{
		// --------------------------------------------------------------------
		// WINDOW EVENTS
		// --------------------------------------------------------------------
		case WM_CREATE:
			// std::cout << "> WM_CREATE message" << std::endl;
			break;

		case WM_DESTROY:
			// std::cout << "> WM_DESTROY message" << std::endl;
			PostQuitMessage(0);
			break;

		case WM_MOVE:
			break;

		case WM_SIZE:
			// std::cout << "> WM_SIZE message | x: " << LOWORD(lParam) << " y: " << HIWORD(lParam) << std::endl;
			resizeWindowEvent(LOWORD(lParam), HIWORD(lParam));
			break;

		case WM_CLOSE:
			// std::cout << "> WM_CLOSE message" << std::endl;
			destroyWindow();
			break;

		case WM_ACTIVATE:
			// std::cout << "> WM_ACTIVATE message | wParam: " << LOWORD(wParam) <<std::endl;
			break;

		case WM_SHOWWINDOW:
			// std::cout << "> WM_SHOWWINDOW message | wParam: " << wParam << " lParam: " << lParam << std::endl;
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