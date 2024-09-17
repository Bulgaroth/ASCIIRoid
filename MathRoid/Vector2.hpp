#pragma once

namespace Math
{
	template<typename T>
	class Vector2
	{
	public:
		Vector2(T x, T y);
		~Vector2() = default;

		T x, y;

		float Distance(Vector2 a, Vector2 b);
		
	};

	using Vector2i = Vector2<int>;
}