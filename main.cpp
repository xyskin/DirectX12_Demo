#define _CRT_SECURE_NO_WARNINGS

#include "systemclass.h"
#include <iostream>
using namespace std;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR psCmdLine, int iCmdshow)
{
	SystemClass* m_System;
	bool result = false;

	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);

	m_System = new SystemClass();
	if (!m_System)
	{
		return 0;
	}

	result = m_System->Initialize();
	if (result)
	{
		m_System->Run();
	}

	m_System->Shutdown();
	delete m_System;
	m_System = 0;

	FreeConsole();

	return 0;
}