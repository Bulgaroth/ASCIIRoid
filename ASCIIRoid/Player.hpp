#pragma once
#include <array>

#include "Math/Utility.hpp"
#include "Math/Vector2.hpp"

class Player
{
public:
	explicit Player(Math::Vector2i pos) : m_pos(pos), m_angle(0.f) {}

	template<typename T>
	Math::Vector2<T> GetPos()
	{
		return {static_cast<T>(m_pos.x), static_cast<T>(m_pos.y)};
	}

	template<typename T>
	Math::Vector2<T> GetCanonPos()
	{
		return GetPos<T>() + GetProjectileVelocity<T>();
	}

	template<typename T>
	Math::Vector2<T> GetProjectileVelocity()
	{
		int shootPosIndex = std::round((m_angle * 7.0f) / (TAN_360));
		
		return  Math::Utility::Neighbours[shootPosIndex];
	}

	bool IsShooting() { auto last = m_shooting; m_shooting = false; return last; }
	
	void HandleInput(float deltaTime);
	void Update();

private:
	Math::Vector2i m_pos;
	float m_angle;
	bool m_shooting;
};
