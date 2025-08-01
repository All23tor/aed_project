#pragma once
#include "Estructuras_datos/MyVector.h"
#include "Estructuras_datos/list.h"
#include "Estructuras_datos/SimpleMap.h"
#include "IPathfindingStrategy.h"
#include "Node.h"
#include "Graph.h" 
#include <utility>
#include <memory>


//adminnistrador de los algoritmos 
class Pathfinding {
public:
   
    Pathfinding(const Graph& g, std::unique_ptr<IPathfindingStrategy> strategy);
    SimpleList<int> findPath(int startNodeId, int endNodeId);
    void setStrategy(std::unique_ptr<IPathfindingStrategy> newStrategy);
    
private:
    const Graph& graph; 
    std::unique_ptr<IPathfindingStrategy> currentStrategy;

};