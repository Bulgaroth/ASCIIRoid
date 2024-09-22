#pragma once
#include <array>

#include "ConsoleRenderer/Camera.hpp"
#include "Math/Utility.hpp"
#include "Math/Vector2.hpp"

class Player
{
public:
	explicit Player(Math::Vector2i pos) : m_camera({1, 1}, {static_cast<float>(pos.x), 0, static_cast<float>(pos.y)}) {}

	const ConsoleRenderer::Camera& GetCamera() const { return m_camera; }
	ConsoleRenderer::Camera& GetCamera() { return m_camera; }
	
	template<typename T>
	Math::Vector2<T> GetPos()
	{
		return {static_cast<T>(m_camera.GetPosition().x), static_cast<T>(m_camera.GetPosition().z)};
	}

	template<typename T>
	Math::Vector2<T> GetCanonPos()
	{
		return GetPos<T>() + GetProjectileVelocity<T>();
	}

	template<typename T>
	Math::Vector2<T> GetProjectileVelocity()
	{
		int shootPosIndex = std::round((-m_camera.GetYaw() * 7.0f) / (TAN_360));
		shootPosIndex = shootPosIndex % Math::Utility::Neighbours.size();
		
		return  Math::Utility::Neighbours[shootPosIndex];
	}

	bool IsShooting() { auto last = m_shooting; m_shooting = false; return last; }
	
	void HandleInput(float deltaTime);

private:
	bool m_spaceDown;
	bool m_shooting;

	ConsoleRenderer::Camera m_camera;
	
};
