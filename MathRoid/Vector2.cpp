#pragma once

#include <cmath>
#include "Vector2.hpp"

namespace Math
{
	Vector2::Vector2(int x, int y) : x(x), y(y)
	{
	}

	float Distance(Vector2 a, Vector2 b)
	{
		return std::sqrt(((a.x - b.x) * (a.x - b.x)) + ((a.y - b.y) * (a.y - b.y)));
	}
}