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

wchar_t Asteroid::GetChar(const Math::Vector2i &charPos) const
{
	if(m_size == 0) return L'.';

	if (charPos.x == -m_size)
	{
		if (charPos.y == -m_size) return  L'╭';
		if (charPos.y == m_size) return  L'╰';
		return  L'|';
	}

	if (charPos.x == m_size)
	{
		if (charPos.y == -m_size) return  L'╮';
		if (charPos.y == m_size) return  L'╯';
		return L'|';
	}

	if (charPos.y == m_size)
	{
		return L'─';
	}
	if (charPos.y == -m_size)
	{
		return L'─';
	}

	switch (m_size)
	{
		case 1: return L'°';
		case 2: return L'o';
		default: return L'O';
	}
}
