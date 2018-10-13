#pragma once
#ifndef _INPUTCONTROLLER_H_
#define _INPUTCONTROLLER_H_

#include <dinput.h>


class InputController
{
public:
	InputController();
	~InputController();

	bool Initialize();

	void KeyDown(unsigned int);
	void KeyUp(unsigned int);
	bool IsKeyDown(unsigned int);

	void GetMouseLocation(int&, int&);



private:
	bool ReadMouse();
	void ProcessInput();

	bool m_keys[256];

	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_mouse;

	DIMOUSESTATE m_mouseState;

	int m_screenWidth, m_screenHeight;
	int m_mouseX, m_mouseY;
};

#endif