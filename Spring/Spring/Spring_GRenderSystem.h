#pragma once

#include "Spring_GD3DClass.h"
#include "Spring_G3DModel.h"

#include "Spring_HPreDef.h"

#include "Spring_HMath.h"

namespace spring {

	// ¡Ø CAUTION ¡Ø
	class GRenderSystem {

		SINGLETON_NOFUNC(GRenderSystem);

	private:
		const bool vSyncEnable;
		const bool isFullScreen;
		const float screenDepth;
		const float screenNear;

		FSize screenSize;

	public:
		void Initialize(int, int, HWND);

		// About DX11
	private:
		GD3DClass* d3dClass;
		G3DModel* testModel;

	private:
		void Render();

	public:
		void FrameRender();
		void ShutdownRenderingSys();

	public:
		void SetModelRot(DirectX::XMFLOAT3);
		DirectX::XMFLOAT3 GetRot() const;

	public:
		FSize GetScreenSize() const;
	}; 

#ifndef GRENDERSYSTEM_DEFINE
#define GRENDERSYSTEM_DEFINE
	extern spring::GRenderSystem* Renderer;
#endif  

} 