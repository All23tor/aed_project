#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "raylib-cpp.hpp"
#include <set>
#include <vector>

struct Node {
  raylib::Vector2 position;
  std::set<int> adj;

  Node(float x, float y) : position(raylib::Vector2{x, y}) {}
};

struct Obstacle {
  raylib::Rectangle rect;
  raylib::Color color;
  std::string name;

  Obstacle(raylib::Rectangle r, raylib::Color c, const std::string& n) :
      rect(r),
      name(n),
      color(c) {}
};

class Graph {
  std::vector<Node> nodes;
  std::vector<Obstacle> obstacles;

public:
  std::size_t findNodeAtPosition(const raylib::Vector2& clickPos,
                                 float radius) const;
  static Graph generateRandom(std::size_t numNodes, std::size_t numObstacles,
                              int maxWidth, int maxHeight);
  const std::vector<Obstacle>& getObstacles() const; // salida de los obstaculos
  const std::vector<Node>& getNodes() const;

  void clear();
  void addObstacle(const Obstacle& obs);
  void removeObstacle(std::size_t index);
};

#endif
