#include "App.hpp"
#include "Pathfinding.hpp"
#include <algorithm>
#include <cstring>
#include <string>

namespace {
static constexpr int SCREEN_WIDTH = 1200;
static constexpr int SCREEN_HEIGHT = 800;

std::list<int> path;
int startNodeId = -1;
int endNodeId = -1;

bool editingObstacles = false;
raylib::Vector2 startDragPos;
bool isDragging = false;
char obstacleNameInput[64] = "";
bool typingObstacleName = false;
Rectangle nameInputBox = {10, (float)SCREEN_HEIGHT - 40, 200, 30};

void HandleGraphicalInput(Graph& graph, raylib::Camera2D& camera) {
  if (IsKeyPressed(KEY_O)) {
    editingObstacles = !editingObstacles;
    if (editingObstacles) {
      startNodeId = -1;
      endNodeId = -1;
      path.clear();
    } else {
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

      if (IsKeyPressed(KEY_BACKSPACE) && strlen(obstacleNameInput) > 0)
        obstacleNameInput[strlen(obstacleNameInput) - 1] = '\0';

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
              Obstacle(finalRect, randomColor, std::string(obstacleNameInput)));
        }
        typingObstacleName = false;
        obstacleNameInput[0] = '\0';

        if (startNodeId != -1 && endNodeId != -1) {
          path = Pathfinding::findPath(graph, startNodeId, endNodeId);
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
          graph.removeObstacle(i);

          if (startNodeId != -1 && endNodeId != -1) {
            path = Pathfinding::findPath(graph, startNodeId, endNodeId);
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
        if (startNodeId == -1 || endNodeId != -1 ||
            clickedNodeId == startNodeId) {
          startNodeId = clickedNodeId;
          endNodeId = -1;
          path.clear();
        } else {
          endNodeId = clickedNodeId;
          path = Pathfinding::findPath(graph, startNodeId, endNodeId);
        }
      }
    }
  }

  if (IsKeyPressed(KEY_R))
    if (startNodeId != -1 && endNodeId != -1)
      path = Pathfinding::findPath(graph, startNodeId, endNodeId);

  if (IsKeyPressed(KEY_C)) {
    startNodeId = -1;
    endNodeId = -1;
    path.clear();
  }
}

void DrawGraphElements(const Graph& graph, const std::list<int>& pathNodes,
                       int startNodeId, int endNodeId) {
  const auto& nodes = graph.getNodes();
  for (const auto& node : nodes) {
    for (const auto& otherId : node.adj) {
      const Node& other = nodes[otherId];
      DrawLineV(node.position, other.position, GRAY);
    }
  }

  for (int i = 0; i < graph.getNodes().size(); ++i) {
    const Node& currentNode = graph.getNodes()[i];
    Color nodeColor = BLUE;
    if (i == startNodeId)
      nodeColor = GREEN;
    else if (i == endNodeId)
      nodeColor = RED;

    DrawCircleV(currentNode.position, 5, nodeColor);
  }

  if (!pathNodes.empty()) {
    auto lastId = pathNodes.front();
    for (auto nodeId : pathNodes) {
      const Node& node = nodes[nodeId];
      const Node& lastNode = nodes[lastId];

      DrawLineEx(lastNode.position, node.position, 4, LIME);
      DrawCircleV(node.position, 4, LIME);
      lastId = nodeId;
    }
  }

  if (startNodeId >= 0 && startNodeId < nodes.size())
    DrawCircleV(nodes[startNodeId].position, 8, GREEN);
  if (endNodeId >= 0 && endNodeId < nodes.size())
    DrawCircleV(nodes[endNodeId].position, 8, RED);
}

void DrawGraphicalHUD(const Graph& graph) {
  DrawText(TextFormat("Nodos: %i", graph.getNodes().size()), 10, 10, 20,
           DARKGRAY);

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

void UpdateApplication(Graph& graph, raylib::Camera2D& camera) {
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

  HandleGraphicalInput(graph, camera);
}

void DrawApplication(const Graph& graph, const raylib::Camera2D& camera) {
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

  DrawGraphicalHUD(graph);

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

auto InitializeApplication(int numNodes) {
  static constexpr float NODE_GENERATION_AREA_PADDING = 50.0f;
  static constexpr int NUM_OBSTACLES = 5;

  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "RayPathFinder - Modo Grafico");
  SetTargetFPS(60);

  raylib::Camera2D camera = {
      {(float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2},
      {(float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2},
      0.0f,
      1.0f,
  };

  Graph graph = Graph::generateRandom(numNodes, NUM_OBSTACLES, SCREEN_WIDTH,
                                      SCREEN_HEIGHT);

  return std::tuple{graph, camera};
}

void CleanupApplication() {
  CloseWindow();
}
} // namespace

void RunGraphicalMode(int numNodes) {
  auto [graph, camera] = InitializeApplication(numNodes);
  while (!WindowShouldClose()) {
    UpdateApplication(graph, camera);
    DrawApplication(graph, camera);
  }
  CleanupApplication();
}
