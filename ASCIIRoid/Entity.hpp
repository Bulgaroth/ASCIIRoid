#include "Math/Vector2.hpp"

class Entity
{
public:
	Entity() = default;
	Entity(Math::Vector2f pos) : m_pos(pos){}

	virtual void Update(float dt) = 0;
	const Math::Vector2i& GetPos() const 
	{ 
		return { static_cast<int>(m_pos.x), static_cast<int>(m_pos.y) };
	}

	const Math::Vector2f& GetPosF() const { return m_pos; }

protected:
	Math::Vector2f m_pos;
};