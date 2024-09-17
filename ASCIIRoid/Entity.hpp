#include "Math/Vector2.hpp"

class Entity
{
public:
	Entity() = default;
	Entity(Math::Vector2i pos) : m_pos(pos){}

	virtual void Update() = 0;
	const Math::Vector2i& GetPos() const { return m_pos; }

protected:
	Math::Vector2i m_pos;
};