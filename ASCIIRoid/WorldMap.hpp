#pragma once
#include <string>

#include "Math/Vector2.hpp"


class WorldMap
{
public:
    WorldMap(Math::Vector2i mapSize);
    wchar_t GetElement(Math::Vector2i pos) const;

private:
    Math::Vector2i m_size;
    std::wstring m_map;
};
