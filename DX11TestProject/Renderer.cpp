#include "Renderer.h"

CRenderer::CRenderer()
{
}


CRenderer::~CRenderer()
{
}

void CRenderer::Init(HWND hwnd)
{
	InitD3D(hwnd);
}
void CRenderer::Shutdown()
{
	CleanD3D();
}
void CRenderer::Update()
{
	RenderFrame();
}
void CRenderer::InitD3D(HWND hWnd)
{
	// create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC scd;

	// clear out the struct for use
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	// fill the swap chain description struct
	scd.BufferCount = 1;                                    // one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
	scd.BufferDesc.Width = SCREEN_WIDTH;
	scd.BufferDesc.Height = SCREEN_HEIGHT;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	scd.SampleDesc.Count = 4;                               // how many multisamples
	scd.SampleDesc.Quality = 0;
	scd.OutputWindow = hWnd;                                // the window to be used
	scd.Windowed = TRUE;                                    // windowed/full-screen mode
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
															// create a device, device context and swap chain using the information in the scd struct
	D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&m_d3dSwapChain,
		&m_d3dDevice,
		NULL,
		&m_d3dContext);

	// get the address of the back buffer
	ID3D11Texture2D *pBackBuffer;
	m_d3dSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	// use the back buffer address to create the render target
	m_d3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_d3dBackBuffer);
	pBackBuffer->Release();

	// set the render target as the back buffer
	m_d3dContext->OMSetRenderTargets(1, &m_d3dBackBuffer, NULL);


	// Set the viewport
	ZeroMemory(&m_Viewport, sizeof(D3D11_VIEWPORT));

	m_Viewport.TopLeftX = 0;
	m_Viewport.TopLeftY = 0;
	m_Viewport.Width = 800;
	m_Viewport.Height = 600;

	m_d3dContext->RSSetViewports(1, &m_Viewport);


	InitPipeline();
	InitGraphics();
}
void CRenderer::CleanD3D()
{
	m_d3dSwapChain->SetFullscreenState(FALSE, NULL);

	// close and release all existing COM objects
	SafeRelease(m_pLayout);
	SafeRelease(m_pVS);
	SafeRelease(m_pPS);
	SafeRelease(m_pVSBuffer);
	SafeRelease(m_pPSBuffer);
	SafeRelease(m_pVBuffer);
	SafeRelease(m_d3dSwapChain);
	SafeRelease(m_d3dBackBuffer);
	SafeRelease(m_d3dDevice);
	SafeRelease(m_d3dContext);
}

void CRenderer::RenderFrame()
{
	// clear the back buffer to a deep blue
	XMFLOAT4 color{ 0.0f, 0.2f, 0.4f, 1.0f };
	m_d3dContext->ClearRenderTargetView(m_d3dBackBuffer, (FLOAT*)&color);

	// do 3D rendering on the back buffer here
	UINT stride = sizeof(TVERTEX);
	UINT offset = 0;

	m_d3dContext->IASetVertexBuffers(0, 1, &m_pVBuffer, &stride, &offset);
	m_d3dContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_d3dContext->Draw(3, 0);

	// switch the back buffer and the front buffer
	m_d3dSwapChain->Present(0, 0);
}

void CRenderer::InitGraphics()
{
	// create a triangle using the VERTEX struct
	TVERTEX tVertices[8] =
	{
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f, 0.0f, 0.0f,1.0) }, // 0
		{ XMFLOAT3(-1.0f,  1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f,1.0) }, // 1
		{ XMFLOAT3(1.0f,  1.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f,1.0) }, // 2
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f,1.0) }, // 3
		{ XMFLOAT3(-1.0f, -1.0f,  1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f,1.0) }, // 4
		{ XMFLOAT3(-1.0f,  1.0f,  1.0f), XMFLOAT4(0.0f, 1.0f, 1.0f,1.0) }, // 5
		{ XMFLOAT3(1.0f,  1.0f,  1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f,1.0) }, // 6
		{ XMFLOAT3(1.0f, -1.0f,  1.0f), XMFLOAT4(1.0f, 0.0f, 1.0f,1.0) }  // 7
	};

	WORD g_Indicies[36] =
	{
		0, 1, 2, 0, 2, 3,
		4, 6, 5, 4, 7, 6,
		4, 5, 1, 4, 1, 0,
		3, 2, 6, 3, 6, 7,
		1, 5, 6, 1, 6, 2,
		4, 0, 3, 4, 3, 7
	};

	// create the vertex buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
	bd.ByteWidth = sizeof(TVERTEX) * 3;             // size is the VERTEX struct * 3
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

	m_d3dDevice->CreateBuffer(&bd, NULL, &m_pVBuffer);       // create the buffer


												   // copy the vertices into the buffer
	D3D11_MAPPED_SUBRESOURCE ms;
	m_d3dContext->Map(m_pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
	memcpy(ms.pData, tVertices, sizeof(tVertices));                 // copy the data
	m_d3dContext->Unmap(m_pVBuffer, NULL);                                      // unmap the buffer
}

void CRenderer::InitPipeline()
{
	// load and compile the two shaders
	hr = D3DCompileFromFile(L"Effects.fx", 0, 0, "VS", "vs_4_0", 0, 0, &m_pVSBuffer, NULL);
	hr = D3DCompileFromFile(L"Effects.fx", 0, 0, "PS", "ps_4_0", 0, 0, &m_pPSBuffer, NULL);

	// encapsulate both shaders into shader objects
	hr = m_d3dDevice->CreateVertexShader(m_pVSBuffer->GetBufferPointer(), m_pVSBuffer->GetBufferSize(), NULL, &m_pVS);
	hr = m_d3dDevice->CreatePixelShader(m_pPSBuffer->GetBufferPointer(), m_pPSBuffer->GetBufferSize(), NULL, &m_pPS);

	// set the shader objects
	m_d3dContext->VSSetShader(m_pVS, 0, 0);
	m_d3dContext->PSSetShader(m_pPS, 0, 0);

	// create the input layout object
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	m_d3dDevice->CreateInputLayout(layout, 2, m_pVSBuffer->GetBufferPointer(), m_pVSBuffer->GetBufferSize(), &m_pLayout);
	m_d3dContext->IASetInputLayout(m_pLayout);
}
