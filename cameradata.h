#pragma once

#ifndef __CAMERADATA_H__
#define __CAMERADATA_H__

#include <directxmath.h>
using namespace DirectX;

struct CameraData
{
public:
	XMVECTOR position;
	XMVECTOR rotationEuler;

	XMVECTOR forward;
	XMVECTOR up;
	XMVECTOR right;

	XMMATRIX viewMatrix;
	XMMATRIX projMatrix;
};
#endif