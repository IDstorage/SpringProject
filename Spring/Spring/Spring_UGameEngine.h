#pragma once

#include "Spring_IPointer.h" 
#include "Spring_HPreDef.h"

#include "Spring_HMath.h"

#include "Spring_UFrameworks.h"
#include "Spring_GRenderSystem.h"
#include "Spring_UInputBinder.h"

#include "Spring_UCamera.h"

#define MAX_LOADSTRING 100

namespace spring {

	template <class _Typ>
	class IPointer;

	class UScene;
	class UCamera;

	class UGameEngine {

		//SINGLETON_NOFUNC(UGameEngine);
		UGameEngine();
	public:
		static UGameEngine* MakeInst() {
			return new UGameEngine();
		}

	private:
		HINSTANCE hInst;
		WCHAR* szTitle;
		WCHAR* szWindowClass;
		HWND targetHWnd;

		MSG windowMsg;

	private:  
		bool WindowInit(int, int, HINSTANCE, HINSTANCE, LPWSTR, int, WNDPROC);

	public:
		bool InitializeGame(int, int, HINSTANCE, HINSTANCE, LPWSTR, int, WNDPROC);

		bool CheckOneFrame();
		void Tick();

		void Release();

	public:
		MSG GetWndMessage();

		void SendWndMessage(UINT);

	public:
		struct UGameState {
			unsigned int frameCount;

			unsigned int objectCount;
			unsigned int layerCount;
			unsigned int sceneCount;

			bool isGameEnd;
		};

	private:
		UGameState* currentState, * prevState;

	public:
		const UGameState* GetCurrentState() const;

	private:
		IPointer<UScene> currentScene;

	public:
		void PushScene(IPointer<UScene>&);

		IPointer<UScene>& PopScene();

	public:
		void ReplaceScene(IPointer<UScene>&);

	private:
		IPointer<UCamera> mainCamera;

	public:
		void InitializeStandardCamera();
		void ChangeMainCamera(IPointer<UCamera>&);
		IPointer<UCamera> GetMainCamera();

	}; 

#ifndef UGAMEENGINE_DEFINE
#define UGAMEENGINE_DEFINE
	extern spring::UGameEngine* GEngine;
#endif  

} 
