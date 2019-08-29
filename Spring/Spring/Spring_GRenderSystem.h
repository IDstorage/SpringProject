#pragma once

#include "Spring_GD3DClass.h"
#include "Spring_HPreDef.h"

namespace spring {

	// ¡Ø CAUTION ¡Ø
	class GRenderSystem {

		SINGLETON(GRenderSystem);

	private:
		const bool vSyncEnable;
		const bool isFullScreen;
		const float screenDepth;
		const float screenNear;

	public:
		static void Initialize(int, int, HWND);

		// About DX11
	private:
		GD3DClass* d3dClass;

	private:
		void Render();

	public:
		static void FrameRender();
		static void ShutdownRenderingSys();
	};

}