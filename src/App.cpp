#include "App.hpp"
#include "Graph.hpp"
#include "Pathfinding.hpp"
#include <algorithm>
#include <chrono>
#include <cstring>
#include <string>

namespace {
static constexpr int SCREEN_WIDTH = 1200;
static constexpr int SCREEN_HEIGHT = 800;

Graph graph;
int startNodeId = -1;
int endNodeId = -1;
List<int> path;
raylib::Camera2D camera;

bool editingObstacles = false;
raylib::Vector2 startDragPos;
bool isDragging = false;
char obstacleNameInput[64] = "";
bool typingObstacleName = false;
Rectangle nameInputBox = {10, (float)SCREEN_HEIGHT - 40, 200, 30};

template <Algorithm alg>
void HandleGraphicalInput() {
  if (IsKeyPressed(KEY_O)) {
    editingObstacles = !editingObstacles;
    if (editingObstacles) {
      std::cout << "Modo de edicion de obstaculos ACTIVADO." << std::endl;

      startNodeId = -1;
      endNodeId = -1;
      path.clear();
    } else {
      std::cout << "Modo de edicion de obstaculos DESACTIVADO." << std::endl;

      typingObstacleName = false;
      obstacleNameInput[0] = '\0';
    }
  }

  if (editingObstacles) {

    raylib::Vector2 mouseWorldPos =
        GetScreenToWorld2D(GetMousePosition(), camera);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {

      if (!typingObstacleName) {
        startDragPos = mouseWorldPos;
        isDragging = true;
      }
    } else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
      if (isDragging) {
        isDragging = false;

        float x1 = std::min(startDragPos.x, mouseWorldPos.x);
        float y1 = std::min(startDragPos.y, mouseWorldPos.y);
        float x2 = std::max(startDragPos.x, mouseWorldPos.x);
        float y2 = std::max(startDragPos.y, mouseWorldPos.y);

        Rectangle newRect = {x1, y1, x2 - x1, y2 - y1};

        if (newRect.width > 5 && newRect.height > 5) {

          typingObstacleName = true;

        } else {

          typingObstacleName = false;
          obstacleNameInput[0] = '\0';
        }
      }
    }

    if (typingObstacleName) {
      int key = GetCharPressed();
      while (key > 0) {

        if (((key >= 32) && (key <= 125)) &&
            (strlen(obstacleNameInput) < sizeof(obstacleNameInput) - 1)) {
          obstacleNameInput[strlen(obstacleNameInput)] = (char)key;
          obstacleNameInput[strlen(obstacleNameInput) + 1] = '\0';
        }
        key = GetCharPressed();
      }

      if (IsKeyPressed(KEY_BACKSPACE)) {
        if (strlen(obstacleNameInput) > 0) {
          obstacleNameInput[strlen(obstacleNameInput) - 1] = '\0';
        }
      }

      if (IsKeyPressed(KEY_ENTER)) {
        if (strlen(obstacleNameInput) > 0) {

          float x1 = std::min(startDragPos.x, mouseWorldPos.x);
          float y1 = std::min(startDragPos.y, mouseWorldPos.y);
          float x2 = std::max(startDragPos.x, mouseWorldPos.x);
          float y2 = std::max(startDragPos.y, mouseWorldPos.y);
          Rectangle finalRect = {x1, y1, x2 - x1, y2 - y1};

          Color randomColor = {(unsigned char)GetRandomValue(50, 200),
                               (unsigned char)GetRandomValue(50, 200),
                               (unsigned char)GetRandomValue(50, 200), 255};
          graph.addObstacle(
              Obstacle(finalRect, std::string(obstacleNameInput), randomColor));
          std::cout << "Obstaculo '" << obstacleNameInput << "' anadido."
                    << std::endl;
        } else {
          std::cout << "Nombre de obstaculo vacio. Obstaculo no anadido."
                    << std::endl;
        }
        typingObstacleName = false;
        obstacleNameInput[0] = '\0';

        if (startNodeId != -1 && endNodeId != -1) {
          path = Pathfinding::findPath<alg>(graph, startNodeId, endNodeId);
        }
      }
    }

    if (IsKeyPressed(KEY_ESCAPE) && typingObstacleName) {
      typingObstacleName = false;
      obstacleNameInput[0] = '\0';
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && !isDragging) {
      for (int i = 0; i < graph.getObstacles().size(); ++i) {

        if (CheckCollisionPointRec(mouseWorldPos,
                                   graph.getObstacles()[i].rect)) {
          std::cout << "Obstaculo '" << graph.getObstacles()[i].name
                    << "' eliminado." << std::endl;
          graph.removeObstacle(i);

          if (startNodeId != -1 && endNodeId != -1) {
            path = Pathfinding::findPath<alg>(graph, startNodeId, endNodeId);
          }
          break;
        }
      }
    }

  } else {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      raylib::Vector2 mouseScreenPos = GetMousePosition();
      raylib::Vector2 mouseWorldPos =
          GetScreenToWorld2D(mouseScreenPos, camera);
      int clickedNodeId = graph.findNodeAtPosition(mouseWorldPos, 8.0f);

      if (clickedNodeId != -1) {
        if (startNodeId == -1) {

          startNodeId = clickedNodeId;
          endNodeId = -1;
          path.clear();
        } else if (endNodeId == -1 && clickedNodeId != startNodeId) {

          endNodeId = clickedNodeId;

          path = Pathfinding::findPath<alg>(graph, startNodeId, endNodeId);
        } else {

          startNodeId = clickedNodeId;
          endNodeId = -1;
          path.clear();
        }
      }
    }

