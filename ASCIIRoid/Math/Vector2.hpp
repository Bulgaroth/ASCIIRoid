#pragma once

#include <cmath>

namespace Math
{
	template<typename T>
	class Vector2
	{
	public:
		Vector2() : x(0), y(0) {}
		Vector2(T x, T y) : x(x), y(y) {}
		~Vector2() = default;

		T x, y;

		static inline float Distance(Vector2<T> a, Vector2<T> b)
		{
			return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
		}

		static Vector2 zero;

		bool operator == (const Vector2<T>& other) const
		{
			return x == other.x && y == other.y;
		}

		Vector2<T> operator + (const Vector2<T>& other) const
		{
			return Vector2<T>(x + other.x, y + other.y);
		}

		Vector2<T> operator - (const Vector2<T>& other) const
		{
			return Vector2<T>(x - other.x, y - other.y);
		}

		Vector2<T> operator - ()
		{
			return Vector2<T>{ -x, -y};
		}

		Vector2<T>& operator += (const Vector2<T>& other)
		{
			*this = *this + other;
			return *this;
		}

	};

	template<typename T>
	Vector2<T> Vector2<T>::zero = Vector2(0, 0);

	using Vector2i = Vector2<int>;
	using Vector2f = Vector2<float>;
}