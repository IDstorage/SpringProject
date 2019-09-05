#pragma once

#include "Spring_HPreDef.h"
#include "Spring_HPreHeader.h"

#include <functional>

namespace spring {

	enum EKeyCode {
		EK_SPACE = 0x20,

		EK_N0,
		EK_N1,
		EK_N2,
		EK_N3,
		EK_N4,
		EK_N5,
		EK_N6,
		EK_N7,
		EK_N8,
		EK_N9,

		EK_COMMA,
		EK_PERIOD,
		EK_SEMICOLON,

		EK_A = 0x41,
		EK_B,
		EK_C,
		EK_D,
		EK_E,
		EK_F,
		EK_G,
		EK_H,
		EK_I,
		EK_J,
		EK_K,
		EK_L,
		EK_M,
		EK_N,
		EK_O,
		EK_P,
		EK_Q,
		EK_R,
		EK_S,
		EK_T,
		EK_U,
		EK_V,
		EK_W,
		EK_X,
		EK_Y,
		EK_Z,
	};

	enum EKeyState : int {
		KS_PRESS,
		KS_HOLD,
		KS_RELEASE
	};

	class UInputBinder {

		SINGLETON(UInputBinder)

	private:
		using INPUT_BIND_FUNC = std::function<void()>;

	private:
		struct IHotKeyStruct {
			bool isLeftShift;
			bool isLeftCtrl;
			bool isLeftAlt;
		};

	private:
		class InputFuncInfo {
		private:
			INPUT_BIND_FUNC keyFunction;
			EKeyState trigState;
			IHotKeyStruct trigHotKey;

		public:
			InputFuncInfo(INPUT_BIND_FUNC func, EKeyState state, IHotKeyStruct hotkey)
				: keyFunction(func), trigState(state), trigHotKey(hotkey) {}

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
				InputFuncInfo* head = this;

				while (index-- > 0)  
					head = head->nextInfo; 

				if(head != nullptr)
					head->keyFunction();
			}

		private:
			InputFuncInfo* nextInfo;

		public:
			void AddInputFuncInfo(InputFuncInfo* info) {
				if (nextInfo == null)
					nextInfo = info;

				nextInfo->AddInputFuncInfo(info);
			}

			InputFuncInfo* GetNextInfo() {
				return nextInfo;
			}
		};

		std::map<EKeyCode, InputFuncInfo*> keyMap;

	public:
		template <class _Func, class... _Args>
		static INPUT_BIND_FUNC&& Bind(_Func f, _Args... args) {
			return std::bind(f, args...);
		}

	public:
		static void BindAction(EKeyCode code, IHotKeyStruct hotkey, EKeyState state, INPUT_BIND_FUNC func) {
			if (instance->keyMap.find(code) == instance->keyMap.end()) {
				instance->keyMap[code] = new InputFuncInfo(func, state, hotkey);
				return;
			}

			instance->keyMap[code]->AddInputFuncInfo(new InputFuncInfo(func, state, hotkey));
		}

	public:
		static void Update(EKeyCode code, IHotKeyStruct hotkey, EKeyState state) {
			if (instance->keyMap.find(code) == instance->keyMap.end())
				return;

			int index = instance->keyMap[code]->CanTrigger(hotkey, state);

			if(index != -1)
				instance->keyMap[code]->Execute();
		}

	};

}