    if (IsKeyPressed(KEY_R)) {
      if (startNodeId != -1 && endNodeId != -1) {
        path = Pathfinding::findPath<alg>(graph, startNodeId, endNodeId);
      }
    }

    if (IsKeyPressed(KEY_C)) {
      startNodeId = -1;
      endNodeId = -1;
      path.clear();
    }
  }
}

void DrawGraphElements(const Graph& graph, const List<int>& pathNodes,
                       int startNodeId, int endNodeId) {
  if (graph.getNumNodes() <= GRAPHICAL_NODE_LIMIT) {

    for (int i = 0; i < graph.getNumNodes(); ++i) {
      const Node& currentNode = graph.getNode(i);
      for (const auto& edge : graph.getAdjacentNodes(i)) {
        const Node& targetNode = graph.getNode(edge.first);
        DrawLineV(currentNode.position, targetNode.position, GRAY);
      }
    }

    for (int i = 0; i < graph.getNumNodes(); ++i) {
      const Node& currentNode = graph.getNode(i);
      Color nodeColor = BLUE;
      if (i == startNodeId)
        nodeColor = GREEN;
      else if (i == endNodeId)
        nodeColor = RED;

      DrawCircleV(currentNode.position, 5, nodeColor);
    }
  }

  if (!pathNodes.empty()) {
    raylib::Vector2 prevPos;
    bool firstNode = true;
    for (auto it = pathNodes.begin(); it != pathNodes.end(); ++it) {
      int nodeId = *it;
      if (nodeId < 0 || nodeId >= graph.getNumNodes())
        continue;

      const Node& currentPathNode = graph.getNode(nodeId);
      raylib::Vector2 currentPos = currentPathNode.position;

      if (!firstNode) {
        DrawLineEx(prevPos, currentPos, 4, LIME);
      }
      DrawCircleV(currentPos, 4, LIME);
      prevPos = currentPos;
      firstNode = false;
    }
  }

  if (startNodeId >= 0 && startNodeId < graph.getNumNodes()) {
    DrawCircleV(graph.getNode(startNodeId).position, 8, GREEN);
  }

  if (endNodeId >= 0 && endNodeId < graph.getNumNodes()) {
    DrawCircleV(graph.getNode(endNodeId).position, 8, RED);
  }
}

