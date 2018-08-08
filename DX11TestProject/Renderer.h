#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

#pragma comment(lib, "d3d11.lib")

#define SCREEN_WIDTH 1440
#define SCREEN_HEIGHT 900

class Renderer
{
public:
	Renderer();
	~Renderer();

	void Init(HWND hwnd);
	void Shutdown();
	void Update();

private:
	IDXGISwapChain * swapchain;
	ID3D11Device*	device;
	ID3D11DeviceContext* context;
	ID3D11RenderTargetView* backbuffer;

	void InitD3D(HWND hWnd);
	void CleanD3D();
	void RenderFrame();
};

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
#endif