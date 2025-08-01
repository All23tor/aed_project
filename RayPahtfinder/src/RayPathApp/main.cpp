#include <iostream>
#include <limits>
#include "Application.h"

int main() {
    int numNodes;
    // Lógica para solicitar el número de nodos...
    std::cout << "Ingrese la cantidad de nodos: ";
    std::cin >> numNodes;
    // ... validación de entrada ...

    Application myApp(numNodes); 
    myApp.Run();                
    
    return 0;
}