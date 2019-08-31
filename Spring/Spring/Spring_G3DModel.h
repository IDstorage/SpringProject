#pragma once

#include "Spring_GDX11Header.h"
#include "Spring_IPointer.h"

#include "Spring_IAlignedAllocationPolicy.h"

namespace spring {

	class G3DModel : public IAlignedAllocationPolicy<16> {

		struct VertexType {
			DirectX::XMFLOAT4 position;
			DirectX::XMFLOAT4 color;
		};

	public:
		G3DModel();
		G3DModel(const G3DModel&);
		~G3DModel();

	private:
		ID3D11Buffer* vertexBuffer = nullptr;
		ID3D11Buffer* indexBuffer = nullptr;

		int vertexCount = 0;
		int indexCount = 0;

	public:
		static IPointer<G3DModel>& Create3DModel();

	public:
		bool Initialize(ID3D11Device*);
		void Release();
		void Render(ID3D11DeviceContext*);

	public:
		int GetIndexCount() const;

	};

}