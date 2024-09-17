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

	ConsoleRenderer::ConsoleWindow window(gridSize.x, gridSize.y);
	std::cout << window << "\n";

	bool playing = true;
	while (playing)
	{
		ctrl.Update();
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}

	return 0;
}