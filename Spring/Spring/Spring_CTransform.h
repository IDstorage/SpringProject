#pragma once

#include "Spring_FVector3.h"
#include "Spring_FMatrix4x4.h"

#include "Spring_UFrameworks.h"


namespace spring {

	class CTransform : public UComponent {

		FMatrix4x4 transformMatrix;

		FVector3 position;
		FVector3 eulerAngle;
		FVector3 localScale;

	public:
		virtual void Init() override;

	};

}