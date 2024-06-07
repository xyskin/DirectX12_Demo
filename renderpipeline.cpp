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

bool RenderPipeline::Initialize(const int& screenWidth, const int& screenHeight, const HWND& hwnd)
{
	bool result;

	m_DirectX = new DirectX12Class;
	if (!m_DirectX)
	{
		return false;
	}

	ASSERT_SUCCESS(m_DirectX->Initialize(screenWidth, screenHeight, hwnd));

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
	Resource* backBufferRenderTarget;
	unsigned int* bufferIndex = m_DirectX->GetBackBufferIndex();
	backBufferRenderTarget = m_DirectX->GetBackBuffer(*bufferIndex);

	m_DirectX->SetTransitionBarrier(backBufferRenderTarget, D3D12_RESOURCE_BARRIER_TYPE_TRANSITION, D3D12_RESOURCE_STATE_RENDER_TARGET);
	commandList->OMSetRenderTargets(1, &(backBufferRenderTarget->handle), FALSE, NULL);

	float clearColor[4] = { 0.1f, 0.3f, 0.5f, 1 };
	commandList->ClearRenderTargetView(backBufferRenderTarget->handle, clearColor, 0, NULL);

	return true;
}

void RenderPipeline::Shutdown()
{
	if (m_DirectX)
	{
		m_DirectX->Shutdown();
		delete m_DirectX;
		m_DirectX = 0;
	}
}