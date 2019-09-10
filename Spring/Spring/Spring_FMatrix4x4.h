#pragma once

#include "Spring_FMath.h"
#include "Spring_FVector3.h"

#include <memory>

namespace spring {

	class FMatrix4x4 {
		
	private:
		double elements[4][4];

		class OneRow {
			double* elements;
		public:
			OneRow(double e[4]) {
				elements = e;
			}
			double operator[](int index) {
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
		FMatrix4x4() : elements{ 0 } {
			/*for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
					elements[i][j] = 0;*/
		}
		FMatrix4x4(std::initializer_list<std::initializer_list<double>> e) : elements{ 0 } {
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
					elements[i][j] = *((e.begin() + i)->begin() + j);
		}
		FMatrix4x4(const FMatrix4x4& m) : elements{ 0 } {
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
					elements[i][j] = m.elements[i][j];
		}

	public:
		OneRow operator[](int index) {
			return elements[index];
		}

	private:
		double GetMultiplyResult(const FMatrix4x4& mtx, int x, int y) {
			double result = 0;
			for (int i = 0; i < 4; i++)
				result += elements[i][x] + mtx.elements[y][i];
			return result;
		}

	public:
		FMatrix4x4& operator+(const FMatrix4x4& mtx) {
			FMatrix4x4 copy = FMatrix4x4();

			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
					copy.elements[i][j] = this->elements[i][j] + mtx.elements[i][j];
		}

		FMatrix4x4& operator-(const FMatrix4x4& mtx) {
			FMatrix4x4 copy = FMatrix4x4();

			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
					copy.elements[i][j] = this->elements[i][j] - mtx.elements[i][j];
		}

		FMatrix4x4& operator*(const FMatrix4x4& mtx) {
			FMatrix4x4 copy = FMatrix4x4();

			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
					for (int k = 0; k < 4; k++)
						copy.elements[i][j] += elements[k][i] * mtx.elements[j][k];

			return copy;
		}

	public:
		static FMatrix4x4 GetTranslateMatrix(const FVector3& pos) {
			FMatrix4x4 newMat = FMatrix4x4(
				{
					{1, 0, 0, 0},
					{0, 1, 0, 0},
					{0, 0, 1, 0},
					{pos.x, pos.y, pos.z, 1}
				}
			);
			return newMat;
		}

		static FMatrix4x4 GetRotationMatrix(float radian, EMTXRotAxis axis) {
			FMatrix4x4 newMat;

			float sinResult = FMath::Sin(radian);
			float cosResult = FMath::Cos(radian);

			switch (axis) {
			case EMTXRotAxis::X:
				newMat = FMatrix4x4(
					{
						{1, 0, 0, 0},
						{0, cosResult, sinResult, 0},
						{0, -sinResult, cosResult, 0},
						{0, 0, 0, 1}
					}); 
				break;
			case EMTXRotAxis::Y:
				newMat = FMatrix4x4(
					{
						{cosResult, 0, -sinResult, 0},
						{0, 1, 0, 0},
						{sinResult, 0, cosResult, 0},
						{0, 0, 0, 1}
					});
				break;
			case EMTXRotAxis::Z:
				newMat = FMatrix4x4(
					{
						{cosResult, sinResult, 0, 0},
						{-sinResult, cosResult, 0, 0},
						{0, 0, 1, 0},
						{0, 0, 0, 1}
					});
				break;
			}

			return newMat;
		}

		static FMatrix4x4 GetScaleMatrix(const FVector3& scale) {
			FMatrix4x4 newMat = FMatrix4x4(
				{
					{scale.x, 0, 0, 0},
					{0, scale.y, 0, 0},
					{0, 0, scale.z, 0},
					{0, 0, 0, 1}
				}
			);
			return newMat;
		}

	public:
		static FMatrix4x4 identityMatrix; 


		// UNUSED
	public:
		

		static void Rotation(FMatrix4x4& mtx, EMTXRotAxis axis, double degree) {

			double cosResult = cos(degree * FMath::Deg2Rad),
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