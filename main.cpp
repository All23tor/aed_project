#include "App.hpp"
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

  InitializeApplication(numNodes);
  if (graphicalMode)
    RunGraphicalMode();
  else
    RunTerminalMode();
  CleanupApplication();
}
