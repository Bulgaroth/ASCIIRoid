#include <iostream>
#include <sstream>

#include "ConsoleRenderer/ConsoleRenderer.hpp"
#include "Controller.hpp"
#include <chrono>
#include <thread>
#include "Math/Vector2.hpp"

int main(int argc, char* argv[])
{
	std::cout << "Resize the console window..\n";
	std::cin.get();
	Math::Vector2i size(156, 46);
	Controller ctrl(size);
	ConsoleRenderer::ConsoleWindow window(size.x, size.y);
	while (!ctrl.end)
	{
		window.Update();
	
		std::wstringstream ss;
		ss << "Player angle is " << window.m_playerAngle * 180 / 3.14159265358979323846 << " | ";
		ss << "Player position is " << window.m_playerPos.x << ", 0, " << window.m_playerPos.y;
		window.Draw(0, 0, ss.str());
	
		window.PushBuffer();
		// ctrl.Update();

		// std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}

	// std::cout << "GAME OVER ! Your Score : " << ctrl.score << std::endl;
	std::cin.get();
}
