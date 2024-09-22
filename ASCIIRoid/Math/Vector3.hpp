#pragma once

#include <cmath>

namespace Math
{
	template<typename T>
	class Vector3
	{
	public:
		Vector3() : x(0), y(0), z(0) {}
		Vector3(T x, T y, T z) : x(x), y(y), z(z) {}
		~Vector3() = default;

		T x, y, z;

		inline float Distance(Vector3 a, Vector3 b) const
		{
			return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z));
		}

		inline float Dot(Vector3 other) const
		{
			float result = x * other.x + y * other.y + z * other.z;
			return result;
		}

		Vector3 operator+(Vector3 other) const
		{
			return Vector3(x + other.x, y + other.y, z + other.z);
		}

		Vector3& operator+=(Vector3 other)
		{
			x += other.x;
			y += other.y;
			z += other.z;
			return *this;
		}

		Vector3 operator-(Vector3 other) const
		{
			return Vector3(x - other.x, y - other.y, z - other.z);
		}

		template<typename TT>
		Vector3 operator*(TT other) const
		{
			return Vector3(x * other, y * other, z * other);
		}

		template<typename TT>
		Vector3 operator/(TT other) const
		{
			return Vector3(x / other, y / other, z / other);
		}

		Vector3 Normalized() const
		{
			T len = std::sqrt(x * x + y * y + z * z);

			if (len != 0.f)
			{
				T nx = x / len;
				T ny = y / len;
				T nz = z / len;
				return Vector3(nx, ny, nz);
			}
			return Vector3(0.f, 0.f, 0.f);
		}

		Vector3 Cross(Vector3 other) const
		{
			T nx = y * other.z - z * other.y;
			T ny = z * other.x - x * other.z;
			T nz = x * other.y - y * other.x;
			return Vector3(nx, ny, nz);
		}
	};

	using Vector3i = Vector3<int>;
	using Vector3f = Vector3<float>;
}