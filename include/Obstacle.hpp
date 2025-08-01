#pragma once
#include "raylib-cpp.hpp"
#include <string>

// Estructura para representar un obstáculo con su forma y nombre
struct Obstacle {
  raylib::Rectangle rect;
  std::string name;
  raylib::Color color;

  Obstacle() : rect{0, 0, 0, 0}, name(""), color(DARKBLUE) {}

  Obstacle(raylib::Rectangle r, const std::string& n,
           raylib::Color c = DARKBLUE) :
      rect(r),
      name(n),
      color(c) {}
};
