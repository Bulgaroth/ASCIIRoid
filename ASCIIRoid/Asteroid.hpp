#pragma once
#include <memory>

#include "Math/Vector2.hpp"
#include "Entity.hpp"

class Asteroid : public Entity
{
public:
	Asteroid(int size, int color, Math::Vector2i pos, Math::Vector2i velocity = Math::Vector2i::zero);
	~Asteroid() = default;

	virtual void Update() override;
	virtual void Hit();
	void Separate();

	const int& GetColor() const { return m_color; }

	void Delete();

private:
	int m_size, m_color;
	Math::Vector2i m_velocity;
};