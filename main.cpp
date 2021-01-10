#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include <ge.h>
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

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	GLEventHandler gameEvents;
	engine = new KEngine(&gameEvents);

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

	engine->startMainLoop();

	delete engine;
	return 1;
}

void GLEventHandler::frameEvent()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
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