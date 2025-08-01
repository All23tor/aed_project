#pragma once

#include "IInitializer.h"
#include "GraphicalInitializer.h"
#include "TerminalInitializer.h"
#include "Estructuras_datos/MyVector.h"
#include "Constants.h"
#include "IPathfindingStrategy.h"
#include "Pathfinding.h"
#include "DijkstraStrategy.h" 
#include "AStarStrategy.h"
#include <memory>
#include <iostream>

class Application {
public:
    // El constructor inicia el modo y el algoritmo
    explicit Application(int numNodes);
    ~Application();
    void Run();

private:
    // Punteros a los "estrategas"
    std::unique_ptr<IInitializer> initializer;
    std::unique_ptr<Pathfinding> myPathfinding;
    
    // El resto de los miembros de la aplicación
    Graph myGraph;
    bool graphicalMode;

    // Variables de estado del pathfinding
    SimpleList<int> path;
    int startNodeId = -1;
    int endNodeId = -1;

    // Variables de estado del modo gráfico
    bool editingObstacles = false;
    bool isDragging = false;
    bool typingObstacleName = false;
    char obstacleNameInput[256] = "\0";
    Vector2 startDragPos = {0, 0};
    Rectangle nameInputBox = { 10, 270, 250, 25 };

    // Métodos privados para la lógica principal de la aplicación
    void runGraphicalMode();
    void runTerminalMode();
    void updateGraphicalMode(float deltaTime);
    void drawGraphicalMode();
    void handleGraphicalInput();
    void drawGraphElements();
    void drawGraphicalHUD();

    // Métodos privados para el modo terminal
    void printPathToTerminal(const SimpleList<int>& p) const;
    int getValidNodeIdFromUser(const std::string& prompt, int maxId) const;
};