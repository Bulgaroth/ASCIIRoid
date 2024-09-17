#pragma once

#include "MathRoid/Vector2.hpp"

class Controller
{
public:
	Controller(Math::Vector2i size);
	~Controller() = default;

	void Update();

private:
	Math::Vector2i m_size;
};