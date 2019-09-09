#pragma once

#include "Spring_GDX11Header.h"
#include "Spring_IPointer.h"

#include "Spring_IAlignedAllocationPolicy.h"

namespace spring {

	class FQuaternion;

	class G3DModel : public IAlignedAllocationPolicy<16> {

		struct VertexType {
			DirectX::XMFLOAT3 position;
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

		DirectX::XMFLOAT3 vertexList[8];

		DirectX::XMFLOAT3 vertVectorList[8];
		DirectX::XMFLOAT3 basicLookRot;

		DirectX::XMFLOAT3 vertexBasicAngle[8];

		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 eulerAngle;

		DirectX::XMFLOAT3 lookAtVector;

		ID3D11Device* device;
		ID3D11DeviceContext* deviceContext;

	public:
		static IPointer<G3DModel>& Create3DModel();

	public:
		bool Initialize(ID3D11Device*, ID3D11DeviceContext*);
		void Release();
		void Render(ID3D11DeviceContext*);

	public:
		int GetIndexCount() const;

	public:
		void SetPosition(float, float, float);
		void SetPositionDX(DirectX::XMFLOAT3);

	private:
		DirectX::XMFLOAT3 RotateX(DirectX::XMFLOAT3, float);
		DirectX::XMFLOAT3 RotateY(DirectX::XMFLOAT3, float);
		DirectX::XMFLOAT3 RotateZ(DirectX::XMFLOAT3, float);

		DirectX::XMFLOAT3 ChangeToSphericalCoord(int);

	public:
		void SetEulerAngle(float, float, float);
		void SetEulerAngle(DirectX::XMFLOAT3);
		//void SetRotation(FQuaternion);

		DirectX::XMFLOAT3 GetEulerAngle() const;

	};

}