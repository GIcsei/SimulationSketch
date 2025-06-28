// === File: src/Grid.h ===
#pragma once
#include "Agent.h"
#include <unordered_map>
#include <vector>
#include <cmath>

struct GridCoord {
    int x, y;

    bool operator==(const GridCoord& other) const {
        return x == other.x && y == other.y;
    }
};

namespace std {
    template<> struct hash<GridCoord> {
        std::size_t operator()(const GridCoord& c) const {
            return std::hash<int>()(c.x) ^ (std::hash<int>()(c.y) << 1);
        }
    };
}

class SpatialGrid {
public:
    explicit SpatialGrid(float cellSize) : cellSize(cellSize) {}

    void clear() {
        cells.clear();
    }

    void insert(const Agent* agent) {
        GridCoord coord = toCell(agent->x, agent->y);
        cells[coord].push_back(agent);
    }

    std::vector<const Agent*> queryNearby(float x, float y) const {
        std::vector<const Agent*> nearby;
        GridCoord center = toCell(x, y);

        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                GridCoord neighbor = { center.x + dx, center.y + dy };
                auto it = cells.find(neighbor);
                if (it != cells.end()) {
                    nearby.insert(nearby.end(), it->second.begin(), it->second.end());
                }
            }
        }

        return nearby;
    }

private:
    float cellSize;
    std::unordered_map<GridCoord, std::vector<const Agent*>> cells;

    GridCoord toCell(float x, float y) const {
        return { static_cast<int>(std::floor(x / cellSize)),
                 static_cast<int>(std::floor(y / cellSize)) };
    }
};
