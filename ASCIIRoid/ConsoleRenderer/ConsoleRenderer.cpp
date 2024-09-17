#include "ConsoleRenderer.hpp"

#include <string>


namespace ConsoleRenderer
{
	ConsoleWindow::ConsoleWindow(int width, int height)
		: m_Width(width), m_Height(height)
	{

	}

	ConsoleWindow::~ConsoleWindow() = default;

	std::ostream& operator<<(std::ostream& stream, const ConsoleWindow& consoleWindow)
	{
		return stream << "Width: " + std::to_string(consoleWindow.m_Width) + ", Height: " + std::to_string(consoleWindow.m_Height);
	}
}
