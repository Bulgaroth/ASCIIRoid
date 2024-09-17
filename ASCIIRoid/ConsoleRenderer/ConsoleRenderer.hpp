#pragma once

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

		void ClearScreen(char fill = ' ') const;

		friend std::ostream& operator<< (std::ostream& stream, const ConsoleWindow& consoleWindow);

		Math::Vector2i GetSize() const;

		void Update();

	private:
		HANDLE m_handle;
		wchar_t* m_screenBuffer;
		int m_screenWidth, m_screenHeight;
		float m_FOV;

		float m_playerAngle = 0;
		Math::Vector2i m_playerPos = Math::Vector2i(8, 8);
		Math::Vector2i m_mapSize = Math::Vector2i(16, 16);

		float m_depth = 16.0;

		WorldMap m_worldMap;

		DWORD m_dwBytesWritten;
	};
}
