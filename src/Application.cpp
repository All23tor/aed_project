#include "Application.h"
#include <iostream>
#include <vector>
#include "Constants.h" 
#include "DijkstraStrategy.h"
#include "AStarStrategy.h"

// Constructor de la clase Application
Application::Application(int numNodes) {

    if (numNodes <= GRAPHICAL_NODE_LIMIT) {
        initializer = std::make_unique<GraphicalInitializer>();
        graphicalMode = true;
    } else {
        initializer = std::make_unique<TerminalInitializer>(numNodes);
        graphicalMode = false;
    }
  
    bool useAStar = true; 


    if (useAStar) {
        myPathfinding = std::make_unique<Pathfinding>(myGraph, std::make_unique<AStarStrategy>());
    } else {
        myPathfinding = std::make_unique<Pathfinding>(myGraph, std::make_unique<DijkstraStrategy>());
    }


    initializer->Initialize();


    myGraph.generateRandomNodes(numNodes, SCREEN_WIDTH, SCREEN_HEIGHT,
                                MAX_EDGES_PER_NODE_GENERATION, MAX_CONNECTION_DISTANCE_GENERATION);

    myGraph.generateRandomObstacles(NUM_OBSTACLES, SCREEN_WIDTH, SCREEN_HEIGHT);
    

    if (!graphicalMode) {
        std::cout << "\n--- Obstaculos Generados ---" << std::endl;
        if (myGraph.getObstacles().empty()) {
            std::cout << "No se generaron obstaculos." << std::endl;
        } else {
            int obsCount = 0;
            for (const auto& obs : myGraph.getObstacles()) {
                std::cout << "  Obstaculo #" << obsCount++ << " (" << obs.name << "): "
                          << "Pos=(" << obs.rect.x << ", " << obs.rect.y << "), "
                          << "Tam=(" << obs.rect.width << ", " << obs.rect.height << ")" << std::endl;
            }
        }
        std::cout << "----------------------------" << std::endl;
    }
}
void Application::Run() {
    if (graphicalMode) {
        runGraphicalMode();
    } else {
        runTerminalMode();
    }
}

void Application::runGraphicalMode() {
    raylib::Window window(SCREEN_WIDTH, SCREEN_HEIGHT, "RayPathFinder - Modo Grafico");
    SetTargetFPS(60);

    raylib::Camera2D camera;
    camera.SetTarget(raylib::Vector2((float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2));
    camera.SetOffset(raylib::Vector2((float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2));
    camera.SetRotation(0.0f);
    camera.SetZoom(1.0f);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode2D(camera);

        // ¡Aquí llamamos a la función de dibujo de la propia clase Application!
        drawGraphElements();

        EndMode2D();

        EndDrawing();
    }
}


void Application::runTerminalMode() {
    std::cout << "\n--- Ejecución en Modo Terminal ---" << std::endl;

    int startID, endID;
    std::cout << "Ingrese el ID del nodo de inicio: ";
    std::cin >> startID;
    std::cout << "Ingrese el ID del nodo de destino: ";
    std::cin >> endID;

    if (!myGraph.isValidNodeId(startID) || !myGraph.isValidNodeId(endID)) {
        std::cout << "IDs inválidos. Asegúrese de que existan los nodos." << std::endl;
        return;
    }

    std::cout << "Ejecutando algoritmo de pathfinding...\n";
    
    // El camino es retornado, lo guardamos en la variable 'path' de la clase
    path = myPathfinding->findPath(startID, endID);

    if (path.empty()) {
        std::cout << "No se encontró un camino entre los nodos." << std::endl;
    } else {
        std::cout << "Camino encontrado:" << std::endl;
        for (const int& nodeId : path) {
            std::cout << "Nodo ID: " << nodeId << std::endl;
        }
    }
    std::cout << "--- Fin de la ejecución en terminal ---" << std::endl;
}
void Application::drawGraphElements() {
    // 1. Dibujar los obstáculos
    for (const auto& obs : myGraph.getObstacles()) {
        DrawRectangleRec(obs.rect, obs.color);
    }
    
    // 2. Dibujar las aristas del grafo
    for (int i = 0; i < myGraph.getNumNodes(); ++i) {
        const Node& startNode = myGraph.getNode(i);
        for (const auto& edge : myGraph.getAdjacentNodes(i)) {
            const Node& endNode = myGraph.getNode(edge.first);
            DrawLineEx(startNode.position, endNode.position, 1, Fade(LIGHTGRAY, 0.6f));
        }
    }

    // 3. Dibujar los nodos
    for (int i = 0; i < myGraph.getNumNodes(); ++i) {
        const Node& node = myGraph.getNode(i);
        DrawCircleV(node.position, 5, BLUE);
    }

    // 4. Dibujar el camino encontrado
    if (!path.empty()) {
        auto it = path.begin();
        const Node& prevNode = myGraph.getNode(*it);
        DrawCircleV(prevNode.position, 10, GREEN); // Nodo de inicio
        ++it;
        
        for (; it != path.end(); ++it) {
            const Node& nextNode = myGraph.getNode(*it);
            DrawLineEx(prevNode.position, nextNode.position, 3, RED);
            DrawCircleV(nextNode.position, 10, RED); // Nodo de destino y nodos intermedios
        }
    }
}


Application::~Application() {
    std::cout << "Recursos liberados. Adios!" << std::endl;
}