#include "App.hpp"
#include <iostream>

int main() {
  int numNodes;
  std::cout << "------------------------------------------" << std::endl;
  std::cout << "        ALGORITMO DE BUSQUEDA A* " << std::endl;
  std::cout << "------------------------------------------" << std::endl;
  std::cout << "Ingrese la cantidad de nodos: ";
  std::cin >> numNodes;

  if (numNodes <= 0) {
    std::cerr << "Error: Entrada invalida. Por favor, ingrese un numero entero "
                 "positivo."
              << std::endl;
    return 1;
  }

  RunGraphicalMode(numNodes);
}
