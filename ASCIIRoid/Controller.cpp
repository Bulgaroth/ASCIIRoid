#include <iterator>
#include <cwchar>

#include "Controller.hpp"
#include "Asteroid.hpp"

#define MAX_ENTITIES 150


Controller::Controller(Math::Vector2i size) : m_size(size)
{
	asteroids.push_back(new Asteroid(1, 0x0B, Math::Vector2i(0, 0), Math::Vector2i(1, 1)));
	asteroids.push_back(new Asteroid(1, 0x0E, Math::Vector2i(2, 5)));
	asteroids.push_back(new Asteroid(1, 0x0A, Math::Vector2i(8, 3)));
	asteroids.push_back(new Asteroid(1, 0x0F, Math::Vector2i(4, 7)));

	m_hOutput = (HANDLE)GetStdHandle(STD_OUTPUT_HANDLE);

	/*buffer = new CHAR_INFO * [size.y];
	for (int i = 0; i < size.y; ++i) buffer[i] = new CHAR_INFO[size.x];*/
}

Controller::~Controller()
{
	/*for (int i = 0; i < m_size.y; ++i) delete[] buffer[i];
	delete[] buffer;*/
	for (int i = 0; i < asteroids.size(); ++i) delete asteroids[i];
}

void Controller::Update()
{
	for (const auto& asteroid : asteroids)
	{
		asteroid->Update();
		if (CheckOutOfBounds(asteroid->GetPos())) asteroid->Delete();
	}
	ClearMap();
	DrawMap();
}

void Controller::DrawMap()
{
	COORD dwBufferSize = { SCREEN_WIDTH, SCREEN_HEIGHT };
	COORD dwBufferCoord = { 0, 0 };
	SMALL_RECT rcRegion = { 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1 };

	/*ReadConsoleOutput(m_hOutput, (CHAR_INFO*)m_buffer, dwBufferSize,
		dwBufferCoord, &rcRegion);*/

	for (const auto& asteroid : asteroids)
	{
		const Math::Vector2i& pos = asteroid->GetPos();
		m_buffer[pos.y][pos.x].Char.AsciiChar = 'O';
		m_buffer[pos.y][pos.x].Attributes = asteroid->GetColor();
	}

	/*buffer[5][10].Char.AsciiChar = 'H';
	buffer[5][10].Attributes = 0x0E;
	buffer[5][11].Char.AsciiChar = 'i';
	buffer[5][11].Attributes = 0x0B;
	buffer[5][12].Char.AsciiChar = '!';
	buffer[5][12].Attributes = 0x0A;*/

	WriteConsoleOutput(m_hOutput, (CHAR_INFO*)m_buffer, dwBufferSize,
		dwBufferCoord, &rcRegion);
}

bool Controller::CheckOutOfBounds(const Math::Vector2i& pos)
{
	return pos.x < m_size.x || pos.x > m_size.x || pos.y < m_size.y || pos.y > m_size.y;
}

void Controller::ClearMap()
{
	for (int y = 0; y < SCREEN_HEIGHT; ++y)
	{
		std::fill_n(m_buffer[y], SCREEN_WIDTH, CHAR_INFO{});
	}
	
}

//ReadConsoleInput()
//INPUT_RECORD