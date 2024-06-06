#include "camera.h"

CameraClass::CameraClass()
{
	position = XMFLOAT3();
	rotationEuler = XMFLOAT3();
	forward = XMFLOAT3();
	up = XMFLOAT3();
	right = XMFLOAT3();

	nearClip = 0;
	farClip = 0;
	fov = 0;
	aspect = 0;

	viewMatrix = XMFLOAT4X4();
	projMatrix = XMFLOAT4X4();

	m_CameraData = 0;
}

CameraClass::CameraClass(const CameraClass&)
{

}

CameraClass::~CameraClass()
{

}

bool CameraClass::Initialize(const int& screenWidth, const int& screenHeight)
{
	position = XMFLOAT3(0, 0, -1);
	rotationEuler = XMFLOAT3(0, 0, 0);

	forward = XMFLOAT3(0, 0, 1);
	up = XMFLOAT3(0, 1, 0);

	nearClip = 0.1;
	farClip = 100;
	fov = 90;
	aspect = (float)screenWidth / (float)screenHeight;

	m_CameraData = new CameraData;
	if (!m_CameraData)
	{
		return false;
	}

	return true;
}

void CameraClass::UpdateMatrix()
{
	XMVECTOR pyr = XMLoadFloat3(&rotationEuler) * (XM_PI / 180.0f);

	m_CameraData->viewMatrix = XMMatrixRotationRollPitchYawFromVector(pyr);

	m_CameraData->forward = XMLoadFloat3(&forward);
	m_CameraData->up = XMLoadFloat3(&up);

	m_CameraData->forward = XMVector3Transform(m_CameraData->forward, m_CameraData->viewMatrix);
	m_CameraData->up = XMVector3Transform(m_CameraData->up, m_CameraData->viewMatrix);

	m_CameraData->right = XMVector3Cross(m_CameraData->up, m_CameraData->forward);
	m_CameraData->position = XMLoadFloat3(&position);

	m_CameraData->viewMatrix = XMMatrixLookAtLH(m_CameraData->position, m_CameraData->position + m_CameraData->forward, m_CameraData->up);

	XMStoreFloat3(&forward, m_CameraData->forward);
	XMStoreFloat3(&right, m_CameraData->right);
	XMStoreFloat3(&up, m_CameraData->up);
	XMStoreFloat4x4(&viewMatrix, m_CameraData->viewMatrix);

	m_CameraData->projMatrix = XMMatrixPerspectiveFovLH(fov, aspect, nearClip, farClip);
	XMStoreFloat4x4(&projMatrix, m_CameraData->projMatrix);
}

void CameraClass::Shutdown()
{
	if (m_CameraData)
	{
		delete m_CameraData;
		m_CameraData = 0;
	}
}