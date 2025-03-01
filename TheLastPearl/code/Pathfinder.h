// Name: Conor Ryan
// K-NUM: K00286377

#pragma once

#include <vector>
#include <SFML/System.hpp>
#include "Map.h"

// Pathfinder class finds a route through the grid from an entry point to the target point
//  by using the A* algorithm to return a vector of positions on the grid.
class Pathfinder {
public:
    // Returns a vector of grid positions which represent a path the enemies can take.
    std::vector<sf::Vector2i> findPath(const Map& map, const sf::Vector2i& start, const sf::Vector2i& target);
};
