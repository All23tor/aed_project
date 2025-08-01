#pragma once
#include "Pair.hpp"
#include "Vector.hpp"
#include "raylib-cpp.hpp"

struct Node {
  int id;
  raylib::Vector2 position;

  Node(int id, float x, float y) : id(id), position(raylib::Vector2{x, y}) {}
  Node() : id(-1), position(raylib::Vector2(0.0f, 0.0f)) {}
};

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

// Clase que representa el grafo
class Graph {
public:
  Graph();
  ~Graph();
  void addNode(int id, float x, float y);
  bool addEdge(int sourceId, int targetId, float weight);
  const Node& getNode(int id) const;
  const Vector<Pair<int, float>>& getAdjacentNodes(int id) const;
  int getNumNodes() const;
  int findNodeAtPosition(const raylib::Vector2& clickPos, float radius) const;
  void generateRandomNodes(int count, int maxWidth, int maxHeight,
                           int maxEdgesPerNode, float maxConnectionDistance);

  void generateRandomObstacles(int count, int maxWidth, int maxHeight);
  const Vector<Obstacle>& getObstacles() const; // salida de los obstaculos

  void clear();
  void addObstacle(const Obstacle& obs);
  void removeObstacle(int index);
  bool areNodesWithinObstacle(int nodeId1, int nodeId2) const;
  size_t getEstimatedMemoryUsage() const;

private:
  Vector<Node> nodes;
  Vector<Vector<Pair<int, float>>> adjacencyList;
  bool isValidNodeId(int id) const;
  Vector<Obstacle> obstacles;

  Vector<Vector<Vector<int>>> spatialGrid;
  int gridCols;
  int gridRows;

  static const int CellSize = 200; // ajustar valor si quieren

  // Metodos axucialres de la cuadricula
  int getCellX(float x) const {
    return static_cast<int>(x / CellSize);
  }
  int getCellY(float y) const {
    return static_cast<int>(y / CellSize);
  }
  bool isValidGridCell(int col, int row) const {
    return col >= 0 && col < gridCols && row >= 0 && row < gridRows;
  }
};
