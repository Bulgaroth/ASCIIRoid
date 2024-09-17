#pragma once

#include <vector>
#include "Math/Vector2.hpp"
#include "Asteroid.hpp"
#include <windows.h>

#define SCREEN_HEIGHT 46
#define SCREEN_WIDTH 146

class Controller
{
public:
	Controller(Math::Vector2i size);
	~Controller();

	void Update();
	void DrawMap();

private:
	Math::Vector2i m_size;
	std::vector<Asteroid*> asteroids;
	HANDLE m_hOutput;
	CHAR_INFO m_buffer[SCREEN_HEIGHT][SCREEN_WIDTH];

	bool CheckOutOfBounds(const Math::Vector2i& pos);
	void ClearMap();
};