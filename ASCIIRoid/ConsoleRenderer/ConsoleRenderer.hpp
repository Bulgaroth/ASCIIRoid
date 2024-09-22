#pragma once

#include <chrono>
#include <ostream>
#include <windows.h>

#include "ASCIIRoid/Player.hpp"
#include "ASCIIRoid/Math/Vector2.hpp"

namespace Math
{
	class Sphere;
}

namespace ConsoleRenderer
{
	class ConsoleWindow
	{
	public:
		ConsoleWindow(int width, int height);
		~ConsoleWindow();

		void Draw(SMALL_RECT region, const CHAR_INFO* buffer);
		void PushBuffer() const;

		void ClearScreen(char fill = ' ') const;

		friend std::ostream& operator<< (std::ostream& stream, const ConsoleWindow& consoleWindow);
		friend std::wostream& operator<< (std::wostream& stream, const ConsoleWindow& consoleWindow);

		Math::Vector2i GetSize() const;

		void Update(const std::vector<Math::Sphere>& spheres, Player& player);

	private:
		std::pair<wchar_t, int> PerPixel(Math::Vector2i coord, const std::vector<Math::Sphere>& spheres);
		void Reallocate();
		void Render(const std::vector<Math::Sphere>& spheres);
		void ThreadRender(int rx, int ry, int rwidth, int rheight, const std::vector<Math::Sphere>& spheres);

	private:
		HANDLE m_handle;
		std::vector<CHAR_INFO> m_screenBuffer;
		int m_screenWidth, m_screenHeight;
		float m_FOV;

		float m_depth = 16.0;

		std::chrono::system_clock::time_point m_lastTime;

		Player* m_player;

		DWORD m_dwBytesWritten;
	};
}
