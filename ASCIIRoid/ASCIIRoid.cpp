#include <iostream>
#include <chrono>
#include <thread>

#include "ConsoleRenderer/ConsoleRenderer.hpp"
#include "Math/Vector2.hpp"
#include "Controller.hpp"

int main(int argc, char* argv[])
{
	Math::Vector2i gridSize(156, 46);
	Controller ctrl(gridSize);

	bool playing = true;
	while (!ctrl.end)
	{
		ctrl.Update();
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}

	std::cout << "GAME OVER ! Your Score : " << ctrl.score << std::endl;
	std::cin.get();

	return 0;
}