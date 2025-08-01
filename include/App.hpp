#pragma once

static constexpr int GRAPHICAL_NODE_LIMIT = 700;
inline bool graphicalMode = true;

void InitializeApplication(int numNodes);
void RunGraphicalMode();
void RunTerminalMode();
void CleanupApplication();
