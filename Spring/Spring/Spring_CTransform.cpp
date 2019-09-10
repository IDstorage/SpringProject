#include "Spring_CTransform.h"

using namespace DirectX;
using namespace spring;


void CTransform::Init() {

	position = FVector3::zero;
	eulerAngle = FVector3::zero;
	localScale = FVector3::one;

	transformMatrix = FMatrix4x4::identityMatrix;

}