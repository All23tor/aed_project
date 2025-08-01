#include "Pathfinding.hpp"
#include "MyVector.hpp"
#include "SimpleMap.hpp"
#include "Utils.hpp"
#include "pair.hpp"
#include "queue.hpp"

namespace {
float calculateHeuristic(const Graph& graph, int nodeId1, int nodeId2) {
  const Node& n1 = graph.getNode(nodeId1);
  const Node& n2 = graph.getNode(nodeId2);
  return Vector2Distance(n1.position, n2.position);
}

SimpleList<int> reconstructPath(const Graph& graph,
                                SimpleMap<int, int> cameFrom, int currentId) {
  auto it = cameFrom.find(currentId);
  if (!it) {
    std::cout << "No se puede reconstruir el camino desde nodo " << currentId
              << ".\n";
    return {};
  }

  SimpleList<int> totalPath;
  while (true) {
    totalPath.push_front(currentId);
    auto it = cameFrom.find(currentId);
    if (!it || *it == -1)
      break;
    currentId = *it;
  }

  return totalPath;
}
} // namespace

SimpleList<int> Pathfinding::findPath(const Graph& graph, int startNodeId,
                                      int endNodeId) {
  int numNodes = graph.getNumNodes();
  if (startNodeId < 0 || startNodeId >= numNodes || endNodeId < 0 ||
      endNodeId >= numNodes) {
    return {};
  }

  // Reiniciar estructuras internas
  MyVector<float> gScore(numNodes, std::numeric_limits<float>::infinity());
  MyVector<float> fScore(numNodes, std::numeric_limits<float>::infinity());
  MyVector<bool> closedSet(numNodes, false);
  SimpleMap<int, int> cameFrom;
  PriorityQueue<Pair<float, int>> openSet;

  gScore[startNodeId] = 0;
  fScore[startNodeId] = calculateHeuristic(graph, startNodeId, endNodeId);
  openSet.push({fScore[startNodeId], startNodeId});

  while (!openSet.empty()) {
    int currentId = openSet.top().second;
    openSet.pop();

    if (currentId == endNodeId) {
      return reconstructPath(graph, cameFrom, currentId);
    }

    if (closedSet[currentId]) {
      continue;
    }

    closedSet[currentId] = true;

    const raylib::Vector2 currentPos = graph.getNode(currentId).position;
    const auto& neighbors = graph.getAdjacentNodes(currentId);

    for (const auto& neighborPair : neighbors) {
      int neighborId = neighborPair.first;
      float edgeCost = neighborPair.second;

      if (closedSet[neighborId]) {
        continue;
      }

      const raylib::Vector2 neighborPos = graph.getNode(neighborId).position;
      bool isBlocked = false;
      for (const auto& obstacle : graph.getObstacles()) {
        if (Utils::CheckLineRectangleCollision(currentPos, neighborPos,
                                               obstacle.rect)) {
          isBlocked = true;
          break;
        }
      }
      if (isBlocked)
        continue;

      float tentative_gScore = gScore[currentId] + edgeCost;

      if (tentative_gScore < gScore[neighborId]) {
        cameFrom.insert(neighborId, currentId);
        gScore[neighborId] = tentative_gScore;
        fScore[neighborId] =
            tentative_gScore + calculateHeuristic(graph, neighborId, endNodeId);
        openSet.push({fScore[neighborId], neighborId});
      }
    }
  }

  return {}; // Camino no encontrado
}
