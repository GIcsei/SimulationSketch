// === File: include/Renderer.h ===
#ifndef RENDERER_H
#define RENDERER_H

#include "Agent.h"

void renderAgents(const AgentList& agents);

void initRenderer();                // Call once
void renderAgents(const AgentList&);
void cleanupRenderer();
#endif // RENDERER_H