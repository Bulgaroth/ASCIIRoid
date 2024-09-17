#include "ConsoleRenderer.hpp"

#include <string>

#define _WIN32_WINNT 0x0501
#include <iostream>
#include <windows.h>


namespace ConsoleRenderer
{
	ConsoleWindow::ConsoleWindow(int width, int height)
		: m_screenWidth(width), m_screenHeight(height), m_worldMap(m_mapSize)
	{
		// set as fullscreen
		INPUT inp[2] = {};
		inp[0].type = INPUT_KEYBOARD;
		inp[0].ki.wVk = VK_F11;
		inp[1].type = INPUT_KEYBOARD;
		inp[1].ki.wVk = VK_F11;
		inp[1].ki.dwFlags = KEYEVENTF_KEYUP;
		// SendInput(2, inp, sizeof(INPUT));
		
		// m_handle = GetStdHandle(STD_OUTPUT_HANDLE);
		m_screenBuffer = new wchar_t[m_screenWidth * m_screenHeight];
		m_handle = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		SetConsoleActiveScreenBuffer(m_handle);

		m_FOV = 3.14159265f / 4.0f; // in radiant
	}

	ConsoleWindow::~ConsoleWindow() = default;

	void ConsoleWindow::ClearScreen(char fill) const
	{
		COORD tl = {0,0};
		CONSOLE_SCREEN_BUFFER_INFO s;
		GetConsoleScreenBufferInfo(m_handle, &s);
		DWORD written, cells = s.dwSize.X * s.dwSize.Y;
		FillConsoleOutputCharacter(m_handle, fill, cells, tl, &written);
		FillConsoleOutputAttribute(m_handle, s.wAttributes, cells, tl, &written);
		SetConsoleCursorPosition(m_handle, tl);
	}

	Math::Vector2i ConsoleWindow::GetSize() const
	{
		CONSOLE_SCREEN_BUFFER_INFO csbi;

		GetConsoleScreenBufferInfo(m_handle, &csbi);
		int columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
		int rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

		return { columns, rows };
	}

	void ConsoleWindow::Update()
	{
		for (int x = 0; x < m_screenWidth; ++x)
		{
			float fRayAngle = (m_playerAngle - m_FOV / 2.0f) + (static_cast<float>(x) / static_cast<float>(m_screenWidth)) * m_FOV;

			float distanceToObject = 0.0f;
			bool hitWall = false;

			float eyeX = sinf(fRayAngle);
			float eyeY = cosf(fRayAngle);
			while (!hitWall && distanceToObject < m_depth)
			{
				distanceToObject += 0.1f;

				int testX = static_cast<int>(m_playerPos.x + eyeX * distanceToObject);
				int testY = static_cast<int>(m_playerPos.y + eyeY * distanceToObject);
				if (testX < 0 || testX >= m_mapSize.x || testY < 0 || testY >= m_mapSize.y)
				{
					hitWall = true;
					distanceToObject = m_depth;
				}
				else
				{
					if (m_worldMap.GetElement({testX, testY}) == '#')
					{
						hitWall = true;
					}
				}

				int ceiling = (float)(m_screenHeight/2.0) - m_screenHeight / ((float)distanceToObject);
				int floor = m_screenHeight - ceiling;

				for (int y = 0; y < m_screenHeight; y++)
				{
					if (y < ceiling)
						m_screenBuffer[y * m_screenWidth + x] = ' ';
					else if (y > ceiling && y < floor)
					{
						__noop;
						m_screenBuffer[y * m_screenWidth + x] = '#';
					}
					else
						m_screenBuffer[y * m_screenWidth + x] = ' ';
				}
			}
		}
		
		m_screenBuffer[m_screenWidth * m_screenHeight - 1] = '\0';
		WriteConsoleOutputCharacter(m_handle, m_screenBuffer, m_screenWidth * m_screenHeight, { 0,0 }, &m_dwBytesWritten);
	}

	std::ostream& operator<<(std::ostream& stream, const ConsoleWindow& consoleWindow)
	{
		auto size = consoleWindow.GetSize();
		return stream << "Width: " + std::to_string(size.x) + ", Height: " + std::to_string(size.y);
	}

}
