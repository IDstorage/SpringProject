#pragma once

#include "Spring_GDX11Header.h"
#include "Spring_IPointer.h"

#include "Spring_GTexture.h"

#include "Spring_FVector3.h"
#include "Spring_FVector2.h"

#include "Spring_IAlignedAllocationPolicy.h"

#include <vector>

namespace spring { 

	class G3DModel : public IAlignedAllocationPolicy<16> {

	public:
		struct VertexType {
			DirectX::XMFLOAT3 position;
			DirectX::XMFLOAT2 texture;
		};

	public:
		G3DModel();
		G3DModel(const G3DModel&);
		~G3DModel();

	public:
		using VertexListType = std::initializer_list<VertexType>;
		using IndexListType = std::initializer_list<int>;
		         
	private:
		ID3D11Buffer* vertexBuffer = nullptr;
		ID3D11Buffer* indexBuffer = nullptr;

		int vertexCount = 0;
		int indexCount = 0;

		std::vector<VertexType> vertexList;

		FVector3 position;
		FEulerAngle eulerAngle;
		FVector3 localScale;

		FVector3 lookAtVector;

		ID3D11Device* device;
		ID3D11DeviceContext* deviceContext;

		GTexture* targetTexture;

	public:
		static IPointer<G3DModel>& Create3DModel();

	public:
		bool Initialize(const VertexListType&, const IndexListType&, char*);
		void Release();
		void Render();

	public:
		bool LoadTexture(char*);
		void ReleaseTexture();

	public:
		int GetIndexCount() const;
		ID3D11ShaderResourceView* GetTexture() const;

	private:
		bool UpdateVertexBuffer();

	public:
		void SetPosition(float, float, float);
		void SetPosition(const FVector3&);
		void SetPositionDX(DirectX::XMFLOAT3);

	private:
		FVector3 GetRotationVector(const FVector3&);

	public:
		void SetEulerAngle(float, float, float);
		void SetEulerAngle(const FVector3&);
		void SetEulerAngle(DirectX::XMFLOAT3);
		//void SetRotation(FQuaternion);

		FEulerAngle GetEulerAngle() const;

	private:
		FVector3 GetScaleVector(const FVector3&);

	public:
		void SetLocalScale(float, float, float);
		void SetLocalScale(const FVector3&);
		void SetLocalScale(DirectX::XMFLOAT3);

	};

}