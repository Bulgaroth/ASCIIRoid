#pragma once

class Asteroid
{
public:
	Asteroid(int size, int color);
	~Asteroid() = default;

	void Update();

private:
	int m_size, m_color;
};