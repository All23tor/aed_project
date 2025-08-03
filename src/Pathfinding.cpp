#include "Pathfinding.hpp"
#include "Utils.hpp"
#include <map>
#include <queue>

namespace {
std::list<int> reconstructPath(const Graph& graph,
                               const std::map<int, int>& cameFrom,
                               int currentId) {
  std::list<int> path;
  while (cameFrom.find(currentId) != cameFrom.end()) {
    path.push_front(currentId);
    currentId = cameFrom.at(currentId);
  }
  path.push_front(currentId);
  return path;
}

bool inObstacle(raylib::Vector2 p1, raylib::Vector2 p2,
                const std::vector<Obstacle>& obstacles) {
  for (const auto& obstacle : obstacles)
    if (Utils::CheckLineRectangleCollision(p1, p2, obstacle.rect))
      return true;
  return false;
}
} // namespace

std::list<int> Pathfinding::findPath(const Graph& graph, int startNodeId,
                                     int endNodeId) {
  const auto& nodes = graph.getNodes();

  std::vector<float> dist(nodes.size(), std::numeric_limits<float>::infinity());
  std::map<int, int> cameFrom;
  std::priority_queue<std::pair<float, int>> pq;

  dist[startNodeId] = 0;
  pq.push({0, startNodeId});

  while (!pq.empty()) {
    float d = pq.top().first;
    int currentId = pq.top().second;
    pq.pop();

    if (currentId == endNodeId)
      return reconstructPath(graph, cameFrom, currentId);

    if (d > dist[currentId]) {
      continue;
    }

    const auto& node = nodes[currentId];
    for (const auto& adjId : node.adj) {
      const auto& adjNode = nodes[adjId];

      raylib::Vector2 pos = node.position;
      raylib::Vector2 adjPos = adjNode.position;
      if (inObstacle(pos, adjPos, graph.getObstacles()))
        continue;

      float newDist = dist[currentId] + pos.Distance(adjPos);
      if (newDist < dist[adjId]) {
        dist[adjId] = newDist;
        cameFrom[adjId] = currentId;
        pq.push({-newDist, adjId});
      }
    }
  }

  return {}; // No se encontró camino
}
