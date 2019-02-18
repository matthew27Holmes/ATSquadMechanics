#include "InputController.h"

InputController::InputController()
{
	m_directInput = 0;
	m_keyboard = 0;
	m_mouse = 0;
}

InputController::~InputController()
{
	// Release the keyboard.
	if (m_keyboard)
	{
		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_keyboard = 0;
	}

	if (m_mouse)
	{
		m_mouse->Unacquire();
		m_mouse->Release();
		m_mouse = 0;
	}

	// Release the main interface to direct input.
	if (m_directInput)
	{
		m_directInput->Release();
		m_directInput = 0;
	}

}

bool InputController::Init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	HRESULT result;

	// Store the screen size which will be used for positioning the mouse cursor.
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// Initialize the location of the mouse on the screen.
	m_mouseX = 0;
	m_mouseY = 0;

	// Initialize the main direct input interface.
	result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Initialize the direct input interface for the keyboard.
	result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Set the data format.  In this case since it is a keyboard we can use the predefined data format.
	result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
	{
		return false;
	}

	// Set the cooperative level of the keyboard to not share with other programs.
	result = m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	// Now acquire the keyboard.
	result = m_keyboard->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	// Initialize the direct input interface for the mouse.
	result = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Set the data format for the mouse using the pre-defined mouse data format.
	result = m_mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
	{
		return false;
	}

	// Set the cooperative level of the mouse to share with other programs.
	result = m_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	// Acquire the mouse.
	result = m_mouse->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool InputController::Update(HWND hwnd)
{
	bool result;

	// Read the current state of the keyboard.
	result = ReadKeyboard();
	if (!result)
	{
		return false;
	}
	result = ReadMouse();
	if (!result)
	{
		return false;
	}
	setMousePos(hwnd);

	return true;
}

bool InputController::ReadKeyboard()
{
	HRESULT result;

	// Read the keyboard device.
	result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if (FAILED(result))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_keyboard->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool InputController::ReadMouse()
{
	HRESULT result;


	// Read the mouse device.
	result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if (FAILED(result))
	{
		// If the mouse lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_mouse->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

void InputController::GetMouseLocation(int& mouseX, int& mouseY)
{
	mouseX = m_mouseX;
	mouseY =  m_mouseY;
}

void InputController::setMousePos(HWND hwnd)
{
	POINT cursorPos;
	GetCursorPos(&cursorPos);
	ScreenToClient(hwnd, &cursorPos);

	m_mouseX = cursorPos.x;
	m_mouseY = cursorPos.y;
}

bool InputController::IsLeftMouseButtonDown()
{
	if (m_mouseState.rgbButtons[0])
	{
		return true;
	}

	return false;
}

bool InputController::IsKeyDown(unsigned int key)
{
	if (m_keyboardState[key] & 0x80)
	{
		return true;
	}

	return false;
}

bool InputController::IsEscapePressed()
{
	if (m_keyboardState[DIK_ESCAPE] & 0x80)
	{
		return true;
	}

	return false;
}



