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

	// ȸ�� ��� ����
	rotationMat = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// lookAt ���Ϳ� up ���͸� ȸ�� ��ķ� �����Ͽ� �䰡 �ùٸ��� �������� ȸ���ϵ��� ��
	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMat);
	upVector = XMVector3TransformCoord(upVector, rotationMat);

	// ȸ���� ī�޶� ��ġ�� ��� ��ġ�� ��ȯ
	lookAtVector = XMVectorAdd(posVector, lookAtVector);

	// �� ���͸� ���� �� ����� ����
	viewMatrix = XMMatrixLookAtLH(posVector, lookAtVector, upVector);
}
