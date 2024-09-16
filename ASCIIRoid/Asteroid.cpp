#include "Asteroid.hpp"
#include <vector>

//enum Type { Base, PowerUp, Explosive, Spreader };

Asteroid::Asteroid(int size, int color, Math::Vector2 pos) 
	: m_size(size), m_color(color), m_pos(pos)
{

}

void Asteroid::Update()
{
	
}

void Asteroid::Hit()
{
	switch (m_size)
	{
		case 1:
			Destroy;
			break;
		case 2:
		{
			
		}
		case 3:
		{

		}
	}
}

void Asteroid::Destroy()
{

}
