#include "renderpipeline.h"
#include "message.h"

RenderPipeline::RenderPipeline()
{
	m_DirectX = 0;
}

RenderPipeline::RenderPipeline(const RenderPipeline&)
{

}

RenderPipeline::~RenderPipeline()
{

}

bool RenderPipeline::Initialize(DirectX12Class* directX)
{
	if (!directX)
	{
		return false;
	}

	m_DirectX = directX;

	return true;
}

bool RenderPipeline::Render()
{
	bool result;

	ASSERT_SUCCESS(m_DirectX->BeginFrame());

	ASSERT_SUCCESS(RenderFrame());

	ASSERT_SUCCESS(m_DirectX->EndFrame());

	return true;
}

bool RenderPipeline::RenderFrame()
{
	ID3D12GraphicsCommandList* commandList = m_DirectX->GetCommandList();
	ID3D12Device* device = m_DirectX->GetDevice();
	ID3D12DescriptorHeap* renderTargetViewHeap = m_DirectX->GetRenderTargetViewHeap();
	Resource* backBufferRenderTarget;
	unsigned int* bufferIndex = m_DirectX->GetBackBufferIndex();
	backBufferRenderTarget = m_DirectX->GetBackBuffer(*bufferIndex);

	unsigned int renderTargetViewDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	D3D12_CPU_DESCRIPTOR_HANDLE renderTargetViewHandle = renderTargetViewHeap->GetCPUDescriptorHandleForHeapStart();
	renderTargetViewHandle.ptr += *bufferIndex * renderTargetViewDescriptorSize;

	m_DirectX->SetTransitionBarrier(backBufferRenderTarget, D3D12_RESOURCE_BARRIER_TYPE_TRANSITION, D3D12_RESOURCE_STATE_RENDER_TARGET);
	commandList->ResourceBarrier(1, backBufferRenderTarget->barrierPtr);
	commandList->OMSetRenderTargets(1, &renderTargetViewHandle, FALSE, NULL);

	float clearColor[4] = { 0.1f, 0.3f, 0.5f, 1 };
	commandList->ClearRenderTargetView(renderTargetViewHandle, clearColor, 0, NULL);

	return true;
}

void RenderPipeline::Shutdown()
{
	if (m_DirectX)
	{
		m_DirectX = 0;
	}
}