void DrawGraphicalHUD() {

  DrawText(TextFormat("Nodos: %i", graph.getNumNodes()), 10, 10, 20, DARKGRAY);

  if (startNodeId != -1)
    DrawText(TextFormat("Inicio: %i", startNodeId), 10, 30, 20, DARKGRAY);
  if (endNodeId != -1)
    DrawText(TextFormat("Fin: %i", endNodeId), 10, 50, 20, DARKGRAY);

  DrawText("Clic IZQ: Seleccionar Inicio/Fin", 10, 80, 15, BLACK);
  DrawText("Clic DER + Arrastrar: Paneo", 10, 100, 15, BLACK);
  DrawText("Rueda mouse: Zoom", 10, 120, 15, BLACK);
  DrawText("R: Recalcular ruta | C: Limpiar seleccion", 10, 140, 15, BLACK);

  if (startNodeId != -1 && endNodeId != -1 && path.empty()) {
    DrawText("No se encontro ruta. (Posiblemente desconectados o sin camino)",
             10, 160, 15, RED);
  }

  DrawText("O: Alternar modo edicion obstaculos", 10, 180, 15, BLACK);
  if (editingObstacles) {
    DrawText("En Modo Edicion Obstaculos:", 10, 200, 15, DARKBLUE);
    DrawText("Clic IZQ + Arrastrar: Dibujar obstaculo", 10, 220, 15, DARKBLUE);
    DrawText("Clic DER: Eliminar obstaculo", 10, 240, 15, DARKBLUE);
    DrawText("ESC: Cancelar nombre obstaculo", 10, 260, 15, DARKBLUE);
  }
}

void PrintPathToTerminal(const List<int>& p) {
  if (!p.empty()) {
    std::cout << "Ruta encontrada: ";
    bool first = true;
    for (int nodeId : p) {
      if (!first) {
        std::cout << " -> ";
      }
      std::cout << nodeId;
      first = false;
    }
    std::cout << std::endl;
  } else
    std::cout << "No se encontro ruta." << std::endl;
}

int GetValidNodeIdFromUser(const std::string& prompt, int maxId) {
  int nodeId;
  std::cout << prompt;
  std::cin >> nodeId;
  if (std::cin.fail()) {
    std::cerr << "Error: Entrada invalida. Por favor, ingrese un numero entero."
              << std::endl;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return -1;
  }
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

  if (nodeId < 0 || nodeId > maxId) {
    std::cerr << "Error: ID de nodo fuera de rango. Rango valido: 0 a " << maxId
              << std::endl;
    return -1;
  }
  return nodeId;
}

template <Algorithm alg>
void UpdateApplication() {
  if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
    raylib::Vector2 delta = GetMouseDelta();

    delta = delta.Scale(-1.0f / camera.zoom);
    camera.target = (raylib::Vector2)camera.target + delta;
  }

  float wheel = raylib::Mouse::GetWheelMove();
  if (wheel != 0) {

    raylib::Vector2 mouseScreenPos = GetMousePosition();
    raylib::Vector2 mouseWorldPos = GetScreenToWorld2D(mouseScreenPos, camera);

    camera.zoom += wheel * 0.1f;
    if (camera.zoom < 0.1f)
      camera.zoom = 0.1f;

    raylib::Vector2 newMouseWorldPos =
        GetScreenToWorld2D(GetMousePosition(), camera);
    raylib::Vector2 zoomCorrection = newMouseWorldPos - mouseWorldPos;
    camera.target = (raylib::Vector2)camera.target - zoomCorrection;
  }

  HandleGraphicalInput<alg>();
}

