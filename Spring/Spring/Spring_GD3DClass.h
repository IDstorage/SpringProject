#pragma once

#include "Spring_GDX11Header.h"

namespace spring {

	class GD3DClass {

	private:
		bool isVsyncEnable = false;

		int videoCardMemory;
		char videoCardDescription[128];

		IDXGISwapChain* swapChain;

		ID3D11Device* device;
		ID3D11DeviceContext* deviceContext;

		ID3D11RenderTargetView* renderTargetView;

		ID3D11Texture2D* depthStencilBuf;
		ID3D11DepthStencilState* depthStencilState;
		ID3D11DepthStencilView* depthStencilView;

		ID3D11RasterizerState* rasterizerState;

		DirectX::XMMATRIX projectionMatrix;
		DirectX::XMMATRIX worldMatrix;
		DirectX::XMMATRIX orthoMatrix;

	public:
		GD3DClass();
		GD3DClass(const GD3DClass&);
		~GD3DClass();

	public:
		bool Initialize(int, int, bool, HWND, bool, float, float);

		void Shutdown();

		void BeginScene(float, float, float, float);
		void EndScene();

	public:
		ID3D11Device* GetDevice();
		ID3D11DeviceContext* GetDeviceContext();

		void GetProjectionMatrix(DirectX::XMMATRIX&);
		void GetWorldMatrix(DirectX::XMMATRIX&);
		void GetOrthoMatrix(DirectX::XMMATRIX&);

		void GetVideoCardInfo(char*, int&);

	};

}