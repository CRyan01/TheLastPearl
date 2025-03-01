// Name: Conor Ryan
// K-NUM: K00286377

#include "Pathfinder.h"
#include <queue>
#include <cmath>
#include <algorithm>

// Create the node structure needed for A*.
struct Node {
    sf::Vector2i gridPos;  // The grid position of the node.
    int pathCost;          // The cost from the starting node.
    int estimatedTotalCost; // The inital cost added to the cost estimated by the heurisitc function.

    // Overload the > operator to prioritize nodes with a lower value.
    bool operator>(const Node& other) const {
        return estimatedTotalCost > other.estimatedTotalCost;
    }
};

// Manhattan distance heuristic function to estimate the cost to the target position.
static int heuristic(const sf::Vector2i& a, const sf::Vector2i& b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

// Finds a path from the 'start' positon to 'target' position using the A* algorithm. Returns a vector of positions.
std::vector<sf::Vector2i> Pathfinder::findPath(const Map& map, const sf::Vector2i& start, const sf::Vector2i& target) {
    std::vector<sf::Vector2i> path; // Declare a vector of positions to store the final path.
    int width = map.getWidth(); // Get the maps width and store it.
    int height = map.getHeight(); // Get the maps height and store it.

    // A 2D vector to tracks already visited nodes.
    std::vector<std::vector<bool>> visited(height, std::vector<bool>(width, false));

    // A 2D vector to store the last node for each grid position. To reconstruct paths.
    std::vector<std::vector<sf::Vector2i>> cameFrom(height, std::vector<sf::Vector2i>(width, { -1, -1 }));

    // A priority queue for searching
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> uncheckedNodes;

    uncheckedNodes.push({ start, 0, heuristic(start, target) });

    // Process all unchecked nodes.
    while (!uncheckedNodes.empty()) {
        Node currentNode = uncheckedNodes.top(); // Get the node with the lowest estimated total cost (the top one).
        uncheckedNodes.pop(); // Pop the node so it can be worked with.

        // Break for the loop if the target is found, to avoid processing uneccessary nodes.
        if (currentNode.gridPos == target) {
            break;
        }

        int x = currentNode.gridPos.x;
        int y = currentNode.gridPos.y;
        
        // Check if this node was already processed. If it was break early and move to the next node.
        if (visited[y][x]) {
            continue; // If not continue.
        }

        // Mark the current node as visted.
        visited[y][x] = true;

        // Define four directions of movement, right, left, down, up.
        const sf::Vector2i directions[4] = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };

        // Check every possible direction.
        for (const auto& direction : directions) {
            sf::Vector2i possiblePosition = currentNode.gridPos + direction; // Store the potential new position.

            // Check if the position is within bounds.
            if (possiblePosition.x < 0 || possiblePosition.y < 0 || possiblePosition.x >= width || possiblePosition.y >= height) {
                continue;
            }

            // Skip any tiles marked as ground type tiles.
            if (!map.isWalkable(possiblePosition.x, possiblePosition.y)) {
                continue;
            }
            // Skip any previously vistied nodes.
            if (visited[possiblePosition.y][possiblePosition.x]) {
                continue;
            }

            int newPathCost = currentNode.pathCost + 1; // Increase the movement cost.

            // Add the newly checked node to the set of unchecked nodes.
            uncheckedNodes.push({ possiblePosition, newPathCost, newPathCost + heuristic(possiblePosition, target) });

            // Store the previous position for path reconstruction.
            cameFrom[possiblePosition.y][possiblePosition.x] = currentNode.gridPos;
        }
    }

    // Return an empty path if the target was not reached.
    if (cameFrom[target.y][target.x].x == -1) {
        return {};
    }

    // Reconstruct the path by tracing backwards from the target.
    sf::Vector2i currentNode = target;
    while (currentNode != start) {
        path.push_back(currentNode);
        currentNode = cameFrom[currentNode.y][currentNode.x];
    }

    // Reverse the path so it starts at the intended position.
    std::reverse(path.begin(), path.end());

    return path; // Return the path.
}