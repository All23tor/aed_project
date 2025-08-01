#pragma once
#include "Pathfinding.hpp"

static constexpr int GRAPHICAL_NODE_LIMIT = 700;
inline bool graphicalMode = true;

void InitializeApplication(int numNodes);
template <Algorithm>
void RunGraphicalMode();
template <Algorithm>
void RunTerminalMode();
void CleanupApplication();
