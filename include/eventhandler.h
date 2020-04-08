#ifndef GAME_ENGINE_EVENT_HANDLER_CLASS_H
#define GAME_ENGINE_EVENT_HANDLER_CLASS_H

class EventHandler
{
public:
	virtual void frameEvent() {}
	virtual void mouseEvent(int button, int state, int x, int y) {}
	virtual void mouseMotionEvent(int x, int y) {}
	virtual void keyboardEvent(unsigned char key, int state) {}
	virtual void resizeWindowEvent(int width, int height) {}
};

#endif