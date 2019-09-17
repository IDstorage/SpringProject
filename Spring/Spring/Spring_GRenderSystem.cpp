#include "Spring_GRenderSystem.h"
#include "Spring_GColorShader.h"
#include "Spring_GTextureShader.h"
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
	char filePath[] = "./test_texture.tga";
	testModel->Initialize({
			G3DModel::VertexType { FVector3(-1.0f, -1.0f, 0.0f), FVector2(0.0f, 1.0f) },
			G3DModel::VertexType { FVector3(0.0f, 1.0f, 0.0f), FVector2(0.5f, 0.0f) },
			G3DModel::VertexType { FVector3(1.0f, -1.0f, 0.0f), FVector2(1.0f, 1.0f) }
		}, {
			0, 1, 2
		},
		filePath);

	if (!GTextureShader::GetInstance()->Initialize(d3dClass->GetDevice(), d3dClass->GetDeviceContext(), hWnd))
		return;
}


GD3DClass* GRenderSystem::GetD3DClass() const {
	return d3dClass;
}


void GRenderSystem::Render() {
	d3dClass->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);

	// 카메라 뷰 행렬 생성
	spring::GEngine->GetMainCamera()->Render();

	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	d3dClass->GetWorldMatrix(worldMatrix);
	spring::GEngine->GetMainCamera()->GetViewMatrix(viewMatrix);
	d3dClass->GetProjectionMatrix(projectionMatrix);

	testModel->Render();

	if (!GTextureShader::GetInstance()->Render(worldMatrix, viewMatrix, projectionMatrix, testModel->GetIndexCount(), testModel->GetTexture()))
		return;

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
	return testModel->GetEulerAngle().DXFloat3();
}


FSize GRenderSystem::GetScreenSize() const {
	return screenSize;
}