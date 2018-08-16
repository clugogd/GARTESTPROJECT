#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <DirectXMath.h>
using namespace DirectX;

class CCamera
{
public:
	CCamera();
	~CCamera();

	void Init() {}
	void Update() {}
	void Shutdown() {}

	XMMATRIX m_WorldMatrix;
	XMMATRIX m_ViewMatrix;
	XMMATRIX m_ProjectionMatrix;
};

#endif