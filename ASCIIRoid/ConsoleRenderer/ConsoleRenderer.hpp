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

		bool IsFullScreen() const;

	private:
		int m_width, m_height;
	};
}
