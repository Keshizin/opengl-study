#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <wglext.h>

#include <iostream>
#include <string>
#include <math.h>
// #include <iomanip>

#include <eventhandler.h>
#include <diblib.h>

// #include <jni.h>

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

GLfloat angle, fAspect;
GLfloat deslocamentoX, deslocamentoY, deslocamentoZ;
GLfloat win;
int w;
int h;

DIB test;

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

void initGL();

// ----------------------------------------------------------------------------
//  MAIN FUNCTION
// ----------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	test.loadFile("assets/24bpp_test.bmp");
	// char path[200]="-Djava.class.path=src;lib\\camunda-bpmn-model-7.13.0-alpha3.jar;lib\\camunda-xml-model-7.13.0-alpha3.jar;";

	// JavaVM *jvm;
	// JNIEnv *env;
	// JavaVMInitArgs vm_args;

	// JavaVMOption jvmopt[1];
	// jvmopt[0].optionString = path;

	// vm_args.version = JNI_VERSION_1_2;
	// vm_args.nOptions = 1;
	// vm_args.options = jvmopt;
	// vm_args.ignoreUnrecognized = JNI_TRUE;

	// jint rc = JNI_CreateJavaVM(&jvm, (void**)&env, &vm_args);
 
	// if (rc != JNI_OK)
	// {
	// 	std::cout << "(!) ERROR It was not possible to create JVM." << std::endl;
	// 	return 0;
	// }

	// jclass jcls = env->FindClass("BPMNParser");

	// if(jcls == NULL)
	// {
	// 	std::cout << "(!) Java class not found." << std::endl;
	// 	env->ExceptionDescribe();
	// 	jvm->DestroyJavaVM();
	// 	return 0;
	// }

 //    jmethodID methodId = env->GetStaticMethodID(jcls, "startBPMNParser", "()V");

	// if(methodId == NULL)
	// 	std::cout << "(!) ERROR Method is not found!" << std::endl;
	// else
	// {
	// 	env->CallStaticVoidMethod(jcls, methodId);

	// 	if (env->ExceptionCheck()) {
	// 		env->ExceptionDescribe();
	// 		env->ExceptionClear();
	// 	}
	// }

	// jvm->DestroyJavaVM();

	int ret;

	std::cout << "> Welcome to OpenGL Application" << std::endl;

	//ret = createWindow(450, 450, 5, 5);
	ret = createWindow(1016, 800, 5, 5);
	std::cout << "> window created: " << ret << std::endl;

	ret = ShowWindow(hWindow, SW_SHOW);
	std::cout << "> show window: " << ret << std::endl;

	MSG msg;
	bool isDone = false;

	LARGE_INTEGER frameTime;
	LARGE_INTEGER startTime;
	LARGE_INTEGER endTime;
	LARGE_INTEGER frequency;
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
		frameEvent();
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

GLubyte img[400 * 400 * 3] = {0};

// ----------------------------------------------------------------------------
//  FUNCTION DEFINITION
// ----------------------------------------------------------------------------
void frameEvent()
{
	// Limpa a janela de visualização com a cor  
	// de fundo definida previamente
	glClear(GL_COLOR_BUFFER_BIT);

	// Altera a cor do desenho para preto
	glColor3f(1.0f, 0.0f, 0.0f);

	// Ajusta a posição inicial de desenho do bitmap
	glRasterPos2i(0, 0);

	// Desenha 10 cópias
	// for(int i=0;i<10;++i)
	// 	//glBitmap(16, 16, 0.0, 0.0, 16.0, 16.0, bitmap);
	
	//glBitmap(test.getWidth(), test.getHeight(), 0.0, 0.0, test.getWidth(), test.getHeight(), test.getColorIndex());
	glDrawPixels(test.getWidth(), test.getHeight(), GL_RGB, GL_UNSIGNED_BYTE, test.getColorIndex());
	glRasterPos2i(100, 100);
	glCopyPixels(0, 0, 400, 400, GL_COLOR);
}

