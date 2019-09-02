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

	// 수직동기화 상태
	isVsyncEnable = vsync;

	// DirectX 그래픽 인터페이스 팩토리
	IDXGIFactory* factory = nullptr;
	if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)& factory)))
		return false;

	// 팩토리 객체를 사용하여 첫번째 그래픽 카드 인터페이스 어뎁터 생성
	IDXGIAdapter* adapter = nullptr;
	if (FAILED(factory->EnumAdapters(0, &adapter)))
		return false;

	// 출력(모니터)에 대한 첫번째 어뎁터를 지정함
	IDXGIOutput* adapterOutput = nullptr;
	if (FAILED(adapter->EnumOutputs(0, &adapterOutput)))
		return false;

	// 출력에 대한 DXGI_FORMAT_R8G8B8A8_UNORM 표시 형식에 맞는 모드 수를 가져옴
	unsigned int numModes = 0;
	if (FAILED(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED,
		&numModes, NULL)))
		return false;

	// 가능한 모든 모니터와 그래픽카드 조합을 저장할 리스트 생성
	DXGI_MODE_DESC* displayModeList = new DXGI_MODE_DESC[100];
	if (!displayModeList)
		return false;

	// 디스플레이 모드에 대한 리스트를 채움
	if (FAILED(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED,
		&numModes, displayModeList)))
		return false;

	// 모든 디스플레이 모드에 대해 화면 너비/높이에 맞는 디스플레이 모드를 찾음
	// 모니터의 새로고침 비율의 분모와 분자 저장
	size_t numerator = 0, denominator = 0;

	for (size_t i = 0; i < numModes; i++) {
		if (displayModeList[i].Width == (size_t)screenWidth
			&& displayModeList[i].Height == (size_t)screenHeight) {
			numerator = displayModeList[i].RefreshRate.Numerator;
			denominator = displayModeList[i].RefreshRate.Denominator;
			break;
		}
	}

	// 비디오카드 구조체
	DXGI_ADAPTER_DESC adapterDesc;
	if (FAILED(adapter->GetDesc(&adapterDesc)))
		return false;

	// 비디오카드 메모리 용량 단위를 메가바이트 단위로 저장
	videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// 비디오카드의 이름 저장
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

	// 스왑체인 구조체 초기화
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// 백버퍼 사용 개수 지정
	swapChainDesc.BufferCount = 1;

	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;

	// 32bit 서페이스 설정
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	if (isVsyncEnable) {
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else {
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// 백버퍼의 사용용도 지정
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// 렌더링에 사용될 윈도우 핸들
	swapChainDesc.OutputWindow = hWnd;

	//멀티샘플링 끄기
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	swapChainDesc.Windowed = !isFullScreen;

	// 스캔 라인 순서 및 크기를 지정하지 않음으로 설정
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// 출력된 다음 백버퍼를 비우도록 지정
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// 추가 옵션 플래그 사용 X
	swapChainDesc.Flags = 0;

	// 피처레벨을 DirectX 11로 설정
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

	if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1,
		D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, NULL, &deviceContext)))
		return false;

	// 백버퍼 포인터
	ID3D11Texture2D* backBufferPtr = nullptr;
	if (FAILED(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)& backBufferPtr)))
		return false;

	// 백버퍼 포인터로 렌더 타겟 뷰를 생성
	if (FAILED(device->CreateRenderTargetView(backBufferPtr, NULL, &renderTargetView)))
		return false;

	// 백버퍼 포인터 해제
	backBufferPtr->Release();
	backBufferPtr = nullptr;

	// 깊이 버퍼 구조체 초기화
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

	// 설정된 깊이 버퍼 구조체를 사용하여 깊이 버퍼 텍스쳐 생성
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

	// 픽셀 정면의 스텐실 설정
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// 픽셀 뒷면의 스텐실 설정
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// 깊이 스텐실 상태 생성
	if (FAILED(device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState)))
		return false;

	// 깊이 스텐실 상태 설정
	deviceContext->OMSetDepthStencilState(depthStencilState, 1);

	// 깊이 스텐실 뷰 구조체
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	if (FAILED(device->CreateDepthStencilView(depthStencilBuf, &depthStencilViewDesc, &depthStencilView)))
		return false;

	// 렌더링 대상 뷰와 깊이 스텐실 버퍼를 출력 렌더 파이프 라인에 바인딩
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

	// 투영 행렬 설정
	float fieldOfView = 3.141592f / 4.0f;
	float screenAspect = (float)screenWidth / (float)screenHeight;

	// 3D 렌더링을 위한 투영 행렬 생성
	projectionMatrix = XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);

	// 세계 행렬을 항등 행렬로 초기화
	worldMatrix = XMMatrixIdentity();

	// 2D 렌더링을 위한 직교 투영 행렬 생성
	orthoMatrix = XMMatrixOrthographicLH((float)screenWidth, (float)screenHeight, screenNear, screenDepth);

	return true;
}

void GD3DClass::Shutdown() {
	// 종료 전 윈도우 모드로 설정하지 않으면 해제 시 예외 발생
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
	// 버퍼를 지울 색 설정
	float color[4] = { r,g,b,a };
	// 백버퍼 지우기
	deviceContext->ClearRenderTargetView(renderTargetView, color);
	// 깊이 버퍼 지우기
	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void GD3DClass::EndScene() {
	// 화면에 백버퍼 표시
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