#include "framework.h"
#include "Spring.h"

#include "Spring_UGameEngine.h" 

#include <chrono>

using namespace spring;
using namespace std::chrono; 

extern spring::GRenderSystem* spring::Renderer;
extern spring::UInputBinder* spring::InputBinder;

spring::UGameEngine* spring::GEngine = spring::UGameEngine::MakeInst();


UGameEngine::UGameEngine() { }


bool UGameEngine::WindowInit(int width, int height, HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow, WNDPROC wndProc) {

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	szTitle = new WCHAR[MAX_LOADSTRING];
	szWindowClass = new WCHAR[MAX_LOADSTRING];

	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_SPRING, szWindowClass, MAX_LOADSTRING);

	// Window Class
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = wndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SPRING));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_SPRING);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	RegisterClassExW(&wcex);


	hInst = hInstance;

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, width, height, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
		return false;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	targetHWnd = hWnd;

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SPRING));

	return true;
}

bool UGameEngine::InitializeGame(int width, int height, HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow, WNDPROC wndProc) { 

	if (!WindowInit(width, height, hInstance, hPrevInstance, lpCmdLine, nCmdShow, wndProc))
		return false;

	currentState = new UGameState{
		60,
		0, 0, 0,
		false
	};

	InitializeStandardCamera();

	spring::InputBinder->Initialize(hInstance, targetHWnd);

	spring::Renderer->Initialize(width, height, targetHWnd);
}

bool UGameEngine::CheckOneFrame() {

	static system_clock::time_point prevTime = system_clock::now();
	static float frame = 1.0f / currentState->frameCount;

	duration<float> seconds = system_clock::now() - prevTime;

	float lag = seconds.count() - frame;

	if (lag >= 0.0f) {
		prevTime = system_clock::now();
		frame = 1.0f / currentState->frameCount - lag;
		return true;
	}

	return false;

}

void UGameEngine::Tick() {

	/*static system_clock::time_point prevTime = system_clock::now();

	duration<float> seconds = system_clock::now() - prevTime;

	if(seconds.count() >= 1.0f / currentState->frameCount) {
		prevTime = system_clock::now();

		
	} */

	spring::InputBinder->UpdateInput();

	spring::Renderer->FrameRender();
}

void UGameEngine::Release() {

	spring::Renderer->ShutdownRenderingSys();

}


const UGameEngine::UGameState* UGameEngine::GetCurrentState() const {
	return currentState;
}


MSG UGameEngine::GetWndMessage() {
	return windowMsg;
}

void UGameEngine::SendWndMessage(UINT msg) {
	SendMessage(targetHWnd, msg, 0, 0);
}


void UGameEngine::InitializeStandardCamera() { 
	mainCamera = new UCamera("MainCamera");
}

void UGameEngine::ChangeMainCamera(IPointer<UCamera>& cam) {
	mainCamera = cam;
}

IPointer<UCamera> UGameEngine::GetMainCamera() {
	return mainCamera;
}