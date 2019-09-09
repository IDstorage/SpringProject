#include "Spring_FVector2.h"

using namespace DirectX;
using namespace spring;


FVector2::FVector2(float _x, float _y)
	: x(_x), y(_y) {}

FVector2::FVector2(const FVector2& v)
	: x(v.x), y(v.y) {}


XMFLOAT2 FVector2::DXFloat2() const {
	return XMFLOAT2(x, y);
}

XMVECTOR FVector2::DXVector() const {
	return XMLoadFloat2(&DXFloat2());
}


FVector2& FVector2::operator=(const FVector2& v) {
	this->x = v.x;
	this->y = v.y;
	return *this;
}

FVector2& FVector2::operator+(const FVector2& v) {
	FVector2 newV = FVector2(this->x + v.x, this->y + v.y);
	return newV;
}

FVector2& FVector2::operator-(const FVector2& v) {
	FVector2 newV = FVector2(this->x - v.x, this->y - v.y);
	return newV;
}

FVector2& FVector2::operator*(const FVector2& v) {
	FVector2 newV = FVector2(this->x * v.x, this->y * v.y);
	return newV;
}

FVector2& FVector2::operator*(float value) {
	FVector2 newV = FVector2(this->x * value, this->y * value);
	return newV;
}

FVector2& FVector2::operator/(const FVector2& v) {
	FVector2 newV = FVector2(this->x / v.x, this->y / v.y);
	return newV;
}

FVector2& FVector2::operator/(float value) {
	FVector2 newV = FVector2(this->x / value, this->y / value);
	return newV;
}


float FVector2::Magnitude() const {
	return sqrt(x * x + y * y);
}

const FVector2& FVector2::Normalized() const {
	float mag = Magnitude();
	return FVector2(x / mag, y / mag);
}

const FVector2& FVector2::Normalize(const FVector2& v) {
	float mag = v.Magnitude();
	return FVector2(v.x / mag, v.y / mag);
}


const FVector2& FVector2::Cross(const FVector2& v1, const FVector2& v2) {
	return FVector2();
}

float FVector2::Dot(const FVector2& v1, const FVector2& v2) {
	return 0.0f;
}


const FVector2& FVector2::Lerp(const FVector2& v1, const FVector2& v2, float t) {
	return FVector2(FMath::Lerp(v1.x, v2.x, t), FMath::Lerp(v1.y, v2.y, t));
}