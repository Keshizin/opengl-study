#include <windows.h>
#include <iostream>
#include <iomanip>
#include <GL/gl.h>
#include <GL/glu.h>

#include <gewin.h>
#include <gewinapiwrapper.h>
#include <geevthandler.h>


#define XMD_H	
#define HAVE_BOOLEAN

#define WINDOW_WIDTH  600
#define WINDOW_HEIGHT 600
#define FRAME_RATE_GOVERNING 1

extern "C" {
	#include <jpeg/jpeglib.h>
}

#include <myres.h>

class UserEventHandler : public GEEventHandler
{
public:
	void drawFrame()
	{
		//std::cout << "> Draw Frame Event\n" << std::endl;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// glColor3f(0.9f, 0.4f, 0.018f);
		// glRasterPos2i(0, 0);
		// glDrawPixels(globalImg->dimx, globalImg->dimy, GL_RGB, GL_UNSIGNED_BYTE, globalImg->data);
		// Specific for rendering context type
		SwapBuffers(hDC);
	}

	void mouseEvent(int button, int state, int x, int y)
	{
		std::cout << "> Mouse Event"
			<< "\n\tbutton: " << button
			<< "\n\t state: " << state
			<< "\n\t     x: " << x
			<< "\n\t     y: " << y
			<< "\n" << std::endl;
	}

	void mouseMotionEvent(int x, int y)
	{
		std::cout << "> Mouse Motion Event"
			<< "\n\tx: " << x
			<< "\n\ty: " << y
			<< "\n" << std::endl;
	}

	void resizeWindow(int width, int height)
	{
		std::cout << "> Resize Window Event"
			<< "\n\twidth: " << width
			<< "\n\theight: " << height
			<< "\n" << std::endl;

		// user configurations
		glViewport(0, 0, width, height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0, width, 0, height);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	void setDC(HDC hDC)
	{
		this->hDC = hDC;
	}

private:
	HDC hDC;
};

// Define a estrutura de uma imagem
typedef struct
{
	char nome[50];			// nome do arquivo carregado
	int ncomp;				// número de componentes na textura (1-intensidade, 3-RGB)
	GLint dimx;				// largura 
	GLint dimy;				// altura
	GLuint texid;			// identifição da textura em OpenGL
	unsigned char *data;	// apontador para a imagem em si
} TEX;

TEX *globalImg;

void initGL();

TEX *LoadJPG(const char *filename, bool inverte = true);
void DecodeJPG(jpeg_decompress_struct* cinfo, TEX *pImageData, bool inverte);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	std::cout << "> Starting application" << std::endl;
	UserEventHandler *userEventHandler = new UserEventHandler();

	GEWinApiWrapper *winApiWrapper = new GEWinApiWrapper();
	winApiWrapper->setEventHandler(userEventHandler);
	winApiWrapper->setWindowClassName("MYWINDOWCLASS");
	GEApiWrapper *apiWrapper = winApiWrapper;

	GEWindow myWindow(apiWrapper);
	myWindow.setWindowName("BPM GAME ENGINE");
	myWindow.setWidth(640);
	myWindow.setHeight(480);

	if(myWindow.createWindow())
		std::cout << "> Window created with success!" << std::endl;
	else
		std::cout << "(!) Fail to create window!" << std::endl;

	userEventHandler->setDC(winApiWrapper->getDC());
	
	myWindow.showWindow();

	MSG msg;
	// int ret;
	bool isDone = false;

	initGL();

	// // -------------------------------------------------------------------------
	// // MAIN LOOP
	// // -------------------------------------------------------------------------
	// Possibilidade de deixar livre ou fixar o tempo (em ms) de cada quado
	// - variable frame rate
	// - frame-rate governing
	unsigned long long frameTime; // microseconds
	unsigned int numberOfFrames = 0;
	LARGE_INTEGER startTime;
	LARGE_INTEGER endTime;
	LARGE_INTEGER frequency;
	LARGE_INTEGER timer;
	LARGE_INTEGER sleepTimer;
	// -------------------------------------------------------------------------
	//unsigned long long timer = 0;

	// unsigned int currentTime;
	// unsigned int lastTime = 0;
	// unsigned int framePerSecond;
	// unsigned int timeAccum = 0;
	timer.QuadPart = 0;
	sleepTimer.QuadPart = 0;

