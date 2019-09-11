#pragma once

#include "Spring_FMath.h"
#include "Spring_FVector3.h"

#include <memory>

namespace spring {

	class FMatrix4x4 {
		
	private:
		float elements[4][4];

		class OneRow {
			float* elements;
		public:
			OneRow(float e[4]) {
				elements = e;
			}
			float operator[](int index) {
				return elements[index];
			}
		};

	public:
		enum class EMTXRotAxis {
			X,
			Y,
			Z
		};

	public:
		FMatrix4x4();
		FMatrix4x4(std::initializer_list<std::initializer_list<float>>);
		FMatrix4x4(const FMatrix4x4&);

	public:
		OneRow operator[](int);

	private:
		float GetMultiplyResult(const FMatrix4x4&, int, int);

	public:
		FMatrix4x4 operator+(const FMatrix4x4&);
		FMatrix4x4 operator-(const FMatrix4x4&);
		FMatrix4x4 operator*(const FMatrix4x4&);

	public:
		static FMatrix4x4 GetTranslateMatrix(const FVector3&);
		static FMatrix4x4 GetRotationMatrix(float, EMTXRotAxis);
		static FMatrix4x4 GetScaleMatrix(const FVector3&);

	public:
		static FMatrix4x4 identityMatrix; 


		// UNUSED
	public:
		static void Rotation(FMatrix4x4& mtx, EMTXRotAxis axis, float degree) {

			float cosResult = cos(degree * FMath::Deg2Rad),
				sinResult = sin(degree * FMath::Deg2Rad);

			switch (axis) {
			case EMTXRotAxis::X:
				mtx = mtx
					* FMatrix4x4(
						{
							{1, 0, 0, 0},
							{0, cosResult, sinResult, 0},
							{0, -sinResult, cosResult, 0},
							{0, 0, 0, 1}
						});
				break;
			case EMTXRotAxis::Y:
				mtx = mtx
					* FMatrix4x4(
						{
							{cosResult, 0, -sinResult, 0},
							{0, 1, 0, 0},
							{sinResult, 0, cosResult, 0},
							{0, 0, 0, 1}
						});
				break;
			case EMTXRotAxis::Z:
				mtx = mtx
					* FMatrix4x4(
						{
							{cosResult, sinResult, 0, 0},
							{-sinResult, cosResult, 0, 0},
							{0, 0, 1, 0},
							{0, 0, 0, 1}
						});
				break;
			}
		}
	};
}