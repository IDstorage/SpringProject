#include "Spring_GRenderSystem.h"
#include "Spring_GColorShader.h"
#include "Spring_UGameEngine.h"

using namespace DirectX;
using namespace spring;

extern spring::UGameEngine* spring::GEngine;

spring::GRenderSystem* spring::Renderer = spring::GRenderSystem::MakeInst();
 

GRenderSystem::GRenderSystem() : vSyncEnable(false), isFullScreen(false),
									screenDepth(1000.0f), screenNear(0.1f),
									d3dClass(nullptr), testModel(nullptr) {}


void GRenderSystem::Initialize(int screenWidth, int screenHeight, HWND hWnd) {

	screenSize = FSize(screenWidth, screenHeight);

	d3dClass = (GD3DClass*)_aligned_malloc(sizeof(GD3DClass), 16);
	
	if (!d3dClass)
		return;

	if (!d3dClass->Initialize(screenWidth, screenHeight, vSyncEnable, hWnd, isFullScreen, screenDepth, screenNear))
		return; 

	testModel = new G3DModel();
	testModel->Initialize(d3dClass->GetDevice(), d3dClass->GetDeviceContext());

	if (!GColorShader::GetInstance()->Initialize(d3dClass->GetDevice(), d3dClass->GetDeviceContext(), hWnd))
		return;
}


void GRenderSystem::Render() {
	d3dClass->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);

	// 카메라 뷰 행렬 생성
	spring::GEngine->GetMainCamera()->Render();

	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	d3dClass->GetWorldMatrix(worldMatrix);
	spring::GEngine->GetMainCamera()->GetViewMatrix(viewMatrix);
	d3dClass->GetProjectionMatrix(projectionMatrix);

	testModel->Render(d3dClass->GetDeviceContext());

	GColorShader::GetInstance()->Render(worldMatrix, viewMatrix, projectionMatrix, testModel->GetIndexCount());

	d3dClass->EndScene(); 
}

void GRenderSystem::FrameRender() {
	Render();
}

void GRenderSystem::ShutdownRenderingSys() {
	if (d3dClass) {
		d3dClass->Shutdown();
		_aligned_free(d3dClass);
		d3dClass = nullptr;
	}
}



void GRenderSystem::SetModelRot(XMFLOAT3 e) {
	testModel->SetEulerAngle(e);
}

XMFLOAT3 GRenderSystem::GetRot() const {
	return testModel->GetEulerAngle();
}


FSize GRenderSystem::GetScreenSize() const {
	return screenSize;
}