#include "Spring_FVector3.h"

using namespace DirectX;
using namespace spring;


IProperty<FVector3, EIPropertySetting::GET> FVector3::zero = FVector3(0.0f, 0.0f, 0.0f);
IProperty<FVector3, EIPropertySetting::GET> FVector3::one = FVector3(1.0f, 1.0f, 1.0f);

IProperty<FVector3, EIPropertySetting::GET> FVector3::left = FVector3(-1.0f, 0.0f, 0.0f);
IProperty<FVector3, EIPropertySetting::GET> FVector3::right = FVector3(1.0f, 0.0f, 0.0f);

IProperty<FVector3, EIPropertySetting::GET> FVector3::up = FVector3(0.0f, 1.0f, 0.0f);
IProperty<FVector3, EIPropertySetting::GET> FVector3::down = FVector3(0.0f, -1.0f, 0.0f);

IProperty<FVector3, EIPropertySetting::GET> FVector3::forward = FVector3(0.0f, 0.0f, 1.0f);
IProperty<FVector3, EIPropertySetting::GET> FVector3::backward = FVector3(0.0f, 0.0f, -1.0f);


FVector3::FVector3(float _x, float _y, float _z)
	: x(_x), y(_y), z(_z) {}

FVector3::FVector3(const FVector3& v)
	: x(v.x), y(v.y), z(v.z) {}


XMFLOAT3 FVector3::DXFloat3() const {
	return XMFLOAT3(x, y, z);
}

XMVECTOR FVector3::DXVector() const {
	return XMLoadFloat3(&DXFloat3());
}


FVector3& FVector3::operator=(const FVector3& v) {
	this->x = v.x;
	this->y = v.y;
	return *this;
}

FVector3& FVector3::operator+(const FVector3& v) {
	FVector3 newV = FVector3(this->x + v.x, this->y + v.y);
	return newV;
}

FVector3& FVector3::operator+=(const FVector3& v) {
	this->x = this->x + v.x;
	this->y = this->y + v.y;
	this->z = this->z + v.z;
	return *this;
}

FVector3& FVector3::operator-(const FVector3& v) {
	FVector3 newV = FVector3(this->x - v.x, this->y - v.y);
	return newV;
}

FVector3& FVector3::operator*(const FVector3& v) {
	FVector3 newV = FVector3(this->x * v.x, this->y * v.y);
	return newV;
}

FVector3& FVector3::operator*=(const FVector3& v) {
	this->x = this->x * v.x;
	this->y = this->y * v.y;
	this->z = this->z * v.z;
	return *this;
}

FVector3& FVector3::operator*(float value) {
	FVector3 newV = FVector3(this->x * value, this->y * value);
	return newV;
}

FVector3& FVector3::operator/(const FVector3& v) {
	FVector3 newV = FVector3(this->x / v.x, this->y / v.y);
	return newV;
}

FVector3& FVector3::operator/(float value) {
	FVector3 newV = FVector3(this->x / value, this->y / value);
	return newV;
}


float FVector3::Magnitude() const {
	return sqrt(x * x + y * y + z * z);
}

const FVector3& FVector3::Normalized() const {
	float mag = Magnitude();
	return FVector3(x / mag, y / mag, z / mag);
}

const FVector3& FVector3::Normalize(const FVector3& v) {
	float mag = v.Magnitude();
	return FVector3(v.x / mag, v.y / mag, v.z / mag);
}


const FVector3& FVector3::Cross(const FVector3& v1, const FVector3& v2) {
	return FVector3();
}

float FVector3::Dot(const FVector3& v1, const FVector3& v2) {
	return 0.0f;
}


const FVector3& FVector3::Lerp(const FVector3& v1, const FVector3& v2, float t) {
	return FVector3(FMath::Lerp(v1.x, v2.x, t), FMath::Lerp(v1.y, v2.y, t), FMath::Lerp(v1.z, v2.z, t));
}