#include "graphics.h"
#include "message.h"

GraphicsClass::GraphicsClass()
{
	m_Pipeline = 0;
	m_Camera = 0;
}

GraphicsClass::GraphicsClass(const GraphicsClass& graphics)
{

}

GraphicsClass::~GraphicsClass()
{

}

bool GraphicsClass::Initialize(const int& screenWidth, const int& screenHeight, const HWND& hwnd)
{
	bool result;

	m_Pipeline = new RenderPipeline;
	if (!m_Pipeline)
	{
		return false;
	}

	ASSERT_SUCCESS(m_Pipeline->Initialize(screenWidth, screenHeight, hwnd));

	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}
	
	ASSERT_SUCCESS(m_Camera->Initialize(screenWidth, screenHeight));

	return true;
}

void GraphicsClass::Shutdown()
{
	if (m_Pipeline)
	{
		m_Pipeline->Shutdown();
		delete m_Pipeline;
		m_Pipeline = 0;
	}

	if (m_Camera)
	{
		m_Camera->Shutdown();
		delete m_Camera;
		m_Camera = 0;
	}

	return;
}

bool GraphicsClass::Frame()
{
	bool result;

	ASSERT_SUCCESS(Render());

	return true;
}

bool GraphicsClass::Render()
{
	bool result;

	m_Camera->UpdateMatrix();

	ASSERT_SUCCESS(m_Pipeline->Render());

	return true;
}