#pragma once

#ifndef __GRAPHICSCLASS_H__
#define __GRAPHICSCLASS_H__

#include "renderpipeline.h"
#include "camera.h"

class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(const int&, const int&, const HWND&);
	void Shutdown();
	bool Frame();

private:
	bool Render();

private:
	RenderPipeline* m_Pipeline;
	CameraClass* m_Camera;
};

#endif // !__GRAPHICSCLASS_H__
