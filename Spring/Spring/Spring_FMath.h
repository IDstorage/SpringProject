#pragma once

#include <cmath>

namespace spring {

	class FMath {

	public:
		static constexpr float Deg2Rad = 3.1416f / 180.0f;
		static constexpr float Rad2Deg = 180.0f / 3.1416f;

	public:
		static float Abs(float n) { return abs(n); }
		static int Abs(int n) { return abs(n); }
		static long Abs(long n) { return abs(n); }
		static long long Abs(long long n) { return abs(n); }

		// x % y
		static float Mod(float x, float y) { return fmod(x, y); }

		static float Log2(float x) { return log2(x); }
		static float LogE(float x) { return log(x); }
		static float Log10(float x) { return log10(x); }

		// x^y
		static float Pow(float x, float y) { return pow(x, y); }

		static float Sqrt(float x) { return sqrt(x); }

		static float Ceil(float x) { return ceil(x); }
		static float Floor(float x) { return floor(x); }

	public:
		static float Sin(float radian) { return sin(radian); }
		static float Cos(float radian) { return cos(radian); }
		static float Tan(float radian) { return tan(radian); }

		static float Sin_Deg(float degree) { return sin(degree * Deg2Rad); }
		static float Cos_Deg(float degree) { return cos(degree * Deg2Rad); }
		static float Tan_Deg(float degree) { return tan(degree * Deg2Rad); }

		static float ASin(float scale) { return asin(scale); }
		static float ACos(float scale) { return acos(scale); }
		static float ATan(float scale) { return atan(scale); }
		static float ATan2(float y, float x) { return atan2(y, x); }

	public:
		static float Lerp(float x, float y, float t) { return (1.0f - t) * x + t * y; }

	public:
		static int Clamp(int value, int min, int max) {
			if (value < min)
				return min;
			else if (value > max)
				return max;
			return value;
		}
		static float Clamp(float value, float min, float max) {
			if (value < min)
				return min;
			else if (value > max)
				return max;
			return value;
		} 

	};

}