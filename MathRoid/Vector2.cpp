#include <cmath>
#include "Vector2.hpp"

namespace Math
{
	template<typename T>
	Vector2<T>::Vector2(T x, T y) : x(x), y(y)
	{
	}

	template<typename T>
	float Vector2<T>::Distance(Vector2 a, Vector2 b)
	{
		return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
	}
}