#include "directx12.h"
#include "message.h"

DirectX12Class::DirectX12Class()
{
	m_Device = 0;
	m_CommandQueue = 0;
	m_SwapChain = 0;
	m_RenderTargetViewHeap = 0;
	m_BackBufferRenderTarget[0] = 0;
	m_BackBufferRenderTarget[1] = 0;
	//m_BackBufferRenderTarget = 0;
	m_CommandAllocator = 0;
	m_CommandList = 0;
	m_PipelineState = 0;
	m_Fence = 0;
	m_BufferIndex = 0;
	m_FenceHandle = 0;

	m_Debug = 0;
}

DirectX12Class::DirectX12Class(const DirectX12Class& directx)
{

}

DirectX12Class::~DirectX12Class()
{

}

bool DirectX12Class::Initialize(const int& screenWidth , const int& screenHeight, const HWND& hwnd)
{
	HRESULT result;

	D3D12_COMMAND_QUEUE_DESC commandQueueDesc;
	//DXGI_ADAPTER_DESC adapterDesc;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
	DXGI_MODE_DESC* displayModeList;
	D3D12_CPU_DESCRIPTOR_HANDLE renderTargetViewHandle;

	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int modeNums = 0;
	unsigned int numerator = 0;
	unsigned int denominator = 0;
	unsigned int renderTargetViewDescriptorSize = 0;
	
	ASSERT_HRESULT_SUCCESS(D3D12GetDebugInterface(IID_ID3D12Debug, (void**)&m_Debug));

	m_Debug->EnableDebugLayer();

	ASSERT_HRESULT_SUCCESS(D3D12CreateDevice(NULL, D3D_FEATURE_LEVEL_12_1, IID_ID3D12Device, (void**)&m_Device));

	ZeroMemory(&commandQueueDesc, sizeof(commandQueueDesc));
	commandQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	commandQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	commandQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	commandQueueDesc.NodeMask = 0;

	ASSERT_HRESULT_SUCCESS(m_Device->CreateCommandQueue(&commandQueueDesc, IID_ID3D12CommandQueue, (void**)&m_CommandQueue));
	
	ASSERT_HRESULT_SUCCESS(CreateDXGIFactory(IID_IDXGIFactory, (void**)&factory));

	ASSERT_HRESULT_SUCCESS(factory->EnumAdapters(NULL, &adapter));

	ASSERT_HRESULT_SUCCESS(adapter->EnumOutputs(NULL, &adapterOutput));

	ASSERT_HRESULT_SUCCESS(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &modeNums, NULL));

	displayModeList = new DXGI_MODE_DESC[modeNums];
	if (!displayModeList)
	{
		return false;
	}

	ASSERT_HRESULT_SUCCESS(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &modeNums, displayModeList));

	for (int i = 0; i < modeNums; i++)
	{
		if (displayModeList[i].Width == screenWidth)
		{
			if (displayModeList[i].Height == screenHeight)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	//ASSERT_HRESULT_SUCCESS(adapter->GetDesc(&adapterDesc));

	adapter->Release();
	adapter = 0;

	adapterOutput->Release();
	adapterOutput = 0;

	delete[] displayModeList;
	displayModeList = 0;

	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	swapChainDesc.BufferCount = 2;

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_BACK_BUFFER;

	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;
	if (VSYNC_ENABLE)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	swapChainDesc.Windowed = !FULL_SCREEN;
	swapChainDesc.OutputWindow = hwnd;

	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

	swapChainDesc.Flags = 0;

	ASSERT_HRESULT_SUCCESS(factory->CreateSwapChain(m_CommandQueue, &swapChainDesc, &m_SwapChain));

	factory->Release();
	factory = 0;

	ZeroMemory(&heapDesc, sizeof(heapDesc));

	heapDesc.NumDescriptors = 2;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	heapDesc.NodeMask = 0;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	ASSERT_HRESULT_SUCCESS(m_Device->CreateDescriptorHeap(&heapDesc, IID_ID3D12DescriptorHeap, (void**)&m_RenderTargetViewHeap));

	m_BackBufferRenderTarget[0] = new Resource;
	m_BackBufferRenderTarget[1] = new Resource;

	renderTargetViewDescriptorSize = m_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	renderTargetViewHandle = m_RenderTargetViewHeap->GetCPUDescriptorHandleForHeapStart();
	ASSERT_HRESULT_SUCCESS(m_SwapChain->GetBuffer(0, IID_ID3D12Resource, (void**)&(m_BackBufferRenderTarget[0]->resourcePtr)));
	m_Device->CreateRenderTargetView(m_BackBufferRenderTarget[0]->resourcePtr, NULL, renderTargetViewHandle);
	m_BackBufferRenderTarget[0]->CreateBarrier();
	m_BackBufferRenderTarget[0]->SetHandle(renderTargetViewHandle);

	renderTargetViewHandle.ptr += renderTargetViewDescriptorSize;
	ASSERT_HRESULT_SUCCESS(m_SwapChain->GetBuffer(1, IID_ID3D12Resource, (void**)&(m_BackBufferRenderTarget[1]->resourcePtr)));
	m_Device->CreateRenderTargetView(m_BackBufferRenderTarget[1]->resourcePtr, NULL, renderTargetViewHandle);
	m_BackBufferRenderTarget[1]->CreateBarrier();
	m_BackBufferRenderTarget[1]->SetHandle(renderTargetViewHandle);

	ASSERT_HRESULT_SUCCESS(m_Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_ID3D12CommandAllocator, (void**)&m_CommandAllocator));

	ASSERT_HRESULT_SUCCESS(m_Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_CommandAllocator, NULL, IID_ID3D12CommandList, (void**)&m_CommandList));

	ASSERT_HRESULT_SUCCESS(m_CommandList->Close());

	ASSERT_HRESULT_SUCCESS(m_Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_ID3D12Fence, (void**)&m_Fence));

	m_FenceHandle = CreateEventEx(NULL, FALSE, FALSE, EVENT_ALL_ACCESS);
	if (!m_FenceHandle)
	{
		return false;
	}

	return true;
}

