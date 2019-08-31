#include "Spring_GRenderSystem.h"
#include "Spring_GColorShader.h"
#include "Spring_UGameEngine.h"

using namespace DirectX;
using namespace spring;


GRenderSystem* GRenderSystem::instance = nullptr;

GRenderSystem::GRenderSystem() : vSyncEnable(true), isFullScreen(false),
									screenDepth(1000.0f), screenNear(0.1f) {}


void GRenderSystem::Initialize(int screenWidth, int screenHeight, HWND hWnd) {
	if (!instance)
		instance = new GRenderSystem();

	instance->d3dClass = (GD3DClass*)_aligned_malloc(sizeof(GD3DClass), 16);
	
	if (!instance->d3dClass)
		return;

	if (!instance->d3dClass->Initialize(screenWidth, screenHeight, instance->vSyncEnable, hWnd, instance->isFullScreen, instance->screenDepth, instance->screenNear))
		return;


	UGameEngine::InitializeStandardCamera();

	if (GColorShader::Initialize(instance->d3dClass->GetDevice(), instance->d3dClass->GetDeviceContext(), hWnd))
		return;

	instance->testModel = new G3DModel();
	instance->testModel->Initialize(instance->d3dClass->GetDevice());
}

void GRenderSystem::InitializeDX(ID3D11Device* d, ID3D11DeviceContext* dc, HWND h) {
	instance->device = d;
	instance->deviceContext = dc;
	instance->hWnd = h;
}


void GRenderSystem::Render() {
	d3dClass->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);

	UGameEngine::GetMainCamera()->Render();

	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	d3dClass->GetWorldMatrix(worldMatrix);
	UGameEngine::GetMainCamera()->GetViewMatrix(viewMatrix);
	d3dClass->GetProjectionMatrix(projectionMatrix);

	testModel->Render(deviceContext);

	GColorShader::Render(worldMatrix, viewMatrix, projectionMatrix, testModel->GetIndexCount());

	d3dClass->EndScene();
}

void GRenderSystem::FrameRender() {
	instance->Render();
}

void GRenderSystem::ShutdownRenderingSys() {
	if (instance->d3dClass) {
		instance->d3dClass->Shutdown();
		_aligned_free(instance->d3dClass);
		instance->d3dClass = nullptr;
	}
}