#include "Graph.hpp"
#include "raylib.h"

const std::vector<Node>& Graph::getNodes() const {
  return nodes;
}

std::size_t Graph::findNodeAtPosition(const raylib::Vector2& clickPos,
                                      float radius) const {
  for (auto id = 0uz; id < nodes.size(); id++)
    if (clickPos.Distance(nodes[id].position) <= radius)
      return id;
  return -1;
}

Graph Graph::generateRandom(std::size_t numNodes, std::size_t numObstacles,
                            int width, int height) {
  Graph graph;
  graph.nodes.reserve(numNodes);

  for (int i = 0; i < numNodes; ++i) {
    float x = GetRandomValue(0, width);
    float y = GetRandomValue(0, height);
    graph.nodes.emplace_back(x, y);
  }

  auto max_dist = width * width + height * height;
  for (auto src = 0uz; src < numNodes; src++)
    while (true) {
      std::size_t tar = GetRandomValue(0, numNodes - 1);
      Node& srcNode = graph.nodes[src];
      Node& tarNode = graph.nodes[tar];
      auto dist = srcNode.position.DistanceSqr(tarNode.position);
      if (GetRandomValue(0, max_dist) > dist)
        continue;

      srcNode.adj.insert(tar);
      tarNode.adj.insert(src);
      if (GetRandomValue(0, 5) == 5)
        break;
    }

  static constexpr std::string_view obstacleNames[] = {
      "Rio",      "Pared",          "Lago",        "Montana",
      "Barranco", "Zona Prohibida", "Bosque Denso"};

  static constexpr Color obstacleColors[] = {BLUE,   DARKGREEN, BROWN, GRAY,
                                             MAROON, VIOLET,    LIME};
  graph.obstacles.reserve(numObstacles);
  for (int i = 0; i < numObstacles; ++i) {
    int minSize = 30;
    int maxSize = 150;
    float rec_width = GetRandomValue(minSize, maxSize);
    float rec_height = GetRandomValue(minSize, maxSize);

    float x = GetRandomValue(0, width - rec_width);
    float y = GetRandomValue(0, height - rec_height);

    Rectangle rect{x, y, rec_width, rec_height};
    std::string name{
        obstacleNames[GetRandomValue(0, std::size(obstacleNames) - 1)]};
    Color color =
        obstacleColors[GetRandomValue(0, std::size(obstacleColors) - 1)];
    graph.obstacles.emplace_back(rect, color, name);
  }

  return graph;
}

const std::vector<Obstacle>& Graph::getObstacles() const {
  return obstacles;
}

void Graph::addObstacle(const Obstacle& obs) {
  obstacles.push_back(obs);
}

void Graph::removeObstacle(std::size_t index) {
  if (index >= 0 && index < obstacles.size()) {
    obstacles.erase(obstacles.begin() + index);
  }
}

void Graph::clear() {
  nodes.clear();
  obstacles.clear();
}