void DrawApplication() {
  BeginDrawing();
  ClearBackground(RAYWHITE);

  BeginMode2D(camera);

  for (const auto& obstacle : graph.getObstacles()) {
    DrawRectangleRec(obstacle.rect, Fade(obstacle.color, 0.7f));
    DrawRectangleLinesEx(obstacle.rect, 2, BLACK);

    DrawText(obstacle.name.c_str(), (int)(obstacle.rect.x + 5),
             (int)(obstacle.rect.y + 5), 10, BLACK);
  }

  if (editingObstacles && isDragging) {
    raylib::Vector2 mouseWorldPos =
        GetScreenToWorld2D(GetMousePosition(), camera);
    float x1 = std::min(startDragPos.x, mouseWorldPos.x);
    float y1 = std::min(startDragPos.y, mouseWorldPos.y);
    float x2 = std::max(startDragPos.x, mouseWorldPos.x);
    float y2 = std::max(startDragPos.y, mouseWorldPos.y);
    Rectangle currentRect = {x1, y1, x2 - x1, y2 - y1};
    DrawRectangleRec(currentRect, Fade(MAROON, 0.5f));
    DrawRectangleLinesEx(currentRect, 2, WHITE);
  }

  DrawGraphElements(graph, path, startNodeId, endNodeId);
  EndMode2D();

  DrawGraphicalHUD();

  if (editingObstacles && typingObstacleName) {
    DrawRectangleRec(nameInputBox, LIGHTGRAY);
    DrawRectangleLinesEx(nameInputBox, 2, BLACK);
    DrawText("Nombre Obstaculo (ENTER para confirmar):",
             (int)nameInputBox.x + 5, (int)nameInputBox.y - 15, 12, DARKGRAY);
    DrawText(obstacleNameInput, (int)nameInputBox.x + 5,
             (int)nameInputBox.y + 8, 15, BLACK);

    if (((int)(GetTime() * 2)) % 2 == 0) {
      DrawText("_",
               (int)nameInputBox.x + 5 + MeasureText(obstacleNameInput, 15),
               (int)nameInputBox.y + 8, 15, BLACK);
    }
  }

  EndDrawing();
}
} // namespace

