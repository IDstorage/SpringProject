#include "Spring_FQuaternion.h"

using namespace DirectX;
using namespace spring;


FQuaternion::FQuaternion(float _w, float _x, float _y, float _z) 
	: w(_w), x(_x), y(_y), z(_z) {}

FQuaternion::FQuaternion(const FQuaternion& q)
	: w(q.w), x(q.x), y(q.y), z(q.z) {}


XMFLOAT4 FQuaternion::DXFloat4() const {
	return XMFLOAT4(w, x, y, z);
}

XMVECTOR FQuaternion::DXVector() const {
	return XMLoadFloat4(&DXFloat4());
}


FQuaternion& FQuaternion::operator=(const FQuaternion& q) {
	this->w = q.w;
	this->x = q.x;
	this->y = q.y;
	this->z = q.z;
	return *this;
}

//FQuaternion& FQuaternion::operator*(const FQuaternion& q)


FQuaternion FQuaternion::Euler(const FVector3& v) {
	return Euler(v.x, v.y, v.z);
}

FQuaternion FQuaternion::Euler(float _x, float _y, float _z) {
	XMFLOAT3 yawRollPith = XMFLOAT3(_y, _z, _x);

	float sinPith = FMath::Sin_Deg(yawRollPith.x / 2), cosPith = FMath::Cos_Deg(yawRollPith.x / 2);
	float sinYaw = FMath::Sin_Deg(yawRollPith.y / 2), cosYaw = FMath::Cos_Deg(yawRollPith.y / 2);
	float sinRoll = FMath::Sin_Deg(yawRollPith.z / 2), cosRoll = FMath::Cos_Deg(yawRollPith.z / 2);

	FQuaternion q = FQuaternion(
		sinRoll * cosPith * cosYaw - cosRoll * sinPith * sinYaw,
		cosRoll * sinPith * cosYaw + sinRoll * cosPith * sinYaw,
		cosRoll * cosPith * sinYaw - sinRoll * sinPith * cosYaw,
		cosRoll * cosPith * cosYaw + sinRoll * sinPith * sinYaw
	);

	return q;
}