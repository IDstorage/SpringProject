#include "Spring_UGameEngine.h"

using namespace spring;


UGameEngine* UGameEngine::instance = nullptr;

UGameEngine::UGameEngine() {}


void UGameEngine::InitializeStandardCamera() {
	if (instance == nullptr)
		instance = new UGameEngine();

	instance->mainCamera = new UCamera("MainCamera");
}


IPointer<UCamera> UGameEngine::GetMainCamera() {
	if (instance == nullptr)
		instance = new UGameEngine();

	return instance->mainCamera;
}