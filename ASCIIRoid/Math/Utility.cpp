#include "Utility.hpp"

#include <array>

bool Math::Utility::SolveQuadratic(const float &a, const float &b, const float &c, float &x0, float &x1)
{
	float discr = b * b - 4 * a * c;
	if (discr < 0) return false;

	if (discr == 0) x0 = x1 = - 0.5 * b / a;
	else {
		float q = (b > 0) ?
			-0.5 * (b + std::sqrt(discr)) :
			-0.5 * (b - std::sqrt(discr));
		x0 = q / a;
		x1 = c / q;
	}
		
	if (x0 > x1) std::swap(x0, x1);
	
	return true;
}

std::array<Math::Vector2f, 8> Math::Utility::Neighbours = {
	Math::Vector2f(0.f,-1.f), Math::Vector2f(1.f,-1.f), Math::Vector2f(1.f,0.f),
	Math::Vector2f(1.f,1.f), Math::Vector2f(0.f,1.f), Math::Vector2f(-1.f,1.f),
	Math::Vector2f(-1.f,0.f), Math::Vector2f(-1.f,-1.f)
};