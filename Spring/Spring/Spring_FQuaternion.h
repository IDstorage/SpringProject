#pragma once

#include "Spring_IProperty.h" 
#include "Spring_GDX11Header.h"

#include "Spring_FMath.h"

#include "Spring_FVector3.h"


namespace spring { 

	class FQuaternion {

	public:
		IProperty<float, EIPropertySetting::GET> w, x, y, z;

	public:
		FQuaternion(float = 0.0f, float = 0.0f, float = 0.0f, float = 0.0f);

		FQuaternion(const FQuaternion&);

	public:
		DirectX::XMFLOAT4 DXFloat4() const;
		DirectX::XMVECTOR DXVector() const;

	public:
		FQuaternion& operator=(const FQuaternion&); 

		FQuaternion& operator*(const FQuaternion&);
		//FQuaternion& operator*(float); 

	public:
		static float Angle(const FQuaternion&, const FQuaternion&);

	public:
		static float Dot(const FQuaternion&, const FQuaternion&);

	public:
		static const FQuaternion Lerp(const FQuaternion&, const FQuaternion&, float);
		static const FQuaternion SLerp(const FQuaternion&, const FQuaternion&, float);

	public:
		static FQuaternion Euler(const FVector3&);
		static FQuaternion Euler(float, float, float);

		static FVector3 ToEulerAngle(const FQuaternion&);
	}; 

}