#pragma once

#ifndef __SYSTEMCLASS_H__
#define __SYSTEMCLASS_H__

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include "graphics.h"
#include "inputclass.h"

class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();
private:
	LPCWSTR m_ApplicationName;
	HINSTANCE m_Hinstance;
	HWND m_Hwnd;

	InputClass* m_Input;
	GraphicsClass* m_Graphics;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static SystemClass* ApplicationHandle = 0;

#endif