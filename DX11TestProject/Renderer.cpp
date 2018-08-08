#include "Renderer.h"
#include <D3DX10math.h>

Renderer::Renderer()
{
}


Renderer::~Renderer()
{
}

void Renderer::Init(HWND hwnd)
{
	InitD3D(hwnd);
}
void Renderer::Shutdown()
{
	CleanD3D();
}
void Renderer::Update()
{
	RenderFrame();
}
void Renderer::InitD3D(HWND hWnd)
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
	scd.OutputWindow = hWnd;                                // the window to be used
	scd.SampleDesc.Count = 4;                               // how many multisamples
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
		&swapchain,
		&device,
		NULL,
		&context);

	// get the address of the back buffer
	ID3D11Texture2D *pBackBuffer;
	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	// use the back buffer address to create the render target
	device->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer);
	pBackBuffer->Release();

	// set the render target as the back buffer
	context->OMSetRenderTargets(1, &backbuffer, NULL);


	// Set the viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = 800;
	viewport.Height = 600;

	context->RSSetViewports(1, &viewport);


	InitPipeline();
	InitGraphics();
}
void Renderer::CleanD3D()
{
	swapchain->SetFullscreenState(FALSE, NULL);

	// close and release all existing COM objects
	pLayout->Release();
	pVS->Release();
	pPS->Release();
	pVBuffer->Release();
	swapchain->Release();
	backbuffer->Release();
	device->Release();
	context->Release();
}

void Renderer::RenderFrame()
{
	// clear the back buffer to a deep blue
	float color[4] = { 0.0f, 0.2f, 0.4f, 1.0f };
	context->ClearRenderTargetView(backbuffer, color);

	// do 3D rendering on the back buffer here
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;

	context->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);
	context->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	context->Draw(3, 0);

	// switch the back buffer and the front buffer
	swapchain->Present(0, 0);
}

void Renderer::InitGraphics()
{
	// create a triangle using the VERTEX struct
	VERTEX OurVertices[] =
	{
		{ 0.0f, 0.5f, 0.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f) },
	{ 0.45f, -0.5, 0.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f) },
	{ -0.45f, -0.5f, 0.0f, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f) }
	};


	// create the vertex buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
	bd.ByteWidth = sizeof(VERTEX) * 3;             // size is the VERTEX struct * 3
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

	device->CreateBuffer(&bd, NULL, &pVBuffer);       // create the buffer


												   // copy the vertices into the buffer
	D3D11_MAPPED_SUBRESOURCE ms;
	context->Map(pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
	memcpy(ms.pData, OurVertices, sizeof(OurVertices));                 // copy the data
	context->Unmap(pVBuffer, NULL);                                      // unmap the buffer

}

void Renderer::InitPipeline()
{
	// load and compile the two shaders
	ID3D10Blob *VS, *PS;
	D3DX11CompileFromFileW(L"shaders.shader", 0, 0, "VShader", "vs_4_0", 0, 0, 0, &VS, 0, 0);
	D3DX11CompileFromFileW(L"shaders.shader", 0, 0, "PShader", "ps_4_0", 0, 0, 0, &PS, 0, 0);

	// encapsulate both shaders into shader objects
	device->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pVS);
	device->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pPS);

	// set the shader objects
	context->VSSetShader(pVS, 0, 0);
	context->PSSetShader(pPS, 0, 0);

	// create the input layout object
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	device->CreateInputLayout(ied, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &pLayout);
	context->IASetInputLayout(pLayout);
}
