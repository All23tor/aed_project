#include "App.hpp"
#include "Pathfinding.hpp"
#include <iostream>

int main() {
  int numNodes;
  std::cout << "------------------------------------------" << std::endl;
  std::cout << "        ALGORITMO DE BUSQUEDA A* " << std::endl;
  std::cout << "------------------------------------------" << std::endl;
  std::cout << "Ingrese la cantidad de nodos (se recomienda <= "
            << GRAPHICAL_NODE_LIMIT << " para interfaz grafica): ";
  std::cin >> numNodes;

  if (std::cin.fail() || numNodes <= 0) {
    std::cerr << "Error: Entrada invalida. Por favor, ingrese un numero entero "
                 "positivo."
              << std::endl;
    return 1;
  }
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

  int algorithm = 0;
  std::cout << "Elegir algoritmo: A* (0), BFS(1) Dijkstra(2): ";
  std::cin >> algorithm;
  std::cout << "Se ha elegido el algoritmo: "
            << (algorithm == 1   ? "BFS"
                : algorithm == 2 ? "Dijkstra"
                                 : "A*")
            << '\n';

  InitializeApplication(numNodes);
  switch (algorithm) {
  case 1:
    if (graphicalMode)
      RunGraphicalMode<Algorithm::Bfs>();
    else
      RunTerminalMode<Algorithm::Bfs>();
    break;
  case 2:
    if (graphicalMode)
      RunGraphicalMode<Algorithm::Dijkstra>();
    else
      RunTerminalMode<Algorithm::Dijkstra>();
    break;
  default:
    if (graphicalMode)
      RunGraphicalMode<Algorithm::AStar>();
    else
      RunTerminalMode<Algorithm::AStar>();
  }
  CleanupApplication();
}
