#pragma once

#include <cmath>

namespace Math
{
	template<typename T>
	class Vector2
	{
	public:
		Vector2(T x, T y) : x(x), y(y) {}
		~Vector2() = default;

		T x, y;

		inline float Distance(Vector2 a, Vector2 b) const
		{
			return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
		}
	};

	using Vector2i = Vector2<int>;
	using Vector2f = Vector2<float>;
}