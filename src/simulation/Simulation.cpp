// === File: src/Simulation.cpp ===
#include "Simulation.h"
#include <cstdlib>
#include "Grid.h"
#include <algorithm>
#include <iostream>
#include <random>

std::random_device generator;

// Simple stub: random walk
double simulateStep(double dt, AgentList& agents) {
    SpatialGrid grid(0.05f);  // Cell size ≈ search radius
    grid.clear();

    // Insert all agents into the grid
    for (const auto& a : agents) {
        grid.insert(&a);
    }

    AgentList newAgents;
    std::vector<size_t> agentsToRemove;

    bool hasReproduced;
    for (size_t i = 0; i < agents.size(); ++i) {
        Agent& a = agents[i];
        hasReproduced = false;
        // Random walk, weighted with energy, if there is any
        if (a.energy > 0.0)
        {
            a.x += (static_cast<float>(generator()) / generator.max() * 2 - 1) * a.energy / 10;
            a.y += (static_cast<float>(generator()) / generator.max() * 2 - 1) * a.energy / 10;
            a.energy -= .2f;
        }

        // Check if agent dies
        if (a.energy <= 0.0) {
            agentsToRemove.push_back(i);
            continue;
        }
        // Query nearby agents
        auto nearby = grid.queryNearby(a.x, a.y);

        bool hasNeighbor = false;
        float radius2 = 0.05f * 0.05f;

        for (const Agent* other : nearby) {
            if (&a == other) continue;

            float dx = a.x - other->x;
            float dy = a.y - other->y;
            float dist2 = dx * dx + dy * dy;
            if (dist2 > 0.05f * 0.05f) continue;
            // Check if agent dies
            if (a.energy <= 0.0) {
                agentsToRemove.push_back(i);
                break;
            }
            // Interaction logic
            if (a.type == Agent::Carnivore && other->type == Agent::Vegetarian) {
                // Eat vegetarian
                auto it = std::find_if(agents.begin(), agents.end(), [other](const Agent& test) {
                    return &test == other;
                    });
                if (it != agents.end()) {
                    agentsToRemove.push_back(std::distance(agents.begin(), it));
                    a.energy += .25f;
                }
            }

            if (a.type == Agent::Carnivore && hasReproduced)
                continue;

            if (a.type == other->type && a.energy > 0.5f) {
                // Reproduce
                Agent baby;
                baby.type = a.type;
                baby.x = a.x + ((rand() / (float)RAND_MAX) - 0.5f) * 0.02f;
                baby.y = a.y + ((rand() / (float)RAND_MAX) - 0.5f) * 0.02f;
                baby.energy = 0.75f;
                newAgents.push_back(baby);
                if (a.type == Agent::Carnivore)
                {
                    a.energy -= .5f;
                    hasReproduced = true;
                }
                else {
                    a.energy /= 1.5;
                }
            }
        }
    }

    std::sort(agentsToRemove.begin(), agentsToRemove.end());
    agentsToRemove.erase(std::unique(agentsToRemove.begin(), agentsToRemove.end()),
        agentsToRemove.end());
    std::sort(agentsToRemove.rbegin(), agentsToRemove.rend());
    
    // Remove eaten vegetarians
    for (size_t idx : agentsToRemove) {
        agents.erase(agents.begin() + idx);
    }

    // Add new agents
    agents.insert(agents.end(), newAgents.begin(), newAgents.end());

    return dt;
}