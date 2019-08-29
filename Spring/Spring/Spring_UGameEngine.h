#pragma once

#include "Spring_IPointer.h"
#include "Spring_HPreDef.h"

namespace spring {

	template <class _Typ>
	class IPointer;

	class UScene;

	class UGameEngine {

		SINGLETON(UGameEngine);

	public:
		struct UGameState {
			unsigned int frameCount;

			unsigned int objectCount;
			unsigned int layerCount;
			unsigned int sceneCount;
		};

	private:
		IPointer<UScene> currentScene;

	public:
		static void PushScene(IPointer<UScene>&);

		static IPointer<UScene>& PopScene();

	public:
		static void ReplaceScene(IPointer<UScene>&);

	};

}