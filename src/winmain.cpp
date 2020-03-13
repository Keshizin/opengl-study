#include <windows.h>
#include <iostream>
#include <iomanip>
#include <GL/gl.h>
#include <GL/glu.h>

#include <gewin.h>
#include <geapiwrapper.h>
#include <geevthandler.h>

#define XMD_H	
#define HAVE_BOOLEAN

#define WINDOW_WIDTH  600
#define WINDOW_HEIGHT 600

extern "C" {
	#include <jpeg/jpeglib.h>
}

#include <myres.h>

class UserEventHandler : public GEEventHandler
{
public:
	void mouseEvent(int button, int state, int x, int y)
	{
		std::cout << "> Mouse Event"
			<< "\n\tbutton: " << button
			<< "\n\tstate: " << state
			<< "\n\tx: " << x
			<< "\n\ty: " << y
			<< "\n" << std::endl;
	}

	void mouseMotionEvent(int x, int y)
	{
		std::cout << "> Mouse Motion Event"
			<< "\n\tx: " << x
			<< "\n\ty: " << y
			<< "\n" << std::endl;
	}
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

void resizeGLWindow(int width, int height);
void initGL();
void drawGLWindow(HDC hDC);

TEX *LoadJPG(const char *filename, bool inverte = true);
void DecodeJPG(jpeg_decompress_struct* cinfo, TEX *pImageData, bool inverte);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	int ret;
	bool isDone = false;

	UserEventHandler *userEventHandler = new UserEventHandler();

	GEApiWrapper *apiWrapper = new GEApiWrapper();
	apiWrapper->setEventHandler(userEventHandler);

	GEWindow myWindow(apiWrapper);
	myWindow.setWindowName("BPM GAME ENGINE");
	myWindow.setWidth(640);
	myWindow.setHeight(480);

	myWindow.createWindow();

	initGL();

	// -------------------------------------------------------------------------
	// MAIN LOOP
	// -------------------------------------------------------------------------
	unsigned int currentTime;
	unsigned int frameTime;
	unsigned int lastTime = 0;
	unsigned int framePerSecond;
	unsigned int frameCounter = 0;
	unsigned int timeAccum = 0;
	std::cout << "current time: " << currentTime << std::endl;

	// Main Loop
	while(!isDone)
	{
		// while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		// {
		// 	if(msg.message == WM_QUIT)
		// 	{
		// 		myWindow.destroyWindow();
		// 		isDone = true;
		// 	}
		// 	std::cout << "> PeekMessage" << std::endl;
		// 	TranslateMessage(&msg);
		// 	DispatchMessage(&msg);
		// }

		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{
				myWindow.destroyWindow();
				isDone = true;
			}

			//std::cout << "> PeekMessage" << std::endl;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		do
		{
			currentTime = GetTickCount();
			frameTime = (currentTime > lastTime) ? currentTime - lastTime : 0;
			lastTime = currentTime >= lastTime ? lastTime : currentTime;
		} while(!(frameTime >= 5));

		timeAccum += frameTime;
		frameCounter++;

		if(timeAccum >= 1000)
		{
			framePerSecond = frameCounter;
			frameCounter = 0;
			timeAccum = 0;

			std::cout << "FPS: " << framePerSecond << " - frame time: " << frameTime << std::endl;
		}

		lastTime = currentTime;
		drawGLWindow(apiWrapper->getHDC());
	}

	delete userEventHandler;
	delete apiWrapper;

	std::cout << ">   END OF APPLICATION!" << std::endl;
	return 0;
}

void resizeGLWindow(int width, int height)
{
	std::cout
		<< "CALLBACK - resizeGLWindow"
		<< "\n   -> width: " << width
		<< "\n   -> height: " << height
		<< std::endl;

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluOrtho2D(0, width, 0, height);
	gluOrtho2D(0, width, 0, height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
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

void drawGLWindow(HDC hDC)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(0.9f, 0.4f, 0.018f);
	//glRasterPos2i(0, 0);
	//glDrawPixels(globalImg->dimx, globalImg->dimy, GL_RGB, GL_UNSIGNED_BYTE, globalImg->data);
	SwapBuffers(hDC);
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