#pragma once
#include "raylib-cpp.hpp" // Para raylib::Vector2

namespace Utils {
bool CheckLineRectangleCollision(raylib::Vector2 p1, raylib::Vector2 p2,
                                 raylib::Rectangle rec);
} // namespace Utils
