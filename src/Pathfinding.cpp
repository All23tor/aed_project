#include "Pathfinding.hpp"
#include "Map.hpp"
#include "Queue.hpp"
#include "Utils.hpp"

namespace {
float calculateHeuristic(const Graph& graph, int nodeId1, int nodeId2) {
  const Node& n1 = graph.getNode(nodeId1);
  const Node& n2 = graph.getNode(nodeId2);
  return Vector2Distance(n1.position, n2.position);
}

List<int> reconstructPath(const Graph& graph, Map<int, int> cameFrom,
                          int currentId) {
  auto it = cameFrom.find(currentId);
  if (!it) {
    std::cout << "No se puede reconstruir el camino desde nodo " << currentId
              << ".\n";
    return {};
  }

  List<int> totalPath;
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

template <>
List<int> Pathfinding::findPath<Algorithm::AStar>(const Graph& graph,
                                                  int startNodeId,
                                                  int endNodeId) {
  int numNodes = graph.getNumNodes();
  if (startNodeId < 0 || startNodeId >= numNodes || endNodeId < 0 ||
      endNodeId >= numNodes) {
    return {};
  }

  Vector<float> gScore(numNodes, std::numeric_limits<float>::infinity());
  Vector<float> fScore(numNodes, std::numeric_limits<float>::infinity());
  Vector<bool> closedSet(numNodes, false);
  Map<int, int> cameFrom;
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

template <>
List<int> Pathfinding::findPath<Algorithm::Bfs>(const Graph& graph,
                                                int startNodeId,
                                                int endNodeId) {
  int numNodes = graph.getNumNodes();

  if (startNodeId < 0 || startNodeId >= numNodes || endNodeId < 0 ||
      endNodeId >= numNodes) {
    return {};
  }

  Vector<bool> visited(numNodes, false);
  Map<int, int> cameFrom;
  Queue<int> q;

  q.push(startNodeId);
  visited[startNodeId] = true;

  while (!q.empty()) {
    int currentId = q.front();
    q.pop();

    if (currentId == endNodeId)
      return reconstructPath(graph, cameFrom, currentId);

    const auto& neighbors = graph.getAdjacentNodes(currentId);
    for (const auto& neighborPair : neighbors) {
      int neighborId = neighborPair.first;

      if (!visited[neighborId]) {
        // Verifica colisión con obstáculos (similar a A*)
        const raylib::Vector2 currentPos = graph.getNode(currentId).position;
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

        visited[neighborId] = true;
        cameFrom.insert(neighborId, currentId);
        q.push(neighborId);
      }
    }
  }

  return {}; // Camino no encontrado
}

template <>
List<int> Pathfinding::findPath<Algorithm::Dijkstra>(const Graph& graph,
                                                     int startNodeId,
                                                     int endNodeId) {
  int numNodes = graph.getNumNodes();

  if (startNodeId < 0 || startNodeId >= numNodes || endNodeId < 0 ||
      endNodeId >= numNodes) {
    return {};
  }

  Vector<float> dist(numNodes, std::numeric_limits<float>::infinity());
  Map<int, int> cameFrom;
  PriorityQueue<Pair<float, int>> pq;

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

    const auto& neighbors = graph.getAdjacentNodes(currentId);
    for (const auto& neighborPair : neighbors) {
      int neighborId = neighborPair.first;
      float edgeCost = neighborPair.second;

      // Verifica colisión con obstáculos
      const raylib::Vector2 currentPos = graph.getNode(currentId).position;
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

      float newDist = dist[currentId] + edgeCost;
      if (newDist < dist[neighborId]) {
        dist[neighborId] = newDist;
        cameFrom.insert(neighborId, currentId);
        pq.push({newDist, neighborId});
      }
    }
  }

  return {}; // Camino no encontrado
}
