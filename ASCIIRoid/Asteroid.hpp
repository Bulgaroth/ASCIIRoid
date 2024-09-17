#pragma once
#include <memory>

#include "Math/Vector2.hpp"

class Asteroid
{
public:
	Asteroid(int size, int color, Math::Vector2i pos);
	~Asteroid() = default;

	void Update();
	virtual void Hit();
	void Destroy();

private:
	int m_size, m_color;
	Math::Vector2i m_pos;
};