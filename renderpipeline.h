#pragma once

#ifndef __RENDER_PIPELINE_H__
#define __RENDER_PIPELINE_H__

#include "directx12.h"

class RenderPipeline
{
public:
	RenderPipeline();
	RenderPipeline(const RenderPipeline&);
	~RenderPipeline();

	bool Initialize(const int&, const int&, const HWND&);
	bool Render();
	void Shutdown();
private:
	bool RenderFrame();
private:
	DirectX12Class* m_DirectX;

private:
	
};

#endif // !1
