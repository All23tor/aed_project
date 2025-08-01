cmake_minimum_required(VERSION 3.10)
project(RayPathFinderApp CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Crear una lista de TODOS los archivos .cpp.
set(PROJECT_SOURCES
    src/RayPathApp/main.cpp
    src/RayPathApp/Application.cpp
    src/RayPathApp/Graph.cpp
    src/RayPathApp/Pathfinding.cpp
    src/RayPathApp/Utils.cpp
    src/RayPathApp/DijkstraStrategy.cpp
    src/RayPathApp/AStarStrategy.cpp
)

# 2. Crear el ejecutable.
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

# 3. Directorios donde buscar archivos de cabecera (.h).
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/src/cover/ventor/raylib-cpp/include
    ${CMAKE_SOURCE_DIR}/src/cover
    ${CMAKE_SOURCE_DIR}/src/cover/Estructuras_datos
)

# 4. Enlazar la librería base raylib a tu ejecutable.
find_package(raylib CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)