#pragma once

#include "Graph.hpp"
#include "List.hpp"

namespace Pathfinding {
List<int> findPath(const Graph&, int startNodeId, int endNodeId);
}