void InitializeApplication(int numNodes) {
  static constexpr float NODE_GENERATION_AREA_PADDING = 50.0f;
  static constexpr int MAX_EDGES_PER_NODE_GENERATION = 4;
  static constexpr float MAX_CONNECTION_DISTANCE_GENERATION = 200.0f;
  static constexpr int NUM_OBSTACLES = 5;

  if (numNodes <= GRAPHICAL_NODE_LIMIT) {
    graphicalMode = true;
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "RayPathFinder - Modo Grafico");
    SetTargetFPS(60);

    camera.target = {(float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2};
    camera.offset = {(float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    std::cout << "\nModo Grafico Activado." << std::endl;
  } else {
    graphicalMode = false;
    std::cout << "\n------------------------------------------" << std::endl;
    std::cout << "MODO TERMINAL ACTIVADO (Nodos: " << numNodes << ")"
              << std::endl;
    std::cout << "------------------------------------------" << std::endl;
    std::cout << "El dibujo del grafo completo esta deshabilitado para mas de "
              << GRAPHICAL_NODE_LIMIT << " nodos." << std::endl;
  }

  graph.generateRandomNodes(numNodes, SCREEN_WIDTH, SCREEN_HEIGHT,
                            MAX_EDGES_PER_NODE_GENERATION,
                            MAX_CONNECTION_DISTANCE_GENERATION);

  size_t totalEdges = 0;
  for (int i = 0; i < graph.getNumNodes(); ++i) {
    totalEdges += graph.getAdjacentNodes(i).size();
  }
  std::cout << "--- Generacion del Grafo Completada ---" << std::endl;
  std::cout << "Numero de nodos generados: " << graph.getNumNodes()
            << std::endl;
  std::cout << "Numero total de entradas de aristas en lista de adyacencia: "
            << totalEdges << std::endl;
  std::cout << "Numero aproximado de aristas unicas (si no dirigido): "
            << totalEdges / 2 << std::endl;

  graph.generateRandomObstacles(NUM_OBSTACLES, SCREEN_WIDTH, SCREEN_HEIGHT);

  if (!graphicalMode) {
    std::cout << "\n--- Obstaculos Generados ---" << std::endl;
    if (graph.getObstacles().empty()) {
      std::cout << "No se generaron obstaculos." << std::endl;
    } else {
      int obsCount = 0;
      for (const auto& obs : graph.getObstacles()) {
        std::cout << "  Obstaculo #" << obsCount++ << " (" << obs.name << "): "
                  << "Pos=(" << obs.rect.x << ", " << obs.rect.y << "), "
                  << "Tam=(" << obs.rect.width << ", " << obs.rect.height << ")"
                  << std::endl;
      }
    }
    std::cout << "----------------------------" << std::endl;
  }
}

template <Algorithm alg>
void RunGraphicalMode() {
  while (!WindowShouldClose()) {
    UpdateApplication<alg>();
    DrawApplication();
  }
}
template void RunGraphicalMode<Algorithm::AStar>();
template void RunGraphicalMode<Algorithm::Bfs>();
template void RunGraphicalMode<Algorithm::Dijkstra>();

template <Algorithm alg>
void RunTerminalMode() {
  std::string command;
  int sNode = -1, eNode = -1;

  do {
    std::cout << "\n------------------------------------------" << std::endl;
    std::cout << "Comandos: 'buscar', 'info', 'salir'" << std::endl;
    std::cout << "Ingrese comando: ";
    std::cin >> command;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (command == "buscar") {
      sNode = GetValidNodeIdFromUser("Ingrese ID del nodo de inicio: ",
                                     graph.getNumNodes() - 1);
      if (sNode == -1)
        continue;

      eNode = GetValidNodeIdFromUser("Ingrese ID del nodo final: ",
                                     graph.getNumNodes() - 1);
      if (eNode == -1)
        continue;

      if (sNode == eNode) {
        std::cout
            << "El nodo de inicio y fin son el mismo. No hay ruta que buscar."
            << std::endl;
        continue;
      }

      std::cout << "Buscando ruta de " << sNode << " a " << eNode << "..."
                << std::endl;
      auto startTime = std::chrono::high_resolution_clock::now();
      List<int> terminalPath = Pathfinding::findPath<alg>(graph, sNode, eNode);
      auto endTime = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double> elapsed = endTime - startTime;

      PrintPathToTerminal(terminalPath);
      std::cout << "Tiempo de busqueda: " << elapsed.count() << " segundos."
                << std::endl;

    } else if (command == "info") {
      std::cout << "Nodos totales en el grafo: " << graph.getNumNodes()
                << std::endl;

      std::cout << "\n--- Obstaculos Actuales ---" << std::endl;
      if (graph.getObstacles().empty()) {
        std::cout << "No hay obstaculos definidos." << std::endl;
      } else {
        int obsCount = 0;
        for (const auto& obs : graph.getObstacles()) {
          std::cout << "  Obstaculo #" << obsCount++ << " (" << obs.name
                    << "): "
                    << "Pos=(" << obs.rect.x << ", " << obs.rect.y << "), "
                    << "Tam=(" << obs.rect.width << ", " << obs.rect.height
                    << ")" << std::endl;
        }
      }
      std::cout << "----------------------------" << std::endl;
    } else if (command != "salir") {
      std::cout << "Comando desconocido. Intente 'buscar', 'info', o 'salir'."
                << std::endl;
    }

  } while (command != "salir");
  std::cout << "Saliendo del modo terminal." << std::endl;
}

template void RunTerminalMode<Algorithm::AStar>();
template void RunTerminalMode<Algorithm::Bfs>();
template void RunTerminalMode<Algorithm::Dijkstra>();

void CleanupApplication() {
  if (graphicalMode)
    CloseWindow();

  graph.clear();
  std::cout << "Recursos liberados. Adios!" << std::endl;
}
