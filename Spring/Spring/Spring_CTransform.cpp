#include "Spring_CTransform.h"

using namespace DirectX;
using namespace spring;


void CTransform::Init() {

	position = FVector3::zero;
	eulerAngle = FVector3::zero;
	localScale = FVector3::one;

	transformMatrix = FMatrix4x4::identityMatrix;

}


void CTransform::SetPosition(const FVector3& pos) {
	position = pos;
}

FVector3 CTransform::GetPosition() const {
	return position;
}

void CTransform::SetEulerAngle(const FVector3& ea) {
	eulerAngle = ea;
}

FVector3 CTransform::GetEulerAngle() const {
	return eulerAngle;
}

//void CTransform::SetRotation(const FQuaternion& quat) {
//	eulerAngle = FQuaternion::ToEulerAngle(quat);
//}
//
FQuaternion CTransform::GetRotation() const {
	return FQuaternion::Euler(eulerAngle);
}

void CTransform::SetLocalScale(const FVector3& s) {
	localScale = s;
}

FVector3 CTransform::GetLocalScale() const {
	return localScale;
}


void CTransform::UpdateMatrix() {
	transformMatrix = FMatrix4x4::identityMatrix;

	transformMatrix = transformMatrix * FMatrix4x4::GetScaleMatrix(localScale);
	
	transformMatrix = transformMatrix * FMatrix4x4::GetRotationMatrix(eulerAngle.x, FMatrix4x4::EMTXRotAxis::X);
	transformMatrix = transformMatrix * FMatrix4x4::GetRotationMatrix(eulerAngle.y, FMatrix4x4::EMTXRotAxis::Y);
	transformMatrix = transformMatrix * FMatrix4x4::GetRotationMatrix(eulerAngle.z, FMatrix4x4::EMTXRotAxis::Z);

	transformMatrix = transformMatrix * FMatrix4x4::GetTranslateMatrix(position); 
}

void CTransform::BeginUpdate() {

	previousPosition = position;
	previousEulerAngle = eulerAngle;
	previousLocalScale = localScale;

}


FMatrix4x4 CTransform::GetTransformMatrix() {
	UpdateMatrix();
	return transformMatrix;
}