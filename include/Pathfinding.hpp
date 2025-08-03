#pragma once

#include "Graph.hpp"
#include <list>

namespace Pathfinding {
std::list<int> findPath(const Graph&, int startNodeId, int endNodeId);
}
