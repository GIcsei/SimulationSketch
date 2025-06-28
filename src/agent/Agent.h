// === File: include/Agent.h ===
#ifndef AGENT_H
#define AGENT_H

#include <vector>

struct Agent {
    enum Type { Carnivore, Vegetarian } type;
    float x, y;
    float energy;
};

using AgentList = std::vector<Agent>;

#endif // AGENT_H