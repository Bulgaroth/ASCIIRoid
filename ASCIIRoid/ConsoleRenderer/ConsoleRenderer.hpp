#pragma once

#include <chrono>
#include <ostream>
#include <windows.h>

#include "ASCIIRoid/WorldMap.hpp"
#include "ASCIIRoid/Math/Vector2.hpp"

namespace ConsoleRenderer
{
	class ConsoleWindow
	{
	public:
		ConsoleWindow(int width, int height);
		~ConsoleWindow();

		void Draw(int x, int y, const std::wstring& str);

		void Render();
		void PushBuffer() const;

		void ClearScreen(char fill = ' ') const;

		friend std::ostream& operator<< (std::ostream& stream, const ConsoleWindow& consoleWindow);
		friend std::wostream& operator<< (std::wostream& stream, const ConsoleWindow& consoleWindow);

		Math::Vector2i GetSize() const;

		void Update();

		float m_playerAngle = 0;
		Math::Vector2f m_playerPos = Math::Vector2f(8, 8);

	private:
		wchar_t PerPixel(Math::Vector2f coord) const;
		void Reallocate();
		void ThreadRender(int rx, int ry, int rwidth, int rheight);

	private:
		HANDLE m_handle;
		std::vector<CHAR_INFO> m_screenBuffer;
		int m_screenWidth, m_screenHeight;
		float m_FOV;
		Math::Vector2i m_mapSize = Math::Vector2i(16, 16);

		float m_depth = 16.0;

		WorldMap m_worldMap;

		std::chrono::system_clock::time_point m_lastTime;

		DWORD m_dwBytesWritten;
	};
}
