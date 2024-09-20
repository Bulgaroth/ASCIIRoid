#pragma once
#include <cmath>
#include <utility>

#include "Vector2.hpp"
#include "Vector3.hpp"

#define TAN_360 6.28318530718

namespace Math {

	class Utility
	{
	public:
		inline static bool SolveQuadratic(const float &a, const float &b, const float &c, float &x0, float &x1);
		static std::array<Math::Vector2f, 8> Neighbours;
	};

	

	
	class Ray
	{
	public:
		Vector3f origin;
		Vector3f direction;
	};
}
