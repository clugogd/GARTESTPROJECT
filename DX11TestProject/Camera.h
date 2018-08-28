#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <DirectXMath.h>

class CCamera
{
public:
	CCamera();
	~CCamera();

	void SetCamera(const DirectX::XMFLOAT4X4 matrix);
};

#endif