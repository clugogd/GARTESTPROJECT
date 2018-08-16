#ifndef _RENDERER_H_
#define _RENDERER_H_

//	DirectX includes
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <DirectXPackedVector.h>

#include <iostream>
#include <string>
//	Link library dependencies
#pragma comment(lib,	"d3d11.lib")
#pragma comment(lib,	"dxgi.lib")
#pragma comment(lib,	"d3dcompiler.lib")

using namespace DirectX;

//	Safely release a COM object
template<typename T>
inline void SafeRelease(T& ptr)
{
	if (ptr != NULL)
	{
		ptr->Release();
		ptr = NULL;
	}
}

template<class ShaderClass>
ShaderClass* LoadShader(const std::wstring&filename, const std::string& entryPoint, const std::string& profile);

const long SCREEN_WIDTH = 1440;
const long SCREEN_HEIGHT = 900;

struct TVERTEX
{
	TVERTEX() {}
	TVERTEX(XMFLOAT3 position, XMFLOAT4 color) : pos(position), Color(color) {}
	XMFLOAT3 pos;
	XMFLOAT4 Color;
};

//	Shader resource
enum ConstantBuffer
{
	CB_Application,
	CB_Frame,
	CB_Object,
	NumConstantBuffers
};

class CRenderer
{
public:
	CRenderer();
	~CRenderer();

	void Init(HWND hwnd);
	void Shutdown();
	void Update();

private:
	IDXGISwapChain * m_d3dSwapChain;
	ID3D11Device*	m_d3dDevice;
	ID3D11DeviceContext* m_d3dContext;
	ID3D11RenderTargetView* m_d3dBackBuffer;
	ID3D11DepthStencilView* m_d3dDepthStencilView;
	ID3D11Texture2D* m_d3dDepthStencilBuffer;
	ID3D11DepthStencilState* m_d3dDepthStencilState;
	ID3D11RasterizerState* m_d3dRasterizerState;

	D3D11_VIEWPORT m_Viewport;

	ID3D11InputLayout* m_pLayout;
	ID3D11Buffer* m_pVBuffer;
	ID3D11Buffer* m_pIBuffer;

	ID3D11VertexShader* m_pVS;
	ID3D11PixelShader* m_pPS;
	ID3D10Blob* m_pVSBuffer;
	ID3D10Blob* m_pPSBuffer;

	ID3D11Buffer* m_d3dConstantBuffers[NumConstantBuffers];

	void InitD3D(HWND hWnd);
	void CleanD3D();
	void RenderFrame();
	void InitGraphics();
	void InitPipeline();

	HRESULT hr;
};

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
#endif