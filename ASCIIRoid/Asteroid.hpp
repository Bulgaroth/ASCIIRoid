#pragma once
#include <memory>

#include "Math/Vector2.hpp"
#include "Entity.hpp"

class Asteroid : public Entity
{
public:
	Asteroid(int size, Math::Vector2f pos, Math::Vector2f velocity = Math::Vector2f::zero);
	~Asteroid() = default;

	virtual void Update() override;

	const int& GetColor() const { return m_color; }
	const int& GetSize() const { return m_size; }
	wchar_t GetChar(const Math::Vector2i &charPos) const;

private:
	int m_size, m_color;
	Math::Vector2f m_velocity;
};