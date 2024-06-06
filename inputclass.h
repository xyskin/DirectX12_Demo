#pragma once

#ifndef __INPUTCLASS_H__
#define __INPUTCLASS_H__

#define DIRECTXINPUT_VERSION 0x0800

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#include <dinput.h>

class InputClass
{
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

	bool Initialize(HINSTANCE hInstance, HWND hwnd);
	void Shutdown();
	bool Frame();

	bool IsButtonDown(const int&);
	bool IsButtonUp(const int&);
	bool IsButton(const int&);
private:
	bool ReadKeyboard();
	void InitializeCounting();
private:
	IDirectInput8* m_DirectInput;
	IDirectInputDevice8* m_Keyboard;

	unsigned char m_KeyboardState[256];
	unsigned int m_KeyDownTimes[256];
	unsigned int m_KeyUpTimes[256];
};
#endif // !__INPUTCLASS_H__

