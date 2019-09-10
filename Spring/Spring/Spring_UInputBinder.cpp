#include "Spring_UInputBinder.h"
#include "Spring_GRenderSystem.h"

using namespace spring;

extern spring::GRenderSystem* spring::Renderer;

spring::UInputBinder* spring::InputBinder = spring::UInputBinder::MakeInst();


UInputBinder::UInputBinder() : dxInput(nullptr), keyboard(nullptr), mouse(nullptr) {}


bool UInputBinder::Initialize(HINSTANCE hInstance, HWND hWnd) {

	screenSize = Renderer->GetScreenSize();

	mousePosition = FVector2(0, 0);

	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dxInput, NULL)))
		return false;

	// Keyboard Initialize
	if (FAILED(dxInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL)))
		return false;

	if (FAILED(keyboard->SetDataFormat(&c_dfDIKeyboard)))
		return false;

	if (FAILED(keyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE)))
		return false;

	if (FAILED(keyboard->Acquire()))
		return false;

	// Mouse Initialize
	if (FAILED(dxInput->CreateDevice(GUID_SysMouse, &mouse, NULL)))
		return false;

	if (FAILED(mouse->SetDataFormat(&c_dfDIMouse)))
		return false;

	if (FAILED(mouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
		return false;

	if (FAILED(mouse->Acquire()))
		return false;

	return true;
}


void UInputBinder::ShutdownInput() {
	if (mouse) {
		mouse->Unacquire();
		mouse->Release();
		mouse = nullptr;
	}

	if (keyboard) {
		keyboard->Unacquire();
		keyboard->Release();
		keyboard = nullptr;
	}

	if (dxInput) {
		dxInput->Release();
		dxInput = nullptr;
	}
}


bool UInputBinder::ReadKeyboard() {
	if (keyboard == nullptr)
		return false;

	ZeroMemory(keyboardState, sizeof(keyboardState));
	HRESULT result = keyboard->GetDeviceState(sizeof(keyboardState), &keyboardState);

	if (FAILED(result)) {
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
			keyboard->Acquire();
		else
			return false;
	}

	return true;
}

bool UInputBinder::ReadMouse() {
	if (mouse == nullptr)
		return false;

	HRESULT result = mouse->GetDeviceState(sizeof(mouseState), (LPVOID)&mouseState);

	if (FAILED(result)) {
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
			mouse->Acquire();
		else
			return false;
	}

	return true;
}

bool UInputBinder::IsPressState(int index) {
	return (keyboardState[index] & 0x80);
}


bool UInputBinder::UpdateInput() {
	if (!ReadKeyboard())
		return false;

	if (!ReadMouse())
		return false;

	mousePosition = FVector2(FMath::Clamp(mousePosition.x + mouseState.lX, 0.0f, screenSize.x),
		FMath::Clamp(mousePosition.y + mouseState.lY, 0.0f, screenSize.y));

	currentHotkey.isLeftAlt = IsPressState(DIK_LALT);
	currentHotkey.isLeftCtrl = IsPressState(DIK_LCONTROL);
	currentHotkey.isLeftShift = IsPressState(DIK_LSHIFT);

	for (auto iter = keyMap.begin(); iter != keyMap.end(); ++iter) { 

		bool currentState = IsPressState(iter->first);
		bool previousState = iter->second->GetPreviousState() == EKeyState::KS_PRESS;

		UInputBinder::InputFuncInfo* info = iter->second;

		// Press Function
		if ((previousState == false) && (currentState == true))
			info->Execute(info->CanTrigger(currentHotkey, EKeyState::KS_PRESS));
		// Hold Function
		else if ((previousState == true) && (currentState == true))
			info->Execute(info->CanTrigger(currentHotkey, EKeyState::KS_HOLD));
		// Release Function
		else if ((previousState == true) && (currentState == false))
			info->Execute(info->CanTrigger(currentHotkey, EKeyState::KS_RELEASE));

		iter->second->UpdatePrevState(currentState ? EKeyState::KS_PRESS : EKeyState::KS_RELEASE);

	}
}

void UInputBinder::Check(EKeyCode code, IHotKeyStruct hotkey, EKeyState state) {
	if (keyMap.find(code) == keyMap.end())
		return;

	int index = keyMap[code]->CanTrigger(hotkey, state);

	if (index != -1)
		keyMap[code]->Execute(index);
}