#pragma once

#ifndef __CAMERACLASS_H__
#define __CAMERACLASS_H__

#include "cameradata.h"
#include <directxmath.h>
using namespace DirectX;

class CameraClass
{
public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	bool Initialize(const int&, const int&);
	void UpdateMatrix();
	void Shutdown();

private:
	XMFLOAT3 position;
	XMFLOAT3 rotationEuler;

	XMFLOAT3 forward;
	XMFLOAT3 up;
	XMFLOAT3 right;

	float nearClip;
	float farClip;
	float fov;
	float aspect;

	XMFLOAT4X4 viewMatrix;
	XMFLOAT4X4 projMatrix;

	CameraData* m_CameraData;

public:
	CameraData* GetCameraData()
	{
		return m_CameraData;
	}
};

#endif // !__CAMERACLASS_H__
