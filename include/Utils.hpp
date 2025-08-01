#pragma once
#include "raylib-cpp.hpp" // Para raylib::Vector2

// Calcula la distancia euclidiana entre dos puntos (Vector2).
namespace Utils {
float GetDistance(raylib::Vector2 p1, raylib::Vector2 p2);

void ClearInputBuffer();
bool CheckLineRectangleCollision(raylib::Vector2 p1, raylib::Vector2 p2,
                                 raylib::Rectangle rec);
} // namespace Utils
