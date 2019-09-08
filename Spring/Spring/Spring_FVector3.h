#pragma once

#include "Spring_IProperty.h"
#include "Spring_FMath.h"

namespace spring {

	class XMFLOAT3;
	class XMVECTOR;

	class FVector3 {

	public:
		IProperty<float, EIPropertySetting::GET> x, y, z;

	public:
		FVector3(float = 0.0f, float = 0.0f, float = 0.0f);
		FVector3(const FVector3&);

	public:
		XMFLOAT3 DXFloat3() const;
		XMVECTOR DXVector() const;

	public:
		FVector3& operator=(const FVector3&);

		FVector3& operator+(const FVector3&);
		FVector3& operator-(const FVector3&);
		FVector3& operator*(const FVector3&);
		FVector3& operator*(float);
		FVector3& operator/(const FVector3&);
		FVector3& operator/(float);

	public:
		float Magnitude() const;
		const FVector3& Normalized() const;
		static const FVector3& Normalize(const FVector3&);

	public:
		static const FVector3& Cross(const FVector3&, const FVector3&);
		static float Dot(const FVector3&, const FVector3&);

	public:
		static const FVector3& Lerp(const FVector3&, const FVector3&, float);
	};

	using FEulerAngle = FVector3;

}