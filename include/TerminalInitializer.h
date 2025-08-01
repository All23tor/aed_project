#pragma once

#include "IInitializer.h"
#include "Constants.h" 
#include <iostream>



class TerminalInitializer : public IInitializer {
public:
    explicit TerminalInitializer(int numNodes) : numNodes(numNodes) {}

    void Initialize() override {
        std::cout << "\n------------------------------------------" << std::endl;
        std::cout << "MODO TERMINAL ACTIVADO (Nodos: " << numNodes << ")" << std::endl;
        std::cout << "------------------------------------------" << std::endl;
        std::cout << "El dibujo del grafo completo esta deshabilitado para mas de " << GRAPHICAL_NODE_LIMIT << " nodos." << std::endl;
    }

private:
    int numNodes;
};