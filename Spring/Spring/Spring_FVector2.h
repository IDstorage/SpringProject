#pragma once

#include "Spring_IProperty.h"
#include "Spring_FMath.h"

namespace spring {

	class XMFLOAT2;
	class XMFLOAT3;
	class XMVECTOR;

	class FVector2 {

	public:
		IProperty<float, EIPropertySetting::GET> x, y;

	public:
		FVector2(float = 0.0f, float = 0.0f);
		FVector2(const FVector2&);

	public:
		FVector2& operator=(const FVector2&);

		FVector2& operator+(const FVector2&);
		FVector2& operator-(const FVector2&);
		FVector2& operator*(const FVector2&);
		FVector2& operator*(float);
		FVector2& operator/(const FVector2&);
		FVector2& operator/(float);

	public:
		float Magnitude() const;
		const FVector2& Normalized() const;
		static const FVector2& Normalize(const FVector2&);

	public:
		static const FVector2& Cross(const FVector2&, const FVector2&);
		static float Dot(const FVector2&, const FVector2&);

	public:
		static const FVector2& Lerp(const FVector2&, const FVector2&, float);
	};

	using FSize = FVector2;

}