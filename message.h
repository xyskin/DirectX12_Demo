#pragma once

#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include <wchar.h>

#define MESSAGE_FAIL(Func)				\
	do									\
	{									\
		MessageBox(NULL, L#Func L" -> Failed", NULL, MB_OK);\
	}									\
	while(0)							\

#define MESSAGE_HRESULT_FAIL(Func, Res)	\
	do									\
	{									\
		wchar_t msg[256];				\
		swprintf_s(msg, L"%s -> Failed : 0x%08X", L#Func, Res);\
		MessageBox(NULL, msg, NULL, MB_OK);\
	}									\
	while(0)							\

#define ASSERT_SUCCESS(Func)			\
	do									\
	{									\
		result = (Func);				\
		if (!result)					\
		{								\
			MESSAGE_FAIL((Func));		\
			return false;				\
		}								\
	}									\
	while(0)							\

#define ASSERT_HRESULT_SUCCESS(Func)				\
	do												\
	{												\
		result = (Func);							\
		if (FAILED(result))							\
		{											\
			MESSAGE_HRESULT_FAIL((Func), (result));	\
			return false;							\
		}											\
	}												\
	while(0)										\

#define RELEASE_D3D12_OBJECT_PTR(obj)	\
	do									\
	{									\
		if (obj)						\
		{								\
			obj->Release();				\
			obj = 0;					\
		}								\
	}									\
	while(0)							\

#define DELETE_D3D12_OBJECT_PTR(obj)	\
	do									\
	{									\
		if (obj)						\
		{								\
			delete obj;					\
			obj = 0;					\
		}								\
	}									\
	while(0)							\

#define RELEASE_DELETE_D3D12_OBJECT_PTR(obj)	\
	do									\
	{									\
		if (obj)						\
		{								\
			obj->Release();				\
			delete obj;					\
			obj = 0;					\
		}								\
	}									\
	while(0)							\

#endif // !__MESSAGE_H__
