#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
using namespace DirectX;

#define SCREEN_WIDTH 1440
#define SCREEN_HEIGHT 900

struct VERTEX 
{
	VERTEX() {}
	VERTEX(float x, float y, float z, float r, float g, float b, float a) : pos(x, y, z),Color(r,g,b,a) {}
	XMFLOAT3 pos;
	XMFLOAT4 Color;
};

//D3D11_INPUT_ELEMENT_DESC layout[] =
//{
//	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
//};

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
	ID3D10Blob* pVSBuffer;
	ID3D10Blob* pPSBuffer;
	ID3D11Buffer* pVBuffer;

	void InitD3D(HWND hWnd);
	void CleanD3D();
	void RenderFrame();
	void InitGraphics();
	void InitPipeline();

	HRESULT hr;
};

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
#endif