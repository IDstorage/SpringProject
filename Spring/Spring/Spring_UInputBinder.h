#pragma once

#include "Spring_HPreDef.h"
#include "Spring_HPreHeader.h"

namespace spring {

	enum EKey {
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

	enum EKeyState {

	};

	class UInputBinder {

		SINGLETON(UInputBinder)

	private: 
		struct IHotKeyStruct {
			bool isLeftShift;
			bool isLeftCtrl;
			bool isLeftAlt;
		};

	public:
		static void BindAction(EKey, IHotKeyStruct, EKeyState, )

	};

}