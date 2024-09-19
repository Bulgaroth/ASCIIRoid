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
	bool end;

	Controller(Math::Vector2i size);
	~Controller();

	void Update();

private:
	Math::Vector2i m_size;
	std::vector<Asteroid*> m_asteroids;
	std::vector<Asteroid*> m_projectiles;
	int m_nbFrames;
	const Math::Vector2f m_playerPos = Math::Vector2f{ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };

	Math::Vector2f m_shootOriginPos;
	int m_shootPosIndex = 0;

	bool m_keyDownL, m_keyDownR, m_keyDownS;

	const std::vector<Math::Vector2f> m_neighboors
	{
		Math::Vector2f(0.f,-1.f), Math::Vector2f(1.f,-1.f), Math::Vector2f(1.f,0.f),
		Math::Vector2f(1.f,1.f), Math::Vector2f(0.f,1.f), Math::Vector2f(-1.f,1.f),
		Math::Vector2f(-1.f,0.f), Math::Vector2f(-1.f,-1.f)
	};

	HANDLE m_hOutput;
	CHAR_INFO m_buffer[SCREEN_HEIGHT][SCREEN_WIDTH];

	const Math::Vector2i& GetPlayerPos() 
	{
		return { static_cast<int>(m_playerPos.x),
				 static_cast<int>(m_playerPos.y) };
	}

	void DrawMap();
	void DrawAsteroid(const Asteroid& asteroid);
	void ClearMap();
	void DrawEndScreen();

	bool CheckOutOfBounds(const Math::Vector2f& pos);
	bool CheckOutOfBounds(const int& x, const int& y);

	void HandleInputs();
	void TurnPlayer(bool left);
	void Shoot();

	void HandleCollisions();
	void UpdateElements(std::vector<Asteroid*>& list);
	void SpawnAsteroid();
	void SpawnAsteroid(int size, Math::Vector2f pos, Math::Vector2f velocity);
	void SpawnAsteroid(Asteroid* asteroid);
	void SpawnProjectile(Math::Vector2f pos, Math::Vector2f velocity);

	std::pair<Asteroid*, Asteroid*> GetSplitAsteroids(Math::Vector2f pos, int size);
};