	// MAIN LOOP
	while(!isDone)
	{
		// READ HIGH-RESOLUTION TIMER
		QueryPerformanceCounter(&startTime);

		// if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{
				myWindow.destroyWindow();
				isDone = true;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// GAME LOOP
		// if (FRAME_RATE_GOVERNING == 1)
		// {
		// 	// SLEEP MODE - FIX TO 60 FPS (Frequency / 60)

		// 	do
		// 	{
		// 		sleepTimer.QuadPart += endTime.QuadPart - startTime.QuadPart;
		// 	} while(sleepTimer.QuadPart < frequency.QuadPart / 60);
		// 	sleepTimer.QuadPart = 0;
		// }

		numberOfFrames++;
		apiWrapper->getEventHandler()->drawFrame();
		// drawGLWindow(apiWrapper->getHDC());

		// READ HIGH-RESOLUTION TIMER
		QueryPerformanceCounter(&endTime);
		QueryPerformanceFrequency(&frequency);

		// CALCULATE FRAME TIME
		// Guard against loss-of-precision: convert to microseconds before dividing by ticks per second
		// 1 seconds -> 1000 milliseconds -> microseconds = 1000000
		timer.QuadPart += endTime.QuadPart - startTime.QuadPart;

		// std::cout << "> frame time (ticks): " << endTime.QuadPart - startTime.QuadPart << std::endl;
		// std::cout << "> timer: " << timer.QuadPart << std::endl;

		if(timer.QuadPart > frequency.QuadPart)
		{
			std::cout << " > FRAME STATUS"
				<< "\n\tFPS: " << numberOfFrames
				<< "\n\tFRAME TIME: " << endTime.QuadPart - startTime.QuadPart
				<< "\n" << std::endl;

			numberOfFrames = 0;
			timer.QuadPart = 0;
		}
	}

	delete userEventHandler;
	delete winApiWrapper;

	std::cout << "> End of application!" << std::endl;
	return 0;
}

void initGL()
{
	globalImg = LoadJPG("assets/logo.jpg");

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(0.133f, 0.161f, 0.173f, 1.0f);
	//glClearColor(0.09f, 0.4f, 0.018f, 1.0f); // logo
	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
}

TEX *LoadJPG(const char *filename, bool inverte)
{
	struct jpeg_decompress_struct cinfo;
	TEX *pImageData = NULL;
	FILE *pFile;

	if((pFile = fopen(filename, "rb")) == NULL) 
	{
		// Exibe uma mensagem de erro avisando que o arquivo não foi encontrado
		// e retorna NULL
		printf("Impossível carregar arquivo JPG: %s\n",filename);
		return NULL;
	}

	// Cria um gerenciado de erro
	jpeg_error_mgr jerr;

	// Objeto com informações de compactação para o endereço do gerenciador de erro
	cinfo.err = jpeg_std_error(&jerr);

	// Inicializa o objeto de decompactação
	jpeg_create_decompress(&cinfo);

	
	// Especifica a origem dos dados (apontador para o arquivo)	
	jpeg_stdio_src(&cinfo, pFile);
	
	// Aloca a estrutura que conterá os dados jpeg
	pImageData = (TEX*)malloc(sizeof(TEX));

	// Decodifica o arquivo JPG e preenche a estrutura de dados da imagem
	DecodeJPG(&cinfo, pImageData, inverte);
	
	// Libera a memória alocada para leitura e decodificação do arquivo JPG
	jpeg_destroy_decompress(&cinfo);
	
	// Fecha o arquivo 
	fclose(pFile);

	// Retorna os dados JPG (esta memória deve ser liberada depois de usada)
	
	return pImageData;
}

void DecodeJPG(jpeg_decompress_struct* cinfo, TEX *pImageData, bool inverte)
{
	// Lê o cabeçalho de um arquivo jpeg
	jpeg_read_header(cinfo, TRUE);
	
	// Começa a descompactar um arquivo jpeg com a informação 
	// obtida do cabeçalho
	jpeg_start_decompress(cinfo);

	// Pega as dimensões da imagem e varre as linhas para ler os dados do pixel
	pImageData->ncomp = cinfo->num_components;
	pImageData->dimx  = cinfo->image_width;
	pImageData->dimy  = cinfo->image_height;

	int rowSpan = pImageData->ncomp * pImageData->dimx;
	// Aloca memória para o buffer do pixel
	pImageData->data = new unsigned char[rowSpan * pImageData->dimy];
		
	// Aqui se usa a variável de estado da biblioteca cinfo.output_scanline 
	// como o contador de loop
	
	// Cria um array de apontadores para linhas
	int height = pImageData->dimy - 1;
	unsigned char** rowPtr = new unsigned char*[pImageData->dimy];
	if(inverte)
		for (int i = 0; i <= height; i++)
			rowPtr[height - i] = &(pImageData->data[i*rowSpan]);
	else
		for (int i = 0; i <= height; i++)
			rowPtr[i] = &(pImageData->data[i*rowSpan]);

	// Aqui se extrai todos os dados de todos os pixels
	int rowsRead = 0;
	while (cinfo->output_scanline < cinfo->output_height) 
	{
		// Lê a linha corrente de pixels e incrementa o contador de linhas lidas
		rowsRead += jpeg_read_scanlines(cinfo, &rowPtr[rowsRead], cinfo->output_height - rowsRead);
	}
	
	// Libera a memória
	delete [] rowPtr;

	// Termina a decompactação dos dados 
	jpeg_finish_decompress(cinfo);
}