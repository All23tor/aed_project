#pragma once

#include "Graph.hpp"
#include "List.hpp"

enum class Algorithm {
  AStar,
  Bfs,
  Dijkstra,
};

namespace Pathfinding {
template <Algorithm>
List<int> findPath(const Graph&, int startNodeId, int endNodeId);
}
