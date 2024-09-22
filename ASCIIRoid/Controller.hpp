#pragma once

#include <chrono>
#include <vector>
#include "Math/Vector2.hpp"
#include "Asteroid.hpp"
#include <windows.h>

#include "Player.hpp"
#include "ConsoleRenderer/ConsoleRenderer.hpp"
#include "Math/Sphere.hpp"

#define SCREEN_HEIGHT 46
#define SCREEN_WIDTH 146

class Controller
{
public:
	bool end;
	int score;

	Controller(Math::Vector2i size);
	~Controller();

	std::vector<Math::Sphere> GetSpheres() const;
	Player& GetPlayer() { return m_player; }

	void DrawMap(ConsoleRenderer::ConsoleWindow& window);

	void Update();

private:
	std::chrono::system_clock::time_point m_lastTime;
	float dt;
	Math::Vector2i m_size;
	std::vector<Asteroid*> m_asteroids;
	std::vector<Asteroid*> m_projectiles;
	float m_timeElapsed;

	Player m_player { Math::Vector2i{SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2} };

	HANDLE m_hOutput;
	CHAR_INFO m_buffer[SCREEN_WIDTH * SCREEN_HEIGHT];
	void DrawAsteroid(const Asteroid& asteroid);
	void ClearMap();
	void DrawEndScreen();

	template<typename T>
	bool CheckOutOfBounds(const Math::Vector2<T>& pos) const;

	void HandleInputs();
	void CheckShoot();

	void HandleCollisions();
	void UpdateElements(std::vector<Asteroid*>& list);
	void SpawnAsteroid();
	void SpawnAsteroid(int size, Math::Vector2f pos, Math::Vector2f velocity);
	void SpawnAsteroid(Asteroid* asteroid);
	void SpawnProjectile(Math::Vector2f pos, Math::Vector2f velocity);

	std::pair<Asteroid*, Asteroid*> GetSplitAsteroids(Math::Vector2f pos, int size);
};