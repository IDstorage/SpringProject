#include "Spring_GRenderSystem.h"
#include "Spring_GColorShader.h"

using namespace DirectX;
using namespace spring;


#pragma region Constructor & Destructor
GD3DClass::GD3DClass()
	: isVsyncEnable(false),
	videoCardMemory(0),
	videoCardDescription{ 0 },
	swapChain(nullptr),
	device(nullptr),
	deviceContext(nullptr),
	renderTargetView(nullptr),
	depthStencilBuf(nullptr),
	depthStencilView(nullptr),
	depthStencilState(nullptr),
	rasterizerState(nullptr) {}

GD3DClass::GD3DClass(const GD3DClass& obj)
	: isVsyncEnable(false),
	videoCardMemory(0),
	videoCardDescription{ 0 },
	swapChain(nullptr),
	device(nullptr),
	deviceContext(nullptr),
	renderTargetView(nullptr),
	depthStencilBuf(nullptr),
	depthStencilView(nullptr),
	depthStencilState(nullptr),
	rasterizerState(nullptr) {}

GD3DClass::~GD3DClass() {}
#pragma endregion


bool GD3DClass::Initialize(int screenWidth, int screenHeight, bool vsync,
	HWND hWnd, bool isFullScreen, float screenDepth, float screenNear) {

	// ��������ȭ ����
	isVsyncEnable = vsync;

	// DirectX �׷��� �������̽� ���丮
	IDXGIFactory* factory = nullptr;
	if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)& factory)))
		return false;

	// ���丮 ��ü�� ����Ͽ� ù��° �׷��� ī�� �������̽� ��� ����
	IDXGIAdapter* adapter = nullptr;
	if (FAILED(factory->EnumAdapters(0, &adapter)))
		return false;

	// ���(�����)�� ���� ù��° ��͸� ������
	IDXGIOutput* adapterOutput = nullptr;
	if (FAILED(adapter->EnumOutputs(0, &adapterOutput)))
		return false;

	// ��¿� ���� DXGI_FORMAT_R8G8B8A8_UNORM ǥ�� ���Ŀ� �´� ��� ���� ������
	unsigned int numModes = 0;
	if (FAILED(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED,
		&numModes, NULL)))
		return false;

	// ������ ��� ����Ϳ� �׷���ī�� ������ ������ ����Ʈ ����
	DXGI_MODE_DESC* displayModeList = new DXGI_MODE_DESC[100];
	if (!displayModeList)
		return false;

	// ���÷��� ��忡 ���� ����Ʈ�� ä��
	if (FAILED(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED,
		&numModes, displayModeList)))
		return false;

	// ��� ���÷��� ��忡 ���� ȭ�� �ʺ�/���̿� �´� ���÷��� ��带 ã��
	// ������� ���ΰ�ħ ������ �и�� ���� ����
	size_t numerator = 0, denominator = 0;

	for (size_t i = 0; i < numModes; i++) {
		if (displayModeList[i].Width == (size_t)screenWidth
			&& displayModeList[i].Height == (size_t)screenHeight) {
			numerator = displayModeList[i].RefreshRate.Numerator;
			denominator = displayModeList[i].RefreshRate.Denominator;
			break;
		}
	}

	// ����ī�� ����ü
	DXGI_ADAPTER_DESC adapterDesc;
	if (FAILED(adapter->GetDesc(&adapterDesc)))
		return false;

	// ����ī�� �޸� �뷮 ������ �ް�����Ʈ ������ ����
	videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// ����ī���� �̸� ����
	size_t stringLength = 0;
	if (wcstombs_s(&stringLength, videoCardDescription, 128, adapterDesc.Description, 128) != 0)
		return false;

	delete[] displayModeList;
	displayModeList = nullptr;

	adapterOutput->Release();
	adapterOutput = nullptr;

	adapter->Release();
	adapter = nullptr;

	factory->Release();
	factory = nullptr;

	// ����ü�� ����ü �ʱ�ȭ
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// ����� ��� ���� ����
	swapChainDesc.BufferCount = 1;

	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;

	// 32bit �����̽� ����
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	if (isVsyncEnable) {
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else {
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// ������� ���뵵 ����
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// �������� ���� ������ �ڵ�
	swapChainDesc.OutputWindow = hWnd;

	//��Ƽ���ø� ����
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	swapChainDesc.Windowed = !isFullScreen;

	// ��ĵ ���� ���� �� ũ�⸦ �������� �������� ����
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// ��µ� ���� ����۸� ��쵵�� ����
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// �߰� �ɼ� �÷��� ��� X
	swapChainDesc.Flags = 0;

	// ��ó������ DirectX 11�� ����
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

	if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1,
		D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, NULL, &deviceContext)))
		return false;

	// ����� ������
	ID3D11Texture2D* backBufferPtr = nullptr;
	if (FAILED(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)& backBufferPtr)))
		return false;

	// ����� �����ͷ� ���� Ÿ�� �並 ����
	if (FAILED(device->CreateRenderTargetView(backBufferPtr, NULL, &renderTargetView)))
		return false;

	// ����� ������ ����
	backBufferPtr->Release();
	backBufferPtr = nullptr;

	// ���� ���� ����ü �ʱ�ȭ
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// ������ ���� ���� ����ü�� ����Ͽ� ���� ���� �ؽ��� ����
	if (FAILED(device->CreateTexture2D(&depthBufferDesc, NULL, &depthStencilBuf)))
		return false;

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// �ȼ� ������ ���ٽ� ����
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// �ȼ� �޸��� ���ٽ� ����
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// ���� ���ٽ� ���� ����
	if (FAILED(device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState)))
		return false;

	// ���� ���ٽ� ���� ����
	deviceContext->OMSetDepthStencilState(depthStencilState, 1);

	// ���� ���ٽ� �� ����ü
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	if (FAILED(device->CreateDepthStencilView(depthStencilBuf, &depthStencilViewDesc, &depthStencilView)))
		return false;

	// ������ ��� ��� ���� ���ٽ� ���۸� ��� ���� ������ ���ο� ���ε�
	deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	if (FAILED(device->CreateRasterizerState(&rasterDesc, &rasterizerState)))
		return false;

	deviceContext->RSSetState(rasterizerState);

	D3D11_VIEWPORT viewport;
	viewport.Width = (float)screenWidth;
	viewport.Height = (float)screenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	deviceContext->RSSetViewports(1, &viewport);

	// ���� ��� ����
	float fieldOfView = 3.141592f / 4.0f;
	float screenAspect = (float)screenWidth / (float)screenHeight;

	// 3D �������� ���� ���� ��� ����
	projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);

	// ���� ����� �׵� ��ķ� �ʱ�ȭ
	worldMatrix = XMMatrixIdentity();

	// 2D �������� ���� ���� ���� ��� ����
	orthoMatrix = XMMatrixOrthographicLH((float)screenWidth, (float)screenHeight, screenNear, screenDepth);

	return true;
}

