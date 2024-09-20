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

		static inline float Distance(Vector2 a, Vector2 b)
		{
			return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
		}

		inline float Dot(Vector2 other) const
		{
			float result = x * other.x + y * other.y;
			return result;
		}

		Vector2 operator+(Vector2 other) const
		{
			return Vector2(x + other.x, y + other.y);
		}

		bool operator == (const Vector2<T>& other) const
		{
			return x == other.x && y == other.y;
		}

		Vector2 operator-(Vector2 other) const
		{
			return Vector2(x - other.x, y - other.y);
		}

		template<typename TT>
		Vector2 operator*(TT other) const
		{
			return Vector2(x * other, y * other);
		}

		template<typename TT>
		Vector2 operator-(TT other) const
		{
			return Vector2(x - other, y - other);
		}

		Vector2 Normalized() const
		{
			T len = std::sqrt(x * x + y * y);

			if (len != 0.f)
			{
				T nx = x / len;
				T ny = y / len;
				return Vector2(nx, ny);
			}
			return Vector2(0.f, 0.f);
		}
		

		Vector2<T> operator - ()
		{
			return Vector2<T>{ -x, -y};
		}
	};

	template<typename T>
	Vector2<T> Vector2<T>::zero = Vector2(0, 0);

	using Vector2i = Vector2<int>;
	using Vector2f = Vector2<float>;
}