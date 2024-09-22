#include "ConsoleRenderer.hpp"

#include <string>

#define _WIN32_WINNT 0x0501
#include <iostream>
#include <windows.h>
#include <chrono>
#include <thread>

#include "ASCIIRoid/Math/Utility.hpp"
#include "ASCIIRoid/Math/Vector3.hpp"
#include "ASCIIRoid/Math/Sphere.hpp"

const std::wstring CHARS = L"@&%QWNM0gB$#DR8mHXKAUbGOpV4d9h6PkqwSE2]ayjxY5Zoen[ult13If}C{iF|(7J)vTLs?z/*cr!+<>;=^,_:'-.`";
const Math::Vector3f LIGHT_DIR = Math::Vector3f(1, 1, 1);


namespace ConsoleRenderer
{
	ConsoleWindow::ConsoleWindow(int width, int height)
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
		Reallocate();
		
		m_handle = GetStdHandle(STD_OUTPUT_HANDLE);

		m_FOV = 3.14159265f / 4.0f; // in radiant

		m_lastTime = std::chrono::system_clock::now();
	}

	ConsoleWindow::~ConsoleWindow() = default;

	void ConsoleWindow::Draw(SMALL_RECT region, const CHAR_INFO* buffer)
	{
		for (int y = region.Top; y < region.Top + region.Bottom; y++)
		{
			for (int x = region.Left; x < region.Left + region.Right; x++)
			{
				auto idx = (x - region.Left) + (y - region.Top) * region.Right;
				m_screenBuffer[x + y * m_screenWidth] = buffer[idx];
			}
		}
		
	}

	/*
	 * Writing in a vector is thread-safe only if the threads are writing to different parts of the vector.
	 * The vector must be resized before the threads start writing to it.
	 * Threads are not allowed to do any kind of insertions or deletions.
	 */
	void ConsoleWindow::ThreadRender(int rx, int ry, int rwidth, int rheight, const std::vector<Math::Sphere>& spheres)
	{
		for (int y = ry; y < ry + rheight; y++)
		{
			for (int x = rx; x < rx + rwidth; x++)
			{
				auto perPixel = PerPixel({x, y}, spheres);
				m_screenBuffer[x + y * m_screenWidth].Char.UnicodeChar = perPixel.first;
				m_screenBuffer[x + y * m_screenWidth].Attributes = static_cast<unsigned short>(perPixel.second);
			}
		}
	}

	void ConsoleWindow::Render(const std::vector<Math::Sphere>& spheres)
	{
		std::vector<std::thread> threads;
		int nbThreads = 32;
		int parWidth = m_screenWidth / nbThreads;
		threads.reserve(nbThreads);
		for (int i = 0; i < nbThreads; i++)
		{
			threads.emplace_back(&ConsoleWindow::ThreadRender, this, i * parWidth, 0, parWidth, m_screenHeight, spheres);
		}

		// wait for all threads to finish
		for (auto& thread : threads)
		{
			thread.join();
		}

	}

	std::pair<wchar_t, int> ConsoleWindow::PerPixel(Math::Vector2i coord, const std::vector<Math::Sphere>& spheres)
	{
		wchar_t result = L' ';
		Math::Vector3f closestPoint = {0.0f, 0.0f, 0.0f};
		int color = 0x0F;
		bool hit = false;
		float closestDistance = 1000000.0f;
		Math::Vector3f rayOrigin = m_player->GetCamera().GetPosition();
		Math::Vector3f rayDirection = m_player->GetCamera().GetRayDirection(coord);
		
		// same for all spheres
		float a = rayDirection.Dot(rayDirection);

		for (auto& sphere : spheres)
		{
			float radius = sphere.radius;
			auto spherePos = sphere.center;
			auto origin = rayOrigin - spherePos;
			float b = 2.0f * origin.Dot(rayDirection);
			float c = origin.Dot(origin) - radius * radius;

			float dis = b * b - 4 * a * c;

			float t0, t1;
			if (dis >= 0)
			{
				t0 = (-b - dis) / (2.0f * a);
				t1 = (-b + dis) / (2.0f * a);

				if (t0 > t1)
					t0 = t1;

				if (t0 >= 0 && t0 < closestDistance)
				{
					closestDistance = t0;
					hit = true;
					closestPoint = spherePos;
					color = sphere.color;
				}
			}
		}

		if (hit)
		{
			auto origin = rayOrigin - closestPoint;
			auto hitPoint = origin + rayDirection * closestDistance;
			auto normal = hitPoint.Normalized();

			auto lightIntensity = max(min(normal.Dot(LIGHT_DIR), 1.0f), 0.0f);
			auto idx = static_cast<int>(lightIntensity * (CHARS.size() - 1));
			result = CHARS[idx];
		}
		return {result, color};
	}

	void ConsoleWindow::Reallocate()
	{
		size_t size = m_screenWidth * m_screenHeight;
		if (m_screenBuffer.size() != size)
		{
			m_screenBuffer.resize(size);
			std::fill_n(m_screenBuffer.begin(), size, CHAR_INFO{});
		}
	}

	void ConsoleWindow::PushBuffer() const
	{
		COORD dwBufferSize = { static_cast<short>(m_screenWidth), static_cast<short>(m_screenHeight)};
		COORD dwBufferCoord = { 0, 0 };
		SMALL_RECT rcRegion = { 0, 0, static_cast<short>(m_screenWidth - 1), static_cast<short>(m_screenHeight- 1) };
		WriteConsoleOutput(m_handle, m_screenBuffer.data(), dwBufferSize, dwBufferCoord, &rcRegion);
		// WriteConsoleOutputCharacter(m_handle, m_screenBuffer, m_screenWidth * m_screenHeight, { 0,0 }, &m_dwBytesWritten);
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

	void ConsoleWindow::Update(const std::vector<Math::Sphere>& spheres, Player& player)
	{
		m_player = &player;
		auto now = std::chrono::system_clock::now();
		std::chrono::duration<float> duration = now - m_lastTime;
		m_lastTime = now;
		float deltaTime = duration.count();
		auto size = GetSize();

		if (size.x != m_screenWidth || size.y != m_screenHeight)
		{
			m_screenWidth = size.x;
			m_screenHeight = size.y;
			player.GetCamera().OnResize(size);
			Reallocate();
			ClearScreen();
		}
		Render(spheres);
		//
		// const auto& spheres = m_worldMap.GetSpheres();
		//
		// for (int x = 0; x < m_screenWidth; ++x)
		// {
		// 	float fRayAngle = (m_playerAngle - m_FOV / 2.0f) + (static_cast<float>(x) / static_cast<float>(m_screenWidth)) * m_FOV;
		//
		// 	float eyeX = sinf(fRayAngle);
		// 	float eyeY = cosf(fRayAngle);
		//
		// 	float distanceToObject = 0.0f;
		// 	Math::Vector2f collision = Math::Vector2f(0.0f, 0.0f);
		// 	bool hitWall = false;
		//
		//
		// 	Math::Ray ray{m_playerPos, {eyeX, eyeY}};
		// 	
		// 	while (!hitWall && distanceToObject < m_depth)
		// 	{
		// 		distanceToObject += 0.1f;
		//
		// 		int testX = static_cast<int>(m_playerPos.x + eyeX * distanceToObject);
		// 		int testY = static_cast<int>(m_playerPos.y + eyeY * distanceToObject);
		// 		if (testX < 0 || testX >= m_mapSize.x || testY < 0 || testY >= m_mapSize.y)
		// 		{
		// 			hitWall = true;
		// 			distanceToObject = m_depth;
		// 		}
		// 		else
		// 		{
		// 			if (m_worldMap.GetElement({testX, testY}) == '#')
		// 			{
		// 				hitWall = true;
		// 			}
		// 		}
		//
		// 	}
		// 	int ceiling = (float)(m_screenHeight/2.0) - m_screenHeight / ((float)distanceToObject);
		// 	int floor = m_screenHeight - ceiling;
		//
		// 	std::wstring chars = L"@&%QWNM0gB$#DR8mHXKAUbGOpV4d9h6PkqwSE2]ayjxY5Zoen[ult13If}C{iF|(7J)vTLs?z/*cr!+<>;=^,_:'-.` ";
		//
		// 	int idx = (distanceToObject / m_depth) * chars.size();
		//
		// 	wchar_t shade = chars[idx];
		// 	for (int y = 0; y < m_screenHeight; y++)
		// 	{
		// 		if (y < ceiling)
		// 			m_screenBuffer[y * m_screenWidth + x] = ' ';
		// 		else if (y > ceiling && y < floor)
		// 		{
		// 			m_screenBuffer[y * m_screenWidth + x] = shade;
		// 		}
		// 		else
		// 		{
		// 			float b = 1.0f - (((float)y - m_screenHeight / 2.0f) / ((float)m_screenHeight / 2.0f));
		// 			int idx2 = b * chars.size();
		// 			short otherShade;
		// 			if (b > 1.0f) otherShade = ' ';
		// 			 else otherShade = chars[idx2];
		// 			// if (b < 0.25f)		otherShade = '#';
		// 			// else if (b < 0.5f)	otherShade = 'x';
		// 			// else if (b < 0.75f)	otherShade = '-';
		// 			// else if (b < 0.9f)	otherShade = '.';
		// 			// else				otherShade = ' ';
		// 			m_screenBuffer[y * m_screenWidth + x] = otherShade;
		// 		}
		// 	}
		// }
		
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
