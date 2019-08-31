#pragma once

#include "Spring_GDX11Header.h"
#include "Spring_HPreDef.h"

#include "Spring_IAlignedAllocationPolicy.h"

namespace spring {

	class GColorShader : public IAlignedAllocationPolicy<16> {

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

		ID3D11DeviceContext* deviceContext;

	public:
		static bool Initialize(ID3D11Device*, ID3D11DeviceContext*, HWND);
		static void ShutdownShader();

	public:
		bool SetShaderParameters(DirectX::XMMATRIX, DirectX::XMMATRIX, DirectX::XMMATRIX);
		static bool Render(DirectX::XMMATRIX, DirectX::XMMATRIX, DirectX::XMMATRIX, int);

	};

}