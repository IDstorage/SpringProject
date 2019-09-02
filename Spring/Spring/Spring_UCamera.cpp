#include "Spring_UCamera.h"

using namespace DirectX;
using namespace spring;


UCamera::UCamera(const std::string& name) 
	: cameraName(name), position(XMFLOAT3(0.0f, 0.0f, -10.0f)),
		eulerAngle(XMFLOAT3(0.0f, 0.0f, 0.0f)), viewMatrix(XMMATRIX()) {}

UCamera::~UCamera() {}


void UCamera::SetPosition(float x, float y, float z) {
	position.x = x;
	position.y = y;
	position.z = z;
}

void UCamera::SetPosition(XMFLOAT3 pos) {
	position = pos;
}


void UCamera::SetEulerAngle(float x, float y, float z) {
	eulerAngle.x = x;
	eulerAngle.y = y;
	eulerAngle.z = z;
}

void UCamera::SetEulerAngle(XMFLOAT3 euler) {
	eulerAngle = euler;
}


XMFLOAT3 UCamera::GetPositionDX() const {
	return position;
}


XMFLOAT3 UCamera::GetEulerAngleDX() const {
	return eulerAngle;
}


void UCamera::GetViewMatrix(XMMATRIX& mat) {
	mat = viewMatrix;
}


void UCamera::Render() {
	XMFLOAT3 up = XMFLOAT3(0.0f, 1.0f, 0.0f),
		pos = position,
		lookAt = XMFLOAT3(0.0f, 0.0f, 1.0f);
	XMVECTOR upVector, posVector, lookAtVector;

	float yaw, pitch, roll;
	XMMATRIX rotationMat;

	upVector = XMLoadFloat3(&up);
	posVector = XMLoadFloat3(&pos);
	lookAtVector = XMLoadFloat3(&lookAt);

	pitch = eulerAngle.x * FMath::Deg2Rad;
	yaw = eulerAngle.y * FMath::Deg2Rad;
	roll = eulerAngle.z * FMath::Deg2Rad;

	// 회전 행렬 생성
	rotationMat = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// lookAt 벡터와 up 벡터를 회전 행렬로 변형하여 뷰가 올바르게 원점에서 회전하도록 함
	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMat);
	upVector = XMVector3TransformCoord(upVector, rotationMat);

	// 회전된 카메라 위치를 뷰어 위치로 변환
	lookAtVector = XMVectorAdd(posVector, lookAtVector);

	// 세 벡터를 통해 뷰 행렬을 만듦
	viewMatrix = XMMatrixLookAtLH(posVector, lookAtVector, upVector);
}
