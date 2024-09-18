#include <iostream>
#include <sstream>

#include "ConsoleRenderer/ConsoleRenderer.hpp"
#include "Math/Vector2.hpp"

int main(int argc, char* argv[])
{
	std::cout << "Resize the console window..\n";
	std::cin.get();
	Math::Vector2i size(156, 46);
	ConsoleRenderer::ConsoleWindow window(size.x, size.y);


	// std::cout << "Please set the console to fullscreen...\n";
	// std::cin.get();
	//
	// window.ClearScreen();
	//
	// std::cout << "Window size is " << window << '\n';
	// std::cin.get();

	while (true)
	{
		window.Update();
	
		std::wstringstream ss;
		ss << "Player angle is " << window.m_playerAngle * 180 / 3.14159265358979323846 << " | ";
		ss << "Player position is " << window.m_playerPos.x << ", 0, " << window.m_playerPos.y;
		window.Draw(0, 0, ss.str());

		window.PushBuffer();
	}

	
	return 0;
}
