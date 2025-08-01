#include "AStarStrategy.h"

SimpleList<int> AStarStrategy::findPath(const Graph& graph, int startNodeId, int endNodeId) {
    if (startNodeId == -1 || endNodeId == -1 || startNodeId >= graph.getNumNodes() || endNodeId >= graph.getNumNodes()) {
        return SimpleList<int>();
    }

    gScore.assign(graph.getNumNodes(), std::numeric_limits<float>::infinity());
    fScore.assign(graph.getNumNodes(), std::numeric_limits<float>::infinity());
    cameFrom.clear();

    gScore[startNodeId] = 0;
    fScore[startNodeId] = calculateHeuristic(startNodeId, endNodeId, graph);
    
    // Usamos una cola de prioridad para manejar los nodos abiertos
    using NodePair = std::pair<float, int>;
    std::priority_queue<NodePair, std::vector<NodePair>, std::greater<NodePair>> openSet;
    openSet.push({fScore[startNodeId], startNodeId});

    while (!openSet.empty()) {
        int currentNodeId = openSet.top().second;
        openSet.pop();

        if (currentNodeId == endNodeId) {
            return reconstructPath(endNodeId);
        }

        for (const auto& edge : graph.getAdjacentNodes(currentNodeId)) {
            int neighborId = edge.first;
            float edgeWeight = edge.second;
            float tentative_gScore = gScore[currentNodeId] + edgeWeight;

            if (tentative_gScore < gScore[neighborId]) {
                cameFrom.insert(neighborId, currentNodeId);
                gScore[neighborId] = tentative_gScore;
                fScore[neighborId] = gScore[neighborId] + calculateHeuristic(neighborId, endNodeId, graph);
                openSet.push({fScore[neighborId], neighborId});
            }
        }
    }

    return SimpleList<int>(); // No se encontró el camino
}

float AStarStrategy::calculateHeuristic(int nodeId1, int nodeId2, const Graph& graph) const {
    const Node& node1 = graph.getNode(nodeId1);
    const Node& node2 = graph.getNode(nodeId2);
    float dx = node1.position.x - node2.position.x;
    float dy = node1.position.y - node2.position.y;
    return std::sqrt(dx * dx + dy * dy);
}

SimpleList<int> AStarStrategy::reconstructPath(int currentId) const {
    SimpleList<int> path;
    while (cameFrom.contains(currentId)) {
        path.push_front(currentId);
        currentId = cameFrom.get(currentId);
    }
    path.push_front(currentId);
    return path;
}