void DirectX12Class::Shutdown()
{
	RELEASE_D3D12_OBJECT_PTR(m_Device);

	RELEASE_D3D12_OBJECT_PTR(m_Debug);

	RELEASE_D3D12_OBJECT_PTR(m_CommandQueue);

	RELEASE_D3D12_OBJECT_PTR(m_SwapChain);

	RELEASE_D3D12_OBJECT_PTR(m_RenderTargetViewHeap);

	RELEASE_DELETE_D3D12_OBJECT_PTR(m_BackBufferRenderTarget[0]);

	RELEASE_DELETE_D3D12_OBJECT_PTR(m_BackBufferRenderTarget[1]);

	RELEASE_D3D12_OBJECT_PTR(m_CommandAllocator);

	RELEASE_D3D12_OBJECT_PTR(m_CommandList);

	RELEASE_D3D12_OBJECT_PTR(m_Fence);

	return;
}

bool DirectX12Class::BeginFrame()
{
	HRESULT result;

	ASSERT_HRESULT_SUCCESS(m_CommandAllocator->Reset());

	ASSERT_HRESULT_SUCCESS(m_CommandList->Reset(m_CommandAllocator, m_PipelineState));

	return true;
}

bool DirectX12Class::EndFrame()
{
	HRESULT result;

	unsigned long long fenceToWaitFor = m_Fence->GetCompletedValue();

	SetTransitionBarrier(m_BackBufferRenderTarget[m_BufferIndex], D3D12_RESOURCE_BARRIER_TYPE_TRANSITION, D3D12_RESOURCE_STATE_PRESENT);
	m_CommandList->ResourceBarrier(1, m_BackBufferRenderTarget[m_BufferIndex]->barrierPtr);

	ASSERT_HRESULT_SUCCESS(m_CommandList->Close());

	ID3D12CommandList* ppCommandList[1];
	ppCommandList[0] = m_CommandList;

	m_CommandQueue->ExecuteCommandLists(1, ppCommandList);

	ASSERT_HRESULT_SUCCESS(m_SwapChain->Present(VSYNC_ENABLE, 0));

	fenceToWaitFor++;

	ASSERT_HRESULT_SUCCESS(m_CommandQueue->Signal(m_Fence, fenceToWaitFor));

	if (m_Fence->GetCompletedValue() < fenceToWaitFor)
	{
		ASSERT_HRESULT_SUCCESS(m_Fence->SetEventOnCompletion(fenceToWaitFor, m_FenceHandle));
		WaitForSingleObject(m_FenceHandle, INFINITE);
	}

	m_BufferIndex ^= 1;

	return true;
}