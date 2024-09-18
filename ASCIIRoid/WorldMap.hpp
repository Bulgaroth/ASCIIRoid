#pragma once
#include <string>
#include <vector>

#include "Math/Sphere.hpp"
#include "Math/Vector2.hpp"


class WorldMap
{
public:
    WorldMap(Math::Vector2i mapSize);
    wchar_t GetElement(Math::Vector2i pos) const;
    const std::vector<Math::Sphere>& GetSpheres() const;
    const Math::Vector2i& GetSize() const;

private:
    Math::Vector2i m_size;
    std::wstring m_map;
    std::vector<Math::Sphere> elements;
};
