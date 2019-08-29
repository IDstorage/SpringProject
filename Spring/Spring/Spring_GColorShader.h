#pragma once

#include "Spring_GDX11Header.h"
#include "Spring_HPreDef.h"

namespace spring {

	class GColorShader {

		SINGLETON(GColorShader)

	private:
		struct MatrixBufferType {
			DirectX::XMMATRIX world;
			DirectX::XMMATRIX view;
			DirectX::XMMATRIX projection;
		};

	private:
		ID3D11VertexShader* vertexShader;
		ID3D11PixelShader* pixelShader;
		ID3D11InputLayout* inputLayout;
		ID3D11Buffer* matrixBuffer;

	public:
		static bool Initialize(ID3D11Device*, HWND);
		static void ShutdownShader();

	public:
		bool SetShaderParameters(ID3D11DeviceContext*, DirectX::XMMATRIX, DirectX::XMMATRIX, DirectX::XMMATRIX);
		static bool Render(ID3D11DeviceContext*, DirectX::XMMATRIX, DirectX::XMMATRIX, DirectX::XMMATRIX, int);

	};

}