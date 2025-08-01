#include "Pathfinding.h"

Pathfinding::Pathfinding(const Graph& g, std::unique_ptr<IPathfindingStrategy> strategy)
    : graph(g), currentStrategy(std::move(strategy)) {}

SimpleList<int> Pathfinding::findPath(int startNodeId, int endNodeId) {
    if (currentStrategy) {
        return currentStrategy->findPath(graph, startNodeId, endNodeId);
    }
    return SimpleList<int>();
}

void Pathfinding::setStrategy(std::unique_ptr<IPathfindingStrategy> newStrategy) {
    currentStrategy = std::move(newStrategy);
}

