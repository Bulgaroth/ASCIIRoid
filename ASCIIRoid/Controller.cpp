#include <iterator>
#include <cwchar>
#include <random>
#include <vector>

#include "Controller.hpp"

#include <chrono>

#include "Asteroid.hpp"

#define MAX_ENTITIES 150
#define START_NB_ASTEROIDS 10;
#define ASTEROID_SPAWN_RATE 10;

#pragma region Constructeurs

Controller::Controller(Math::Vector2i size) : m_size(size)
{
	end = false; 

	for (int i = 0; i < 5; i++) SpawnAsteroid();

	m_hOutput = (HANDLE)GetStdHandle(STD_OUTPUT_HANDLE);
}

Controller::~Controller()
{
	for (int i = 0; i < m_asteroids.size(); ++i) delete m_asteroids[i];
	for (int i = 0; i < m_projectiles.size(); ++i) delete m_projectiles[i];
}
#pragma endregion

#pragma region Runtime

void Controller::Update()
{
	auto now = std::chrono::system_clock::now();
	std::chrono::duration<float> duration = now - m_lastTime;
	m_lastTime = now;
	dt = duration.count();
	if (++m_nbFrames >= 1000)
	{
		m_nbFrames = 0;
		SpawnAsteroid();
	}

	HandleInputs();
	CheckShoot();
	UpdateElements(m_asteroids);
	UpdateElements(m_projectiles);
	ClearMap();
	DrawMap();
	HandleCollisions();
}

void Controller::UpdateElements(std::vector<Asteroid*>& list)
{
	for (auto itr = list.begin(); itr != list.end(); )
	{
		Asteroid* el = *itr;
		el->Update(dt);

		if (CheckOutOfBounds(el->GetPosF()))
		{
			itr = list.erase(itr);
			delete el;
		}
		else itr++;
	}
}

void Controller::HandleCollisions()
{
	// V�rification de la condition de d�faite.
	for (auto itr = m_asteroids.begin(); itr != m_asteroids.end(); ++itr)
	{
		Asteroid* asteroid = *itr;
		const Math::Vector2i asteroidCenter = asteroid->GetPos();
		float dst = Math::Vector2i::Distance(asteroidCenter, m_player.GetPos<int>());

		if (dst < asteroid->GetSize())
		{
    		DrawEndScreen();
			end = true;
			return;
		}
	}

	std::vector<Asteroid*> asteroidsToSpawn{};

	bool projectileHit = false;
	// V�rification des destructions d'ast�roides.
	for (auto projItr = m_projectiles.begin(); projItr != m_projectiles.end();)
	{
		Asteroid* projectile = *projItr;
		const Math::Vector2f projPos = projectile->GetPosF();
		for (auto asterItr = m_asteroids.begin(); asterItr != m_asteroids.end();)
		{
			Asteroid* asteroid = *asterItr;
			const Math::Vector2f asteroidCenter = asteroid->GetPosF();

			if (Math::Vector2f::Distance(projPos, asteroidCenter) <= asteroid->GetSize())
			{
				projectileHit = true;

				if (asteroid->GetSize() > 1)
				{
					auto splitAsteroids = GetSplitAsteroids(asteroidCenter, asteroid->GetSize());
					asteroidsToSpawn.push_back(splitAsteroids.first);
					asteroidsToSpawn.push_back(splitAsteroids.second);
				}

				++score;

				asterItr = m_asteroids.erase(asterItr);
				delete asteroid;
			}
			else ++asterItr;
		}
		if (projectileHit)
		{
			projItr = m_projectiles.erase(projItr);
			delete projectile;
		}
		else ++projItr;
	}

	for (const auto& asteroid : asteroidsToSpawn) SpawnAsteroid(asteroid);
}

#pragma region Affichage

void Controller::DrawMap()
{
	COORD dwBufferSize = { SCREEN_WIDTH, SCREEN_HEIGHT };
	COORD dwBufferCoord = { 0, 0 };
	SMALL_RECT rcRegion = { 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1 };

	for (const auto& asteroid : m_asteroids) DrawAsteroid(*asteroid);

	for (const auto& projectile : m_projectiles)
	{
		const Math::Vector2i& pos = projectile->GetPos();

		CHAR_INFO& cell = m_buffer[pos.y][pos.x];
		cell.Char.UnicodeChar = projectile->GetChar(pos);
		cell.Attributes = projectile->GetColor();
	}

	const Math::Vector2i& playerPos = m_player.GetPos<int>();
	const Math::Vector2i& shootPos = m_player.GetCanonPos<int>();

	CHAR_INFO& playerCell = m_buffer[playerPos.y][playerPos.x];
	playerCell.Char.UnicodeChar = L'A';
	playerCell.Attributes = 0x0F;

	CHAR_INFO& shootCell = m_buffer[shootPos.y][shootPos.x];
	shootCell.Char.UnicodeChar = L'i';
	shootCell.Attributes = 0x0E;

	WriteConsoleOutput(m_hOutput, (CHAR_INFO*)m_buffer, dwBufferSize,
		dwBufferCoord, &rcRegion);
}

