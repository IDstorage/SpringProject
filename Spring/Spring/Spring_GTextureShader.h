#pragma once

#include "Spring_GDX11Header.h"
#include "Spring_HPreDef.h"

#include "Spring_IAlignedAllocationPolicy.h"

namespace spring {

	class GTextureShader : public IAlignedAllocationPolicy<16> {

		SINGLETON(GTextureShader)

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

		ID3D11SamplerState* sampleState;

		ID3D11DeviceContext* deviceContext;

	public:
		bool Initialize(ID3D11Device*, ID3D11DeviceContext*, HWND);
		void ShutdownShader();

	public:
		bool SetShaderParameters(DirectX::XMMATRIX, DirectX::XMMATRIX, DirectX::XMMATRIX, ID3D11ShaderResourceView*);
		bool Render(DirectX::XMMATRIX, DirectX::XMMATRIX, DirectX::XMMATRIX, int, ID3D11ShaderResourceView*);

	};

}