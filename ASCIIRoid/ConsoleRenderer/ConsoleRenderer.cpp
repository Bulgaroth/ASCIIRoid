#include "ConsoleRenderer.hpp"

#include <string>

#define _WIN32_WINNT 0x0501
#include <iostream>
#include <windows.h>
#include <chrono>

#include "ASCIIRoid/Math/Utility.hpp"



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

		m_lastTime = std::chrono::system_clock::now();
	}

	ConsoleWindow::~ConsoleWindow() = default;

	void ConsoleWindow::Draw(int x, int y, const std::wstring& str) const
	{
		for (int i = 0; i < str.size(); i++)
		{
			if (x >= 0 && x < m_screenWidth && y >= 0 && y < m_screenHeight)
			{
				m_screenBuffer[y * m_screenWidth + x + i] = str[i];
			}
		}
		
	}

	void ConsoleWindow::Render()
	{
		m_screenBuffer[m_screenWidth * m_screenHeight - 1] = '\0';
		WriteConsoleOutputCharacter(m_handle, m_screenBuffer, m_screenWidth * m_screenHeight, { 0,0 }, &m_dwBytesWritten);
	}

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
		auto now = std::chrono::system_clock::now();
		std::chrono::duration<float> duration = now - m_lastTime;
		m_lastTime = now;
		float deltaTime = duration.count();
		auto size = GetSize();

		if (size.x != m_screenWidth || size.y != m_screenHeight)
		{
			delete[] m_screenBuffer;
			m_screenBuffer = new wchar_t[size.x * size.y];
			m_screenWidth = size.x;
			m_screenHeight = size.y;
			ClearScreen();
		}


		if (GetAsyncKeyState((unsigned short)'Q') & 0x8000)
			m_playerAngle -= 0.5f * deltaTime;

		if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
			m_playerAngle += 0.5f * deltaTime;

		if (GetAsyncKeyState((unsigned short)'Z') & 0x8000)
		{
			m_playerPos.x += sinf(m_playerAngle) * 1.0f * deltaTime;
			m_playerPos.y += cosf(m_playerAngle) * 1.0f * deltaTime;
		}

		if (GetAsyncKeyState((unsigned short)'S') & 0x8000)
		{
			m_playerPos.x -= sinf(m_playerAngle) * 1.0f * deltaTime;
			m_playerPos.y -= cosf(m_playerAngle) * 1.0f * deltaTime;
		}

		const auto& spheres = m_worldMap.GetSpheres();
		
		for (int x = 0; x < m_screenWidth; ++x)
		{
			float fRayAngle = (m_playerAngle - m_FOV / 2.0f) + (static_cast<float>(x) / static_cast<float>(m_screenWidth)) * m_FOV;

			float distanceToObject = 0.0f;
			Math::Vector2f collision = Math::Vector2f(0.0f, 0.0f);
			bool hitWall = false;

			float eyeX = sinf(fRayAngle);
			float eyeY = cosf(fRayAngle);

			Math::Ray ray{m_playerPos, {eyeX, eyeY}};
			
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

			}
			int ceiling = (float)(m_screenHeight/2.0) - m_screenHeight / ((float)distanceToObject);
			int floor = m_screenHeight - ceiling;

			std::wstring chars = L"@&%QWNM0gB$#DR8mHXKAUbGOpV4d9h6PkqwSE2]ayjxY5Zoen[ult13If}C{iF|(7J)vTLs?z/*cr!+<>;=^,_:'-.` ";

			int idx = (distanceToObject / m_depth) * chars.size();

			wchar_t shade = chars[idx];
			for (int y = 0; y < m_screenHeight; y++)
			{
				if (y < ceiling)
					m_screenBuffer[y * m_screenWidth + x] = ' ';
				else if (y > ceiling && y < floor)
				{
					m_screenBuffer[y * m_screenWidth + x] = shade;
				}
				else
				{
					float b = 1.0f - (((float)y - m_screenHeight / 2.0f) / ((float)m_screenHeight / 2.0f));
					int idx2 = b * chars.size();
					short otherShade;
					if (b > 1.0f) otherShade = ' ';
					 else otherShade = chars[idx2];
					// if (b < 0.25f)		otherShade = '#';
					// else if (b < 0.5f)	otherShade = 'x';
					// else if (b < 0.75f)	otherShade = '-';
					// else if (b < 0.9f)	otherShade = '.';
					// else				otherShade = ' ';
					m_screenBuffer[y * m_screenWidth + x] = otherShade;
				}
			}
		}
		
	}

	std::ostream& operator<<(std::ostream& stream, const ConsoleWindow& consoleWindow)
	{
		auto size = consoleWindow.GetSize();
		return stream << "Width: " + std::to_string(size.x) + ", Height: " + std::to_string(size.y);
	}
	std::wostream& operator<<(std::wostream& stream, const ConsoleWindow& consoleWindow)
	{
		auto size = consoleWindow.GetSize();
		return stream << L"Width: " + std::to_wstring(size.x) + L", Height: " + std::to_wstring(size.y);
	}

}
