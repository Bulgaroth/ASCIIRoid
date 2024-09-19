#include "Asteroid.hpp"
#include <vector>

//enum Type { Base, PowerUp, Explosive, Spreader };

Asteroid::Asteroid(int size, Math::Vector2f pos, Math::Vector2f velocity)
	: m_size(size), m_velocity(velocity), Entity(pos)
{
	switch (size)
	{
		case 1: m_color = 0x0A;
			break;
		case 2: m_color = 0x0B;
			break;
		default: m_color = 0x0C;
	}
}

void Asteroid::Update()
{
	if (m_velocity == Math::Vector2f::zero) return;
	m_pos += m_velocity;
}

wchar_t Asteroid::GetChar() const
{
	wchar_t res = L' ';
	switch (m_size)
	{
		case 0: res = L'.';
			break;
		case 1: res = L'°';
			break;
		case 2: res = L'o';
			break;
		default: res = L'O';
	}
	return res;
}
