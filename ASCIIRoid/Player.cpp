#include "Player.hpp"

#include <windows.h>

void Player::HandleInput(float deltaTime)
{
	if (GetAsyncKeyState((unsigned short)'Q') & 0x8000 || GetAsyncKeyState(VK_LEFT) & 0x8000)
		m_angle -= 4 * deltaTime;
	
	if (GetAsyncKeyState((unsigned short)'D') & 0x8000 || GetAsyncKeyState(VK_RIGHT) & 0x8000)
		m_angle += 4 * deltaTime;

	if (m_angle > TAN_360)
		m_angle -= TAN_360;

	if (m_angle < 0.f)
		m_angle += TAN_360;

	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		m_shooting = true;
	}
	
}
