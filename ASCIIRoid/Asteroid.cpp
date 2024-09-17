#include "Asteroid.hpp"
#include <vector>

//enum Type { Base, PowerUp, Explosive, Spreader };

Asteroid::Asteroid(int size, int color, Math::Vector2i pos, Math::Vector2i velocity)
	: m_size(size), m_color(color), m_velocity(velocity), Entity(pos)
{}

void Asteroid::Update()
{
	if (m_velocity == Math::Vector2i::zero) return;
	m_pos += m_velocity;
}

void Asteroid::Hit()
{
	switch (m_size)
	{
		case 1:
			Separate();
			break;
		case 2:
		{
			
		}
		case 3:
		{

		}
	}
}

void Asteroid::Separate()
{

}


void Asteroid::Delete()
{

}
