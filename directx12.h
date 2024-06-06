#pragma once
#ifndef __DIRECTX12_H__
#define __DIRECTX12_H__

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <d3d12.h>
#include <dxgi1_6.h>

#include "resource.h"

#define VSYNC_ENABLE true
#define FULL_SCREEN false

class DirectX12Class
{
public:
	DirectX12Class();
	DirectX12Class(const DirectX12Class&);
	~DirectX12Class();

	bool Initialize(const int&, const int&, const HWND&);
	void Shutdown();

	bool BeginFrame();
	bool EndFrame();

private:
	ID3D12Device* m_Device;
	ID3D12CommandQueue* m_CommandQueue;
	IDXGISwapChain* m_SwapChain;
	ID3D12DescriptorHeap* m_RenderTargetViewHeap;
	//ID3D12Resource* m_BackBufferRenderTarget;
	Resource* m_BackBufferRenderTarget[2];
	unsigned int m_BufferIndex;
	ID3D12CommandAllocator* m_CommandAllocator;
	ID3D12GraphicsCommandList* m_CommandList;
	ID3D12PipelineState* m_PipelineState;
	ID3D12Fence* m_Fence;
	HANDLE m_FenceHandle;

	ID3D12Debug* m_Debug;

#pragma region Get_Function
public:
	ID3D12CommandAllocator* GetCommandAllocator()
	{
		return m_CommandAllocator;
	}

	ID3D12GraphicsCommandList* GetCommandList()
	{
		return m_CommandList;
	}

	ID3D12PipelineState* GetPipelineState()
	{
		return m_PipelineState;
	}

	ID3D12CommandQueue* GetCommandQueue()
	{
		return m_CommandQueue;
	}
	
	IDXGISwapChain* GetSwapChain()
	{
		return m_SwapChain;
	}

	ID3D12Fence* GetFence()
	{
		return m_Fence;
	}

	HANDLE* GetFenceHandle()
	{
		return &m_FenceHandle;
	}

	unsigned int* GetBackBufferIndex()
	{
		return &m_BufferIndex;
	}

	ID3D12Device* GetDevice()
	{
		return m_Device;
	}

	ID3D12DescriptorHeap* GetRenderTargetViewHeap()
	{
		return m_RenderTargetViewHeap;
	}

	Resource* GetBackBuffer(int index)
	{
		return m_BackBufferRenderTarget[index];
	}
#pragma endregion

#pragma region Cmd_Function
public:
	void SetTransitionBarrier(Resource* resource, D3D12_RESOURCE_BARRIER_TYPE type, D3D12_RESOURCE_STATES state)
	{
		resource->barrierPtr->Type = type;
		D3D12_RESOURCE_STATES currentState = resource->barrierPtr->Transition.StateAfter;
		resource->barrierPtr->Transition.StateBefore = currentState;
		resource->barrierPtr->Transition.StateAfter = state;
	}
#pragma endregion

};

#endif // !__DIRECTX12_H__