void Controller::DrawAsteroid(const Asteroid& asteroid)
{
	const Math::Vector2i& center = asteroid.GetPos();
	int start = -asteroid.GetSize();
	int end = -start;

	for (int y = start; y <= end; ++y)
	{
		for (int x = start; x <= end; ++x)
		{
			const Math::Vector2i& pos{ center.x + x, center.y + y };
			const Math::Vector2i& charPos{ x, y };

			if (CheckOutOfBounds(pos)) continue;
			CHAR_INFO &cell = m_buffer[pos.y][pos.x];
			cell.Char.UnicodeChar = asteroid.GetChar(charPos);
			cell.Attributes = asteroid.GetColor();
		}
	}
}

void Controller::ClearMap()
{
	for (int y = 0; y < SCREEN_HEIGHT; ++y)
	{
		std::fill_n(m_buffer[y], SCREEN_WIDTH, CHAR_INFO{});
	}
}

void Controller::DrawEndScreen()
{
	COORD dwBufferSize = { SCREEN_WIDTH, SCREEN_HEIGHT };
	COORD dwBufferCoord = { 0, 0 };
	SMALL_RECT rcRegion = { 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1 };

	ClearMap();

	WriteConsoleOutput(m_hOutput, (CHAR_INFO*)m_buffer, dwBufferSize,
		dwBufferCoord, &rcRegion);
}
#pragma endregion

#pragma region Inputs

void Controller::HandleInputs()
{
	m_player.HandleInput(dt);
}

void Controller::CheckShoot()
{
	if (!m_player.IsShooting()) return;

	auto velocity = m_player.GetProjectileVelocity<float>();
	SpawnProjectile(m_player.GetCanonPos<float>() + velocity, velocity);
}
#pragma endregion

#pragma endregion

#pragma region Spawn

void Controller::SpawnAsteroid()
{
	// Initialisation du g�n�rateur de nombre al�atoire.
	std::random_device rd;
	std::mt19937 mt(rd());

	// Initialisation des distributions.
	std::uniform_int_distribution<int> zoneDist(0, 3);
	std::uniform_int_distribution<int> sizeDist(1, 3);
	std::uniform_int_distribution<int> velocityDist(-1, 2);

	// Index de la zone de spawn (0 en haut, 1 � droite, 2 en bas, 3 � gauche de la console)
	int spawnZoneIndex = zoneDist(mt);
	// G�n�ration al�atoire de la taille de l'ast�roide.
	int size = sizeDist(mt);

	// Distribution sur la totalit� de la bound en fonction de si elle est horizontale ou verticale.
	std::uniform_int_distribution<int> posDist;
	posDist = (spawnZoneIndex == 0 || spawnZoneIndex == 2) ?
		std::uniform_int_distribution<int>(1, SCREEN_WIDTH - 2) :
		std::uniform_int_distribution<int>(1, SCREEN_HEIGHT - 2);

	// Placement de l'ast�roide sur l'une des bounds de la console et
	// g�n�ration de sa v�locit� initiale.
	Math::Vector2f pos;
	Math::Vector2f velocity;
	switch (spawnZoneIndex)
	{
	case 0: pos = Math::Vector2f(posDist(mt), 0.0f);
		velocity = Math::Vector2f(velocityDist(mt), 0.5f);
		break;
	case 1: pos = Math::Vector2f(SCREEN_WIDTH, posDist(mt));
		velocity = Math::Vector2f(-1.0f, velocityDist(mt)/2);
		break;
	case 2: pos = Math::Vector2f(posDist(mt), SCREEN_HEIGHT);
		velocity = Math::Vector2f(velocityDist(mt), -0.5f);
		break;
	default: pos = Math::Vector2f(0.0f, posDist(mt));
		velocity = Math::Vector2f(1.0f, velocityDist(mt)/2);
	}

	SpawnAsteroid(size, pos, velocity);
}

void Controller::SpawnAsteroid(int size, Math::Vector2f pos, Math::Vector2f velocity)
{
	m_asteroids.push_back(new Asteroid{ size, pos, velocity });
}

void Controller::SpawnAsteroid(Asteroid* asteroid)
{
	m_asteroids.push_back(asteroid);
}

void Controller::SpawnProjectile(Math::Vector2f pos, Math::Vector2f velocity)
{
	int color = 0x0E;
	int size = 0;

	m_projectiles.push_back(new Asteroid{ size, pos, velocity });
}

std::pair<Asteroid*, Asteroid*> Controller::GetSplitAsteroids(Math::Vector2f pos, int size)
{
	// Initialisation du g�n�rateur de nombre al�atoire.
	std::random_device rd;
	std::mt19937 mt(rd());

	// Initialisation de la distribution.
	std::uniform_int_distribution<int> dist(0, 7); // 8 directions possibles.

	Math::Vector2f posDelta = Math::Utility::Neighbours[dist(mt)];
	Math::Vector2f velocity(posDelta.x, posDelta.y);

	// Heap allocated. Will be deleted either in the HandleCollisions method or in the destructor.
	return std::make_pair(new Asteroid{ size - 1, pos + posDelta, velocity }, new Asteroid{ size - 1, pos - posDelta, -velocity });
}
#pragma endregion

template<typename T>
bool Controller::CheckOutOfBounds(const Math::Vector2<T>& pos) const
{
	return pos.x < 0 || pos.x > m_size.x || pos.y < 0 || pos.y > m_size.y;
}



