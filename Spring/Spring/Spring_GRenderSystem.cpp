#include "Spring_GRenderSystem.h"

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

	if (instance->d3dClass->Initialize(screenWidth, screenHeight, instance->vSyncEnable, hWnd, instance->isFullScreen, instance->screenDepth, instance->screenNear))
		return;
}

void GRenderSystem::Render() {
	d3dClass->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);
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