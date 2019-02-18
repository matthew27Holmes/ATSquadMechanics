#pragma once

#define DIRECTINPUT_VERSION 0x0800
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#include <dinput.h>
#include "windows.h"

class InputController
{
public:
	InputController();
	~InputController();

	bool Init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight);

	bool Update(HWND hwnd);

	bool ReadMouse();
	void GetMouseLocation(int&, int&);
	void setMousePos(HWND hwnd);//int pixelX, int pixelY, DWORD flags
	bool IsLeftMouseButtonDown();


	bool IsKeyDown(unsigned int key);
	bool IsEscapePressed();

private:
	bool ReadKeyboard();

	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;
	unsigned char m_keyboardState[256];

	IDirectInputDevice8* m_mouse;
	DIMOUSESTATE m_mouseState;

	int m_mouseX, m_mouseY;
	int m_screenWidth, m_screenHeight;
};
