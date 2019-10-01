#pragma once

#include "Spring_FVector3.h"
#include "Spring_FMatrix4x4.h"
#include "Spring_FQuaternion.h"

#include "Spring_UFrameworks.h"


namespace spring {

	class FQuaternion;

	class CTransform : public UComponent {

		FMatrix4x4 transformMatrix;

		FVector3 position;
		FVector3 eulerAngle;
		FVector3 localScale;

		// Update when rendering
		FVector3 previousPosition;
		FVector3 previousEulerAngle;
		FVector3 previousLocalScale;

	public:
		virtual void Init() override;

	private:
		void UpdateMatrix();

	public:
		void BeginUpdate() override;

	public:
		void SetPosition(const FVector3&);
		FVector3 GetPosition() const;

		void SetEulerAngle(const FVector3&);
		FVector3 GetEulerAngle() const;

		void SetRotation(const FQuaternion&);
		FQuaternion GetRotation() const;

		void SetLocalScale(const FVector3&);
		FVector3 GetLocalScale() const;

	public:
		FMatrix4x4 GetTransformMatrix();
	};

}