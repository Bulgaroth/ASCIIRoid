#pragma once

#include <ostream>

namespace ConsoleRenderer
{
	class ConsoleWindow
	{
	public:
		ConsoleWindow(int width, int height);
		~ConsoleWindow();

		friend std::ostream& operator<< (std::ostream& stream, const ConsoleWindow& consoleWindow);

	private:
		int m_Width, m_Height;
	};
}
