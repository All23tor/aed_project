#include "DijkstraStrategy.h"
#include <queue>
#include <limits>
#include "Estructuras_datos/MyVector.h"

SimpleList<int> DijkstraStrategy::findPath(const Graph& graph, int startNodeId, int endNodeId) {
    if (startNodeId == -1 || endNodeId == -1 || startNodeId >= graph.getNumNodes() || endNodeId >= graph.getNumNodes()) {
        return SimpleList<int>();
    }

    MyVector<float> distances(graph.getNumNodes(), std::numeric_limits<float>::max());
    SimpleMap<int, int> cameFrom;
    
    distances[startNodeId] = 0;
    
    using NodePair = std::pair<float, int>;
    std::priority_queue<NodePair, std::vector<NodePair>, std::greater<NodePair>> pq;
    pq.push({0, startNodeId});

    while (!pq.empty()) {
        float currentDistance = pq.top().first;
        int currentNodeId = pq.top().second;
        pq.pop();

        if (currentNodeId == endNodeId) {
            return reconstructPath(endNodeId, cameFrom);
        }

        if (currentDistance > distances[currentNodeId]) {
            continue;
        }

        for (const auto& edge : graph.getAdjacentNodes(currentNodeId)) {
            int neighborId = edge.first;
            float edgeWeight = edge.second;
            float newDistance = currentDistance + edgeWeight;
            
            if (newDistance < distances[neighborId]) {
                distances[neighborId] = newDistance;
                cameFrom.insert(neighborId, currentNodeId);
                pq.push({newDistance, neighborId});
            }
        }
    }

    return SimpleList<int>(); // No se encontró el camino
}

SimpleList<int> DijkstraStrategy::reconstructPath(int currentId, const SimpleMap<int, int>& cameFrom) const {
    SimpleList<int> path;
    while (cameFrom.contains(currentId)) {
        path.push_front(currentId);
        currentId = cameFrom.get(currentId);
    }
    path.push_front(currentId); // Agrega el nodo de inicio
    return path;
}