void GD3DClass::Shutdown() {
	// ���� �� ������ ���� �������� ������ ���� �� ���� �߻�
	if (swapChain)
		swapChain->SetFullscreenState(false, NULL);

	if (rasterizerState) {
		rasterizerState->Release();
		rasterizerState = nullptr;
	}

	if (depthStencilView) {
		depthStencilView->Release();
		depthStencilView = nullptr;
	}

	if (depthStencilState) {
		depthStencilState->Release();
		depthStencilState = nullptr;
	}

	if (depthStencilBuf) {
		depthStencilBuf->Release();
		depthStencilBuf = nullptr;
	}

	if (renderTargetView) {
		renderTargetView->Release();
		renderTargetView = nullptr;
	}

	if (deviceContext) {
		deviceContext->Release();
		deviceContext = nullptr;
	}

	if (device) {
		device->Release();
		device = nullptr;
	}

	if (swapChain) {
		swapChain->Release();
		swapChain = nullptr;
	}
}


void GD3DClass::BeginScene(float r, float g, float b, float a) {
	// ���۸� ���� �� ����
	float color[4] = { r,g,b,a };
	// ����� �����
	deviceContext->ClearRenderTargetView(renderTargetView, color);
	// ���� ���� �����
	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void GD3DClass::EndScene() {
	// ȭ�鿡 ����� ǥ��
	if (isVsyncEnable)
		swapChain->Present(1, 0);
	else
		swapChain->Present(0, 0);
}


ID3D11Device* GD3DClass::GetDevice() { return device; }

ID3D11DeviceContext* GD3DClass::GetDeviceContext() { return deviceContext; }

void GD3DClass::GetProjectionMatrix(XMMATRIX& projMat) { projMat = projectionMatrix; }

void GD3DClass::GetWorldMatrix(XMMATRIX& worldMat) { worldMat = worldMatrix; }

void GD3DClass::GetOrthoMatrix(XMMATRIX& orthMat) { orthMat = orthoMatrix; }

void GD3DClass::GetVideoCardInfo(char* cardName, int& mem) {
	strcpy_s(cardName, 128, videoCardDescription);
	mem = videoCardMemory;
}