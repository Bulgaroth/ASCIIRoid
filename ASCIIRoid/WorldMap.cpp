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


    for (int x = 0; x < m_size.x; x++)
    {
        for (int y = 0; y < m_size.y; y++)
        {
            if (GetElement({x, y}) == L'#')
            {
                elements.push_back(
                    Math::Sphere{
                        Math::Vector2f{
                            static_cast<float>(x), static_cast<float>(y)
                        },0.5f
                    });
            }
        }
    }
}

wchar_t WorldMap::GetElement(Math::Vector2i pos) const
{
    return m_map.c_str()[pos.x + pos.y * m_size.x];
}

const std::vector<Math::Sphere>& WorldMap::GetSpheres() const
{
    return elements;
}

const Math::Vector2i& WorldMap::GetSize() const
{
    return m_size;
}
