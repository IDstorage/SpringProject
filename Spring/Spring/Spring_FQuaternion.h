#pragma once

#include "Spring_IProperty.h"
#include "Spring_FMath.h"


namespace spring {

	class FVector3;

	class XMFLOAT4;
	class XMVECTOR;

	class FQuaternion {

	public:
		IProperty<float, EIPropertySetting::GET> w, x, y, z;

	public:
		FQuaternion(float = 0.0f, float = 0.0f, float = 0.0f, float = 0.0f);

		FQuaternion(const FQuaternion&);

	public:
		XMFLOAT4 DXFloat4() const;
		XMVECTOR DXVector() const;

	public:
		FQuaternion& operator=(const FQuaternion&); 

		FQuaternion& operator*(const FQuaternion&);
		//FQuaternion& operator*(float); 

	public:
		static float Angle(const FQuaternion&, const FQuaternion&);

	public:
		static float Dot(const FQuaternion&, const FQuaternion&);

	public:
		static const FQuaternion& Lerp(const FQuaternion&, const FQuaternion&, float);
		static const FQuaternion& SLerp(const FQuaternion&, const FQuaternion&, float);

	public:
		static FQuaternion& Euler(const FVector3&);
		static FQuaternion& Euler(float, float, float);
	};

	using FEulerAngle = FQuaternion;

}