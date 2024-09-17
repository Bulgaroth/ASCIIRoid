#include <iostream>

#include "ConsoleRenderer/ConsoleRenderer.hpp"
#include "Math/Vector2.hpp"

int main(int argc, char* argv[])
{
	Math::Vector2i foo(1920, 1080);
	std::cout << "Please set the console to fullscreen\n";

	ConsoleRenderer::ConsoleWindow window(foo.x, foo.y);

	

	
	std::cin.get();
	return 0;
}
