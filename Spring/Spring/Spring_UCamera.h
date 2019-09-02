#pragma once

#include "Spring_FVector2.h"
#include "Spring_FMatrix4x4.h"

#include "Spring_UFrameworks.h"

#include "Spring_GDX11Header.h"

#include "Spring_IAlignedAllocationPolicy.h"


namespace spring {

	class UCamera : public URef, public IAlignedAllocationPolicy<16> {

	private:
		std::string cameraName;

	private:
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 eulerAngle;
		//FQuaternion rotation;
		DirectX::XMMATRIX viewMatrix;

	public:
		UCamera(const std::string&);
		~UCamera();

	public:
		void SetPosition(float, float, float);
		void SetPosition(DirectX::XMFLOAT3);
		//void SetPosition(const FVector3&);

		void SetEulerAngle(float, float, float);
		void SetEulerAngle(DirectX::XMFLOAT3);
		//void SetEulerAngle(const FVector3&);
		//void SetRotation(const FQuaternion&);

	public:
		DirectX::XMFLOAT3 GetPositionDX() const;
		//FVector3 GetPosition() const;

		DirectX::XMFLOAT3 GetEulerAngleDX() const;
		//FVector3 GetEulerAngle() const;
		//FQuaternion GetRotation() const;

		void GetViewMatrix(DirectX::XMMATRIX&);

	public:
		void TEMP_RotateLittle(int, int);

	public:
		void Render();

	};

}