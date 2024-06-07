#pragma once

#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <d3d12.h>
#include <dxgi1_6.h>

class Resource
{
public:
	D3D12_RESOURCE_DESC* descPtr;
	ID3D12Resource* resourcePtr;
	D3D12_RESOURCE_BARRIER* barrierPtr;
	D3D12_CPU_DESCRIPTOR_HANDLE handle;

	void Release()
	{
		if (descPtr)
		{
			delete descPtr;
			descPtr = 0;
		}

		if (barrierPtr)
		{
			delete barrierPtr;
			barrierPtr = 0;
		}

		if (resourcePtr)
		{
			resourcePtr->Release();
			resourcePtr = 0;
		}
	}

	void CreateBarrier()
	{
		barrierPtr = new D3D12_RESOURCE_BARRIER;

		barrierPtr->Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;

		barrierPtr->Transition.pResource = resourcePtr;
		barrierPtr->Transition.StateBefore = D3D12_RESOURCE_STATE_COMMON;
		barrierPtr->Transition.StateAfter = D3D12_RESOURCE_STATE_COMMON;
		barrierPtr->Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

		barrierPtr->UAV.pResource = resourcePtr;

		barrierPtr->Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	}

	void SetHandle(D3D12_CPU_DESCRIPTOR_HANDLE handle)
	{
		this->handle = handle;
	}
};


#endif // !__RESOURCE_H__
