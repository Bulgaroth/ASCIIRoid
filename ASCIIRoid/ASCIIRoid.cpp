#include <iostream>

#include "ConsoleRenderer/ConsoleRenderer.hpp"

int main(int argc, char* argv[])
{
	std::cout << "Hello World!\n";

	ConsoleRenderer::ConsoleWindow window(1920, 1080);
	
	std::cout << window << "\n";
	return 0;
}
