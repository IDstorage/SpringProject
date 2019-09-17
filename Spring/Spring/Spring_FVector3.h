#pragma once

#include "Spring_IProperty.h"
#include "Spring_FMath.h"

#include "Spring_GDX11Header.h"

namespace spring { 

	class FVector3 {

	public:
		IProperty<float, EIPropertySetting::GET_SET> x, y, z;

	public:
		FVector3(float = 0.0f, float = 0.0f, float = 0.0f);
		FVector3(const FVector3&);

	public:
		DirectX::XMFLOAT3 DXFloat3() const;
		DirectX::XMVECTOR DXVector() const;

	public:
		FVector3& operator=(const FVector3&);

		FVector3& operator+(const FVector3&);
		FVector3& operator+=(const FVector3&);

		FVector3& operator-(const FVector3&);
		FVector3& operator-=(const FVector3&);

		FVector3& operator*(const FVector3&);
		FVector3& operator*(float);
		FVector3& operator*=(const FVector3&);
		FVector3& operator*=(float);

		FVector3& operator/(const FVector3&);
		FVector3& operator/(float);
		FVector3& operator/=(const FVector3&);
		FVector3& operator/=(float);

	public:
		float Magnitude() const;
		const FVector3& Normalized() const;
		static const FVector3& Normalize(const FVector3&);

	public:
		static const FVector3& Cross(const FVector3&, const FVector3&);
		static float Dot(const FVector3&, const FVector3&);

	public:
		static const FVector3& Lerp(const FVector3&, const FVector3&, float);


	public:
		static IProperty<FVector3, EIPropertySetting::GET> zero;
		static IProperty<FVector3, EIPropertySetting::GET> one;

		static IProperty<FVector3, EIPropertySetting::GET> left;
		static IProperty<FVector3, EIPropertySetting::GET> right;

		static IProperty<FVector3, EIPropertySetting::GET> up;
		static IProperty<FVector3, EIPropertySetting::GET> down;

		static IProperty<FVector3, EIPropertySetting::GET> forward;
		static IProperty<FVector3, EIPropertySetting::GET> backward;
	};

	using FEulerAngle = FVector3;

}