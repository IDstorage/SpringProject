#pragma once

#include "Spring_FMath.h"
#include <memory>

namespace spring {

	class FMatrix4x4 {
	public:
		static FMatrix4x4 identityMatrix;

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
		enum class EMTXRotAxis {
			X,
			Y,
			Z
		};

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

	FMatrix4x4 FMatrix4x4::identityMatrix = FMatrix4x4({ {1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1} });
}