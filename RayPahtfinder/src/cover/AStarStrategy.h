#pragma once
#include "IPathfindingStrategy.h"
#include "Estructuras_datos/MyVector.h"
#include "Estructuras_datos/SimpleMap.h"
#include <queue>
#include <limits>
#include <cmath>

class AStarStrategy : public IPathfindingStrategy {
public:
    SimpleList<int> findPath(const Graph& graph, int startNodeId, int endNodeId) override;

private:
    // Variables y métodos específicos de A*
    MyVector<float> gScore;
    MyVector<float> fScore;
    SimpleMap<int, int> cameFrom;
    MyVector<bool> closedSet;

    // La función heurística ahora es privada de esta clase
    float calculateHeuristic(int nodeId1, int nodeId2, const Graph& graph) const;
    SimpleList<int> reconstructPath(int currentId) const;
};