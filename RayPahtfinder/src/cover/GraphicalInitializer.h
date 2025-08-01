#pragma once
#include "Constants.h" 
#include "IInitializer.h"
#include "raylib-cpp.hpp"
#include <iostream>

class GraphicalInitializer : public IInitializer {
public:
    void Initialize() override {
        std::cout << "\nModo Grafico Activado." << std::endl;
    }

private:
    raylib::Camera2D camera = {};
};