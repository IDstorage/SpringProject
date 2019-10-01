#pragma once

#include "Spring_HPreHeader.h"

#include "Spring_UFrameworks.h"

#include "Spring_FVector2.h"
#include "Spring_FVector3.h"

#include "Spring_GTexture.h"

#include <d2d1.h>
#include <dwrite.h>


namespace spring {

	class CSpriteRenderer : public UComponent {

	public:
		struct VertexType {
			FVector3 position;
			FVector2 texture;
		};

	private:
		ID3D11Buffer* vertexBuffer;
		ID3D11Buffer* indexBuffer;

		int vertexCount, indexCount;

		GTexture* texture;
		FSize size;

		ID3D11Device* device;
		ID3D11DeviceContext* deviceContext;

	public:
		CSpriteRenderer();
		CSpriteRenderer(const CSpriteRenderer&);
		~CSpriteRenderer();

	public:
		bool Initialize(const std::string&, const FSize&);
		void Release();

		void Render() override;

	private:
		bool LoadTexture(const std::string&);
		void ReleaseTexture();

	};

}