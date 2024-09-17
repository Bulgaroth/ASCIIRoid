#include "WorldMap.hpp"

WorldMap::WorldMap(Math::Vector2i mapSize): m_size(mapSize)
{
    m_map += L"################";
    m_map += L"#..............#";
    m_map += L"#..............#";
    m_map += L"#..............#";
    m_map += L"#..............#";
    m_map += L"#..............#";
    m_map += L"#..............#";
    m_map += L"#..............#";
    m_map += L"#..............#";
    m_map += L"#..............#";
    m_map += L"#..............#";
    m_map += L"#..............#";
    m_map += L"#..............#";
    m_map += L"#..............#";
    m_map += L"#..............#";
    m_map += L"################";
}

wchar_t WorldMap::GetElement(Math::Vector2i pos) const
{
    return m_map.c_str()[pos.x + pos.y * m_size.x];
}
