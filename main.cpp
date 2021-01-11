#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include <ge.h>
#include <gemath.h>

#include <iostream>

#define GAME_WINDOW_WIDTH 640
#define GAME_WINDOW_HEIGHT 480

#define WINDOW_LEFT   -50.0
#define WINDOW_RIGHT   50.0
#define WINDOW_BOTTOM -50.0
#define WINDOW_TOP     50.0

class GLEventHandler : public GEEventHandler
{
	void frameEvent();
	void mouseEvent(int button, int state, int x, int y);
	void mouseMotionEvent(int x, int y);
	void keyboardEvent(unsigned char key, int state);
	void keyboardSpecialEvent(unsigned char key, int state);
	void resizeWindowEvent(int width, int height);
	void finishAfterEvent();
	void finishBeforeEvent();
	void resumeEvent();
	void pauseEvent();
	void beforeMainLoopEvent();
	void createWindowEvent();
};

KEngine *engine = 0;
GETimer *timer = 0;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	GLEventHandler gameEvents;
	engine = new KEngine(&gameEvents);
	timer = new GETimer(engine->getTimeHandler());

	// Setting up Game Window
	engine->getGameWindow()->setName("K-Engine!");
	engine->getGameWindow()->setWidth(GAME_WINDOW_WIDTH);
	engine->getGameWindow()->setHeight(GAME_WINDOW_HEIGHT);
	engine->getGameWindow()->setX(960 - (GAME_WINDOW_WIDTH / 2));
	engine->getGameWindow()->setY(540 - (GAME_WINDOW_HEIGHT / 2));
	engine->getGameWindow()->setStyle(K_WINDOW_COMPLETE);
	engine->getGameWindow()->create();

	// Setting up Rendering Engine
	engine->getRenderingSystem()->initialize();
	engine->getRenderingSystem()->setRenderingContext(K_CONTEXT_2D);
	engine->getRenderingSystem()->setWindow(WINDOW_LEFT, WINDOW_RIGHT, WINDOW_BOTTOM, WINDOW_TOP);
	engine->getRenderingSystem()->setViewport(0, 0, GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT);
	engine->getRenderingSystem()->setProjection();
	engine->getRenderingSystem()->setVSync(0);

	engine->setFrameRate(120);

	engine->getGameWindow()->show(nCmdShow);

	timer->setTimerInMs(1000);
	engine->startMainLoop();

	delete engine;
	delete timer;
	return 1;
}

GLfloat tx = 0.0f;
GLfloat ty = 0.0f;
GLfloat tz = 0.0f;

GLfloat sx = 1.0f;
GLfloat sy = 1.0f;
GLfloat sz = 1.0f;

GLfloat offset_x = 0.05f;
GLfloat offset_y = 0.05f;

GLfloat angleRad = 0.0f;
int points = 0;

void GLEventHandler::frameEvent()
{
	// ------------------------------------------------------------------------
	// SCALE TEST
	// ------------------------------------------------------------------------
	// sx += offset_x;
	// sy += offset_y;

	// if(sy > 2.0f || sy < 1.0f)
	// 	offset_y = -offset_y;

	// if(sy > 2.0f || sy < 1.0f)
	// 	offset_x = -offset_x;

	// ------------------------------------------------------------------------
	// TRANSLATE TEST
	// ------------------------------------------------------------------------
	if(timer->isDoneLoop())
	{
		tx = 25 * cos(angleRad);
		ty = 25 * sin(angleRad);

		points++;
		angleRad = 2 * K_PI * points / 200;

		if(points >= 200)
		{
			points = 0;
		}

		std::cout << "tx: " << tx << std::endl;
		std::cout << "ty: " << ty << std::endl << std::endl;

	}

	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(tx, ty, tz);
	// glTranslatef(1.0f, 0.0f, 0.0f);
	// glScalef(sx, sy, sz);

	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	glVertex3f(-20.5f,  20.5f, 0.0f);
	glVertex3f( 20.5f,  20.5f, 0.0f);
	glVertex3f( 20.5f, -20.5f, 0.0f);
	glVertex3f(-20.5f, -20.5f, 0.0f);
	glEnd();

}

void GLEventHandler::mouseEvent(int button, int state, int x, int y)
{
}

void GLEventHandler::mouseMotionEvent(int x, int y)
{
}

void GLEventHandler::keyboardEvent(unsigned char key, int state)
{
	if(key == 27 && state == 1)
	{
		engine->getGameWindow()->destroy();
	}

	if(key == '1' && state == 1)
	{
		timer->startLoop(0);
	}
}

void GLEventHandler::keyboardSpecialEvent(unsigned char key, int state)
{
}

void GLEventHandler::resizeWindowEvent(int width, int height)
{
	engine->getRenderingSystem()->setViewport(0, 0, width, height);
	engine->getRenderingSystem()->setProjection();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void GLEventHandler::finishAfterEvent()
{
	engine->stopMainLoop();
}

void GLEventHandler::finishBeforeEvent()
{
	engine->getGameWindow()->destroy();
}

void GLEventHandler::resumeEvent()
{
}

void GLEventHandler::pauseEvent()
{
}

void GLEventHandler::beforeMainLoopEvent()
{
	glClearColor(247.0f / 255.0f, 194.0f / 255.0f, 23.0f / 255.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void GLEventHandler::createWindowEvent()
{
}