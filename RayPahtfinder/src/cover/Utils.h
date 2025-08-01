#pragma once

#include "Graph.h"         // Asegura Graph y Node
#include "Obstacle.h"      // Asegura Obstacle
#include "Estructuras_datos/list.h"  // Asegura SimpleList
#include "raylib-cpp.hpp"
#include <string>
#include <iostream>
#include <limits>

namespace Utils {
    float GetDistance(raylib::Vector2 p1, raylib::Vector2 p2);
    void ClearInputBuffer();
    bool CheckLineRectangleCollision(raylib::Vector2 p1, raylib::Vector2 p2, raylib::Rectangle rec);
}

// === Funciones auxiliares fuera del namespace Utils ===
bool AskUserGraphicalMode();

void DrawGraphElements(const Graph& graph, const SimpleList<int>& pathNodes, int startNodeId, int endNodeId);

int GetNodeIdFromMouse(const Graph& graph);

int GetValidNodeIdFromUser(const Graph& graph, const std::string& nombre);

void PrintPathToTerminal(const SimpleList<Node*>& path);
