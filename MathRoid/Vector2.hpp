#pragma once

namespace Math
{
	class Vector2
	{
	public:
		Vector2(int x, int y);
		~Vector2() = default;

		int x, y;

		float Distance(Vector2 a, Vector2 b);
		
	};
}