#pragma once

#include "Graph.hpp"
#include "list.hpp"

namespace Pathfinding {
SimpleList<int> findPath(const Graph&, int startNodeId, int endNodeId);
}
