#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

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
	ID3D11InputLayout* pLayout;
	ID3D11VertexShader* pVS;
	ID3D11PixelShader* pPS;
	ID3D11Buffer* pVBuffer;

	struct VERTEX { FLOAT X, Y, Z; float Color[4]; };

	void InitD3D(HWND hWnd);
	void CleanD3D();
	void RenderFrame();
	void InitGraphics();
	void InitPipeline();
};

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
#endif