#include "ConsoleRenderer.hpp"

#include <string>

ConsoleRenderer::ConsoleWindow::ConsoleWindow(int width, int height)
    : m_Width(width), m_Height(height)
{
}

ConsoleRenderer::ConsoleWindow::~ConsoleWindow() = default;

std::ostream& ConsoleRenderer::operator<<(std::ostream& stream, const ConsoleWindow& consoleWindow)
{
    return stream << "Width: " + std::to_string(consoleWindow.m_Width) + ", Height: " + std::to_string(consoleWindow.m_Height);
}
