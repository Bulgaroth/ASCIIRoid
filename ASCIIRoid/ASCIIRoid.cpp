#include <iostream>

#include "ConsoleRenderer/ConsoleRenderer.hpp"
#include "Math/Vector2.hpp"

int main(int argc, char* argv[])
{
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
	}

	
	return 0;
}
