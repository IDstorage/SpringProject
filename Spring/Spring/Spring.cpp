// Spring.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "Spring.h"

#include "Spring_UGameEngine.h" 

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "D3DCompiler.lib")

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")  

#pragma warning(disable:4996) 

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
HWND targetHWnd;

bool keys[7] = { false }; 


// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


void RotateX(float deltaAngle) {
	DirectX::XMFLOAT3 e = spring::Renderer->GetRot();
	e.x += deltaAngle;
	spring::Renderer->SetModelRot(e);
}


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{

	spring::GEngine->InitializeGame(1280, 720, hInstance, hPrevInstance, lpCmdLine, nCmdShow, WndProc);

	//std::function<void()> f = spring::UInputBinder::Bind(Hello, 10);

	//f();

	spring::InputBinder->BindAction(spring::EK_Q, spring::IHotKeyStruct{ false, false, false }, spring::EKeyState::KS_PRESS, spring::UInputBinder::Bind(RotateX, 45.0f));

	MSG msg;

    // 기본 메시지 루프입니다:
    while (!spring::GEngine->GetCurrentState()->isGameEnd)//GetMessage(&msg, nullptr, 0, 0))
    {
		if (!spring::GEngine->CheckOneFrame())
			continue;

		MSG msg = spring::GEngine->GetWndMessage();

        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

		/*DirectX::XMFLOAT3 e = spring::Renderer->GetRot();
		float delta = 4.0f;
		if (keys[0])
			e.x += delta;
		if (keys[1])
			e.x -= delta;
		if (keys[2])
			e.y -= delta;
		if (keys[3])
			e.y += delta;
		if (keys[4])
			e.z -= delta;
		if (keys[5])
			e.z += delta;
		if (keys[6])
			e = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		spring::Renderer->SetModelRot(e);*/

		spring::GEngine->Tick(); 

		spring::Renderer->FrameRender();
    }

	spring::GEngine->Release();

    return (int)spring::GEngine->GetWndMessage().wParam;
}
 
//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
	{
 //   case WM_PAINT:
 //       {
 //           PAINTSTRUCT ps;
 //           HDC hdc = BeginPaint(hWnd, &ps);

 //           EndPaint(hWnd, &ps);
 //       }
 //       break;
	//case WM_KEYDOWN:
	//	
	//	switch (wParam) {
	//	// Key Q
	//	case 0x51:
	//		keys[0] = true;
	//		break;
	//	// Key A
	//	case 0x41:
	//		keys[1] = true;
	//		break;
	//	// Key W
	//	case 0x57:
	//		keys[2] = true;
	//		break;
	//	// Key S
	//	case 0x53:
	//		keys[3] = true;
	//		break; 
	//	// Key D
	//	case 0x44:
	//		keys[4] = true;
	//		break; 
	//	// Key E
	//	case 0x45:
	//		keys[5] = true;
	//		break;
	//	case VK_SPACE:
	//		keys[6] = true;
	//		break;
	//	}
	//	
	//	break;
	//case WM_KEYUP:
	//	switch (wParam) {
	//		// Key Q
	//	case 0x51:
	//		keys[0] = false;
	//		break;
	//		// Key A
	//	case 0x41:
	//		keys[1] = false;
	//		break;
	//		// Key W
	//	case 0x57:
	//		keys[2] = false;
	//		break;
	//		// Key S
	//	case 0x53:
	//		keys[3] = false;
	//		break;
	//		// Key D
	//	case 0x44:
	//		keys[4] = false;
	//		break;
	//		// Key E
	//	case 0x45:
	//		keys[5] = false;
	//		break;
	//	case VK_SPACE:
	//		keys[6] = false;
	//		break;
	//	}
	//	break;
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}