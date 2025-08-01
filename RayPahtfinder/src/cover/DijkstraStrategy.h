#pragma once
#include "IPathfindingStrategy.h"
#include "Estructuras_datos/MyVector.h"
#include "Estructuras_datos/SimpleMap.h"

class DijkstraStrategy : public IPathfindingStrategy {
public:
    SimpleList<int> findPath(const Graph& graph, int startNodeId, int endNodeId) override;

private:
    SimpleList<int> reconstructPath(int currentId, const SimpleMap<int, int>& cameFrom) const;
};