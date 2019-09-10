#pragma once

#include "Spring_HPreDef.h"
#include "Spring_HPreHeader.h"

#include "Spring_HMath.h"

#include <functional>

#include <dinput.h>


namespace spring {

	enum EKeyCode {
		EK_SPACE = 0x20,

		EK_0 = DIK_0,
		EK_1 = DIK_1,
		EK_2 = DIK_2,
		EK_3 = DIK_3,
		EK_4 = DIK_4,
		EK_5 = DIK_5,
		EK_6 = DIK_6,
		EK_7 = DIK_7,
		EK_8 = DIK_8,
		EK_9 = DIK_9,

		EK_NUMPAD0 = DIK_NUMPAD0,
		EK_NUMPAD1 = DIK_NUMPAD1,
		EK_NUMPAD2 = DIK_NUMPAD2,
		EK_NUMPAD3 = DIK_NUMPAD3,
		EK_NUMPAD4 = DIK_NUMPAD4,
		EK_NUMPAD5 = DIK_NUMPAD5,
		EK_NUMPAD6 = DIK_NUMPAD6,
		EK_NUMPAD7 = DIK_NUMPAD7,
		EK_NUMPAD8 = DIK_NUMPAD8,
		EK_NUMPAD9 = DIK_NUMPAD9,

		EK_COMMA = DIK_COMMA,
		EK_PERIOD = DIK_PERIOD,
		EK_SEMICOLON = DIK_SEMICOLON,

		EK_A = DIK_A,
		EK_B = DIK_B,
		EK_C = DIK_C,
		EK_D = DIK_D,
		EK_E = DIK_E,
		EK_F = DIK_F,
		EK_G = DIK_G,
		EK_H = DIK_H,
		EK_I = DIK_I,
		EK_J = DIK_J,
		EK_K = DIK_K,
		EK_L = DIK_L,
		EK_M = DIK_M,
		EK_N = DIK_N,
		EK_O = DIK_O,
		EK_P = DIK_P,
		EK_Q = DIK_Q,
		EK_R = DIK_R,
		EK_S = DIK_S,
		EK_T = DIK_T,
		EK_U = DIK_U,
		EK_V = DIK_V,
		EK_W = DIK_W,
		EK_X = DIK_X,
		EK_Y = DIK_Y,
		EK_Z = DIK_Z
	};

	enum EKeyState : int {
		KS_PRESS,
		KS_HOLD,
		KS_RELEASE
	};

	struct IHotKeyStruct {
		bool isLeftShift;
		bool isLeftCtrl;
		bool isLeftAlt;
	};

	class UInputBinder {

		SINGLETON_NOFUNC(UInputBinder)

	private:
		LPDIRECTINPUT8 dxInput;
		LPDIRECTINPUTDEVICE8 keyboard;
		LPDIRECTINPUTDEVICE8 mouse;

		unsigned char keyboardState[256];
		DIMOUSESTATE mouseState;

		FSize screenSize;
		FVector2 mousePosition;

	private:
		using INPUT_BIND_FUNC = std::function<void()>;

	private: 
		IHotKeyStruct currentHotkey;

	private:
		class InputFuncInfo {
		private:
			INPUT_BIND_FUNC keyFunction;
			EKeyState trigState;
			IHotKeyStruct trigHotKey;

			EKeyState prevState;

		public:
			InputFuncInfo(INPUT_BIND_FUNC func, EKeyState state, IHotKeyStruct hotkey)
				: keyFunction(func), trigState(state), trigHotKey(hotkey), prevState(EKeyState::KS_RELEASE), nextInfo(nullptr) {}

		public:
			int CanTrigger(IHotKeyStruct hotkey, EKeyState state) {
				InputFuncInfo* head = this;
				int n = 0;

				while (head != nullptr) {
					if (state == trigState
						&& (hotkey.isLeftShift == trigHotKey.isLeftShift
							|| hotkey.isLeftCtrl == trigHotKey.isLeftCtrl
							|| hotkey.isLeftAlt == trigHotKey.isLeftAlt))
						return n;

					head = head->nextInfo;
					n++;
				}

				return -1;
			}

			void Execute(int index) {
				if (index == -1)
					return;

				InputFuncInfo* head = this;

				while (index-- > 0)
					head = head->nextInfo;

				if (head != nullptr)
					head->keyFunction();
			}

		private:
			InputFuncInfo* nextInfo;

		public:
			void AddInputFuncInfo(InputFuncInfo* info) {
				if (nextInfo == nullptr)
					nextInfo = info;

				nextInfo->AddInputFuncInfo(info);
			}

			InputFuncInfo* GetNextInfo() {
				return nextInfo;
			}

		public:
			void UpdatePrevState(EKeyState prev) {
				prevState = prev;
			}

			EKeyState GetPreviousState() {
				return prevState;
			}
		};

		std::map<EKeyCode, InputFuncInfo*> keyMap;

	public:
		template <class _Func, class... _Args>
		static auto Bind(_Func f, _Args... args) {
			return std::bind(f, args...);
		}
		template <class _Func>
		static auto Bind(_Func f) {
			return f;
		}

	public:
		void BindAction(EKeyCode code, IHotKeyStruct hotkey, EKeyState state, INPUT_BIND_FUNC func) {
			if (keyMap.find(code) == keyMap.end()) {
				keyMap[code] = new InputFuncInfo(func, state, hotkey);
				return;
			}

			keyMap[code]->AddInputFuncInfo(new InputFuncInfo(func, state, hotkey));
		}

	private:
		bool ReadKeyboard();
		bool ReadMouse();

		bool IsPressState(int);

	public:
		bool Initialize(HINSTANCE, HWND);

		bool UpdateInput();
		void Check(EKeyCode, IHotKeyStruct, EKeyState);

		void ShutdownInput();

	}; 

#ifndef UINPUTBINDER_DEFINE
#define UINPUTBINDER_DEFINE
	extern UInputBinder* InputBinder;
#endif  
}
 