#include <iterator>
#include <cwchar>
#include <random>
#include <vector>

#include "Controller.hpp"
#include "Asteroid.hpp"

#define MAX_ENTITIES 150
#define START_NB_ASTEROIDS 10;
#define ASTEROID_SPAWN_RATE 10;

#pragma region Constructeurs

Controller::Controller(Math::Vector2i size) : m_size(size)
{
	end = false; 

	for (int i = 0; i < 10; i++) SpawnAsteroid();

	m_hOutput = (HANDLE)GetStdHandle(STD_OUTPUT_HANDLE);
	m_shootOriginPos = m_playerPos + m_neighboors[m_shootPosIndex];
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
	if (++m_nbFrames >= 10)
	{
		m_nbFrames = 0;
		SpawnAsteroid();
	}

	HandleInputs();
	UpdateElements(m_asteroids);
	UpdateElements(m_projectiles);
	HandleCollisions();
	ClearMap();
	DrawMap();
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
		cell.Char.UnicodeChar = projectile->GetChar();
		cell.Attributes = projectile->GetColor();
	}

	const Math::Vector2i& playerPos = GetPlayerPos();
	const Math::Vector2i& shootPos = { static_cast<int>(m_shootOriginPos.x), static_cast<int>(m_shootOriginPos.y) };

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
	int start, end;

	switch (asteroid.GetSize())
	{
		case 1: start = -1;
			break;
		case 2:
			start = -2;
			break;
		case 3: start = -4;
			break;
		default: start = 0;
	}
	end = -start;

	for (int y = start; y <= end; ++y)
	{
		for (int x = start; x <= end; ++x)
		{
			if (CheckOutOfBounds(x, y)) continue;
			CHAR_INFO &cell = m_buffer[center.y + y][center.x + x];
			cell.Char.UnicodeChar = asteroid.GetChar();
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

bool Controller::CheckOutOfBounds(const Math::Vector2f& pos)
{
	return pos.x < 0 || pos.x > m_size.x || pos.y < 0 || pos.y > m_size.y;
}

bool Controller::CheckOutOfBounds(const int& x, const int& y)
{
	return x < 0 || x > m_size.x || y < 0 || y > m_size.y;
}

#pragma region Inputs

void Controller::HandleInputs()
{
	if (GetAsyncKeyState((unsigned short)'Q') & 0x8000 ||
		GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		if (!m_keyDownL) TurnPlayer(true);
		m_keyDownL = true;
	}
	else m_keyDownL = false;

	if (GetAsyncKeyState((unsigned short)'D') & 0x8000 ||
		GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		if (!m_keyDownR) TurnPlayer(false);
		m_keyDownR = true;
	}
	else m_keyDownR = false;

	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		if (!m_keyDownS) Shoot();
		m_keyDownS = true;
	}
	else m_keyDownS = false;
}

void Controller::TurnPlayer(bool left)
{
	m_shootPosIndex += left ? -1 : 1;

	if (m_shootPosIndex < 0) m_shootPosIndex = m_neighboors.size() - 1;
	if (m_shootPosIndex > m_neighboors.size() - 1) m_shootPosIndex = 0;

	m_shootOriginPos = m_playerPos + m_neighboors[m_shootPosIndex];
}

void Controller::Shoot()
{
	Math::Vector2f delta = m_neighboors[m_shootPosIndex];
	Math::Vector2f velocity(delta.x, delta.y);
	SpawnProjectile(m_shootOriginPos + delta, velocity);
}
#pragma endregion

#pragma endregion

#pragma region Spawn

void Controller::SpawnAsteroid()
{
	// Initialisation du générateur de nombre aléatoire.
	std::random_device rd;
	std::mt19937 mt(rd());

	// Initialisation des distributions.
	std::uniform_int_distribution<int> zoneDist(0, 3);
	std::uniform_int_distribution<int> sizeDist(1, 3);
	std::uniform_int_distribution<int> velocityDist(-1, 2);

	// Index de la zone de spawn (0 en haut, 1 à droite, 2 en bas, 3 à gauche de la console)
	int spawnZoneIndex = zoneDist(mt);
	// Génération aléatoire de la taille de l'astéroide.
	int size = sizeDist(mt);

	// Distribution sur la totalité de la bound en fonction de si elle est horizontale ou verticale.
	std::uniform_int_distribution<int> posDist;
	posDist = (spawnZoneIndex == 0 || spawnZoneIndex == 2) ?
		std::uniform_int_distribution<int>(1, SCREEN_WIDTH - 2) :
		std::uniform_int_distribution<int>(1, SCREEN_HEIGHT - 2);

	// Placement de l'astéroide sur l'une des bounds de la console et
	// génération de sa vélocité initiale.
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
	// Initialisation du générateur de nombre aléatoire.
	std::random_device rd;
	std::mt19937 mt(rd());

	// Initialisation de la distribution.
	std::uniform_int_distribution<int> dist(0, m_neighboors.size() - 1);

	Math::Vector2f posDelta = m_neighboors[dist(mt)];
	Math::Vector2f velocity(posDelta.x, posDelta.y);

	return std::make_pair(new Asteroid{ size - 1, pos + posDelta, velocity }, new Asteroid{ size - 1, pos - posDelta, -velocity });
}
#pragma endregion

void Controller::HandleCollisions()
{
	// Vérification de la condition de défaite.
	for (auto itr = m_asteroids.begin(); itr != m_asteroids.end(); ++itr)
	{
		Asteroid* asteroid = *itr;
		const Math::Vector2f asteroidCenter = asteroid->GetPosF();
		if (Math::Vector2f::Distance(asteroidCenter, m_playerPos) <= asteroid->GetSize())
		{
 			DrawEndScreen();
			end = true;
			return;
		}
	}

	std::vector<Asteroid*> asteroidsToSpawn{};

	bool projectileHit = false;
	// Vérification des destructions d'astéroides.
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

void Controller::UpdateElements(std::vector<Asteroid*>& list)
{
	for (auto itr = list.begin(); itr != list.end(); )
	{
		Asteroid* el = *itr;
		el->Update();

		if (CheckOutOfBounds(el->GetPosF()))
		{
			itr = list.erase(itr);
			delete el;
		}
		else itr++;
	}
}