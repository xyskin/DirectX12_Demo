#include "inputclass.h"
#include "message.h"

InputClass::InputClass()
{
	m_DirectInput = 0;
	m_Keyboard = 0;
}

InputClass::InputClass(const InputClass& input)
{

}

InputClass::~InputClass()
{

}

void InputClass::InitializeCounting()
{
	for (unsigned int i = 0; i < 256; i++)
	{
		m_KeyDownTimes[i] = 0;
		m_KeyUpTimes[i] = 0;
	}
}

bool InputClass::Initialize(HINSTANCE hInstance, HWND hwnd)
{
	HRESULT result;

	ASSERT_HRESULT_SUCCESS(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_DirectInput, NULL));

	ASSERT_HRESULT_SUCCESS(m_DirectInput->CreateDevice(GUID_SysKeyboard, &m_Keyboard, NULL));

	ASSERT_HRESULT_SUCCESS(m_Keyboard->SetDataFormat(&c_dfDIKeyboard));

	ASSERT_HRESULT_SUCCESS(m_Keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE));

	ASSERT_HRESULT_SUCCESS(m_Keyboard->Acquire());

	return true;
}

void InputClass::Shutdown()
{
	if (m_DirectInput)
	{
		m_DirectInput->Release();
		m_DirectInput = 0;
	}

	if (m_Keyboard)
	{
		m_Keyboard->Unacquire();
		m_Keyboard->Release();
		m_Keyboard = 0;
	}

	return;
}

bool InputClass::Frame()
{
	bool result;
	ASSERT_SUCCESS(ReadKeyboard());

	return true;
}

bool InputClass::ReadKeyboard()
{
	HRESULT result;
	result = m_Keyboard->GetDeviceState(sizeof(m_KeyboardState), (LPVOID)&m_KeyboardState);
	if (FAILED(result))
	{
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
		{
			m_Keyboard->Acquire();
		}
		else
		{
			MESSAGE_FAIL(m_Keyboard->GetDeviceState, result);
			return false;
		}
	}

	return true;
}

bool InputClass::IsButtonDown(const int& key)
{
	if (m_KeyboardState[key] & 0x80)
	{
		m_KeyDownTimes[key]++;

		if (m_KeyDownTimes[key] == 1)
		{
			return true;
		}
		return false;
	}

	m_KeyDownTimes[key] = 0;
	return false;
}

bool InputClass::IsButton(const int& key)
{
	return m_KeyboardState[key] & 0x80 ? true : false;
}

bool InputClass::IsButtonUp(const int& key)
{
	if (m_KeyboardState[key] & 0x80)
	{
		m_KeyUpTimes[key]++;
		return false;
	}
	
	if (m_KeyUpTimes[key] > 0)
	{
		m_KeyUpTimes[key] = 0;
		return true;
	}

	return false;
}