#pragma once
#include "Utility.hpp"
#include "Vector2.hpp"
#include <limits>

namespace Math
{
	class Ray;

	class Sphere
	{
	public:
		Vector3f center;
		float radius;

		bool Intersect(const Ray& ray, Vector3f& position, float& distance) const
		{
			Vector3f d = ray.origin - center;

			float p1 = -1 * ray.direction.Dot(d);
			float p2sqr = pow(p1, 2) - d.Dot(d) + pow(radius, 2);

			if (p2sqr < 0)
			{
				distance = 0;
				return false;
			}

			float p2 = sqrt(p2sqr);
			float t = p1 - p2 > 0 ? p1 - p2 : p1 + p2;

			float max = std::numeric_limits<float>::infinity();

			if (t > 0 && t < max)
			{
				distance = t;
				position = ray.origin + ray.direction * t;
			}

			return true;
		}
	};
}
