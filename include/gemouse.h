#ifndef GAME_ENGINE_MOUSE_INPUT_CLASS_H
#define GAME_ENGINE_MOUSE_INPUT_CLASS_H

class GEMouse
{
public:
	GEMouse();

	// Getters and Setters
	void setXPosition(int xPosition);
	void setYPosition(int yPosition);
	void setState(int state);
	void setButton(int button);
	int getXPosition();
	int getYPosition();
	int getState();
	int getButton();

private:
	int xPosition;
	int yPosition;
	int state;
	int button;
};

#endif