void mouseEvent(int button, int state, int x, int y)
{
	if (button == 1 && state == 0) {  // zoom in
			if(angle >= 10)
				angle -= 5;
	}
	
	if (button == 3 && state == 0) { // zoom out
			if(angle <= 130)
				angle += 5;
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// VISUALIZAÇÃO 3D
	//gluPerspective(angle, fAspect,0.5,500);
	// glOrtho(-win, win, -win, win, -200.0, 200.0);
	// glMatrixMode(GL_MODELVIEW);
	// glLoadIdentity();
	// gluLookAt(0+deslocamentoX,0+deslocamentoY,150+deslocamentoZ,0+deslocamentoX,0+deslocamentoY,0+deslocamentoZ, 0,1,0);
	if(w <= h)
	{
		gluOrtho2D(-win + deslocamentoX, win + deslocamentoX, -win * h / w + deslocamentoY, win * h / w + deslocamentoY);
	}
	else
	{
		gluOrtho2D(-win * w / h + deslocamentoX, win * w / h + deslocamentoX, -win + deslocamentoY, win + deslocamentoY);
	}
}

void mouseMotionEvent(int x, int y)
{
}

void keyboardEvent(unsigned char key, int state)
{
	if(key == '1' && state == 0)
	{
		deslocamentoY += 2;	
	}

	if(key == '2' && state == 0)
	{
		deslocamentoY -= 2;	
	}

	if(key == '3' && state == 0)
	{
		deslocamentoX -= 2;	
	}

	if(key == '4' && state == 0)
	{
		deslocamentoX += 2;	
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// VISUALIZAÇÃO 3D
	// gluPerspective(angle, fAspect,0.5,500);
	// glOrtho(-win, win, -win, win, -200.0, 200.0);
	// glMatrixMode(GL_MODELVIEW);
	// glLoadIdentity();
	// gluLookAt(0+deslocamentoX,0+deslocamentoY,150+deslocamentoZ,0+deslocamentoX,0+deslocamentoY,0+deslocamentoZ, 0,1,0);

	// VISUALIZAÇÃO 2D
	if(w <= h)
	{
		gluOrtho2D(-win + deslocamentoX, win + deslocamentoX, -win * h / w + deslocamentoY, win * h / w + deslocamentoY);
	}
	else
	{
		gluOrtho2D(-win * w / h + deslocamentoX, win * w / h + deslocamentoX, -win + deslocamentoY, win + deslocamentoY);
	}
}

void keyboardSpecialEvent(unsigned char key, int state)
{
}

void resizeWindowEvent(int width, int height)
{
	w = width;
	h = height;

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// VISUALIZAÇÃO 3D
	// fAspect = (GLfloat)width / (GLfloat)height;
	// gluPerspective(angle, fAspect,0.5,500);
	// glOrtho(-win, win, -win, win, -200.0, 200.0);
	// glMatrixMode(GL_MODELVIEW);
	// glLoadIdentity();
	// gluLookAt(0+deslocamentoX,0+deslocamentoY,150+deslocamentoZ,0+deslocamentoX,0+deslocamentoY,0+deslocamentoZ, 0,1,0);

	// VISUALIZAÇÃO 2D
	// if(width <= height)
	// {
	// 	gluOrtho2D(-win + deslocamentoX, win + deslocamentoX, -win * height / width + deslocamentoY, win * height / width + deslocamentoY);
	// }
	// else
	// {
	// 	gluOrtho2D(-win * width / height + deslocamentoX, win * width / height + deslocamentoX, -win + deslocamentoY, win + deslocamentoY);
	// }

	gluOrtho2D(0, width, 0, height);

}

void initGL()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	win = 5.0f;
	glPixelStorei (GL_UNPACK_ALIGNMENT, 4);
	// glPixelStorei (GL_UNPACK_SWAP_BYTES, GL_TRUE);

	angle=45;

	// Inicializa as variáveis utilizadas para implementação
	// da operação de pan
	deslocamentoX = 0.0f;
	deslocamentoY = 0.0f;
	deslocamentoZ = 0.0f;
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
			std::cout << "> WM_CREATE message" << std::endl;
			break;

		case WM_DESTROY:
			std::cout << "> WM_DESTROY message" << std::endl;
			PostQuitMessage(0);
			break;

		case WM_MOVE:
			break;

		case WM_SIZE:
			std::cout << "> WM_SIZE message | x: " << LOWORD(lParam) << " y: " << HIWORD(lParam) << std::endl;
			resizeWindowEvent(LOWORD(lParam), HIWORD(lParam));
			break;

		case WM_CLOSE:
			std::cout << "> WM_CLOSE message" << std::endl;
			destroyWindow();
			break;

		case WM_ACTIVATE:
			std::cout << "> WM_ACTIVATE message | wParam: " << LOWORD(wParam) <<std::endl;
			break;

		case WM_SHOWWINDOW:
			std::cout << "> WM_SHOWWINDOW message | wParam: " << wParam << " lParam: " << lParam << std::endl;
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