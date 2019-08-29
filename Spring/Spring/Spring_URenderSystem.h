#pragma once

#include "Spring_GD3DClass.h"

namespace spring {

	// ¡Ø CAUTION ¡Ø
	class GRenderSystem {

	private:
		static GRenderSystem* instance;
		GRenderSystem() { Initialize(); }

	private:
		void Initialize();

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