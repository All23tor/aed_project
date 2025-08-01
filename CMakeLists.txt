cmake_minimum_required(VERSION 3.11)
project(RayPathFinderApp)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(RAYLIB_VERSION 5.5)
find_package(raylib ${RAYLIB_VERSION} QUIET)
if (NOT raylib_FOUND)
  include(FetchContent)
  FetchContent_Declare(
    raylib
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
  )
  FetchContent_GetProperties(raylib)
  if (NOT raylib_POPULATED)
    set(FETCHCONTENT_QUIET NO)
    FetchContent_MakeAvailable(raylib)
  endif()
endif()

set(PROJECT_SOURCES
  src/Application.cpp
  src/Graph.cpp
  src/Pathfinding.cpp
  src/Utils.cpp
  src/DijkstraStrategy.cpp
  src/AStarStrategy.cpp
  main.cpp
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
target_link_libraries(${PROJECT_NAME} raylib raylib_cpp)
target_include_directories(${PROJECT_NAME} PUBLIC
  include/
  data_structres/
)
