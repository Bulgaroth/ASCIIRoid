#include "Player.hpp"

#include <windows.h>

void Player::HandleInput(float deltaTime)
{
	if (GetAsyncKeyState((unsigned short)'Q') & 0x8000 || GetAsyncKeyState(VK_LEFT) & 0x8000)
		m_camera.RotateY(2 * deltaTime);
	
	if (GetAsyncKeyState((unsigned short)'D') & 0x8000 || GetAsyncKeyState(VK_RIGHT) & 0x8000)
		m_camera.RotateY(-2 * deltaTime);

	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		if (!m_spaceDown)
			m_shooting = true;
		m_spaceDown = true;
	} else
		m_spaceDown = false;
}
