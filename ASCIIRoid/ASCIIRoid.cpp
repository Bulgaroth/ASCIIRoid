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
	ConsoleRenderer::ConsoleWindow window(size.x, size.y);
	Controller ctrl(size);
	while (!ctrl.end)
	{
	
		// std::wstringstream ss;
		// ss << "Player angle is " << window.GetPlayer().GetCamera().GetYaw() * 180 / 3.14159265358979323846 << " | ";
		// ss << "Player position is " << window.GetPlayer().GetCamera().GetPosition().x << ", 0, " << window.GetPlayer().GetCamera().GetPosition().y;
		// window.Draw(0, 0, ss.str());
		ctrl.Update();
		window.Update(ctrl.GetSpheres(), ctrl.GetPlayer());
		ctrl.DrawMap(window);
	
		window.PushBuffer();

		// std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}

	window.ClearScreen();
	std::cout << "GAME OVER ! Your Score : " << ctrl.score << std::endl;
	std::cin.get();
}
