#pragma once
#include "Estructuras_datos/list.h"
#include "Graph.h"

class IPathfindingStrategy {
public:
    virtual SimpleList<int> findPath(const Graph& graph, int startNodeId, int endNodeId) = 0;
    virtual ~IPathfindingStrategy() = default;
};