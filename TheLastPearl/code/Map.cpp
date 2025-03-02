// Name: Conor Ryan
// K-NUM: K00286377

#include "Map.h"
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iostream>
#include "Pathfinder.h"

// Constructs a map with the specified width and height in tiles, with specified cluster parameters.
Map::Map(int width, int height, int numClusters, int minClusterSize, int maxClusterSize)
    : width(width), height(height), grid(width* height),
    numClusters(numClusters), minClusterSize(minClusterSize), maxClusterSize(maxClusterSize) {
    // empty on purpose.
}

// Returns true if the tile at (tileX, tileY) is a ground tile.
bool Map::isWalkable(int tileX, int tileY) const {
    if (tileX < 0 || tileY < 0 || tileX >= width || tileY >= height) {
        return false;
    }
    return getTile(tileX, tileY).type == TileType::Ground;
}

// Sets the tile at (tileX, tileY) to the specified type.
void Map::setTile(int tileX, int tileY, TileType type) {
    if (tileX >= 0 && tileY >= 0 && tileX < width && tileY < height) {
        getTile(tileX, tileY).type = type;
    }
}

// Returns the width in tiles of the map.
int Map::getWidth() const {
    return width;
}

// Returns the height in tiles of the map.
int Map::getHeight() const {
    return height;
}

// Returns a reference to the tile at (tileX, tileY).
const Tile& Map::getTile(int tileX, int tileY) const {
    return grid[tileY * width + tileX];
}

// Returns a reference to the tile at (tileX, tileY).
Tile& Map::getTile(int tileX, int tileY) {
    return grid[tileY * width + tileX];
}

// Draws each tile onto the render window with the dimensions (tileSize x tileSize)
void Map::draw(sf::RenderWindow& window, int tileSize) const {
    // Load a texture for the water once.
    static sf::Texture waterTexture;
    static bool waterTextureLoaded = false;
    if (!waterTextureLoaded) {
        // Check if theres an issue loading the texture. (replace with texture holder at some point)
        if (!waterTexture.loadFromFile("graphics/water.png")) {
            // If there is print debug.
            std::cout << "Couldnt load water texture.\n";
        } else {
            waterTextureLoaded = true; // Otherwise disable loading the texture again.
        }
    }

    // Load a texture for the sand once.
    static sf::Texture sandTexture;
    static bool sandTextureLoaded = false;
    if (!sandTextureLoaded) {
        // Check if theres an issue loading the texture. (replace with texture holder at some point)
        if (!sandTexture.loadFromFile("graphics/sand.png")) {
            // If there is print debug.
            std::cout << "Couldnt load sand texture.\n";
        } else {
            sandTextureLoaded = true; // Otherwise disable loading the texture again.
        }
    }

    // Load an alternate texture for the sand once.
    static sf::Texture alternateSandTexture;
    static bool alternateSandTextureLoaded = false;
    if (!alternateSandTextureLoaded) {
        // Check if theres an issue loading the texture. (replace with texture holder at some point)
        if (!alternateSandTexture.loadFromFile("graphics/sandAlternate.png")) {
            // If there is print debug.
            std::cout << "Couldnt load sand texture.\n";
        } else {
            alternateSandTextureLoaded = true; // Otherwise disable loading the texture again.
        }
    }

    // Create a sprite for the tiles & scale it to the correct size.
    sf::Sprite tileSprite;

    // Loop through every tile
    for (int tileY = 0; tileY < height; ++tileY) {
        for (int tileX = 0; tileX < width; ++tileX) {
            sf::Vector2i tilePosition(tileX, tileY); // Store current tile position
            sf::Sprite tileSprite; // Create a new sprite for each tile.

            // Check which type of tyle it should be and fill it with the right color.
            if (tilePosition == targetPoint) {
                // If the tile is a target point.
                tileSprite.setTexture(waterTexture); // Switch to the water texture.
            } 
            else if (std::find(entryPoints.begin(), entryPoints.end(), tilePosition) != entryPoints.end()) {
                // If the tile is an entry point.
                tileSprite.setTexture(waterTexture); // Switch to the water texture.
            } 
            else if (getTile(tileX, tileY).type == TileType::Wall) {
                // If the tile is a wall.
                if ((tileX + tileY) % 2 == 0) { // Apply a checkered pattern by darkening ever second tile.
                    tileSprite.setTexture(sandTexture); // Switch to the sand texture.
                } else {
                    tileSprite.setTexture(alternateSandTexture); // Switch to the alternate sand texture.
                }
            } 
            else {
                // For the initial tile.
                tileSprite.setTexture(waterTexture); // Switch to the water texture.
            }

            // Set the position for the tile then draw it with the right dimensions.
            tileSprite.setPosition((tileX * tileSize), (tileY * tileSize));
            // Set the scale of the tile sprite.
            tileSprite.setScale(tileSize / waterTexture.getSize().x, tileSize / waterTexture.getSize().y);

            window.draw(tileSprite); // Draw the tile.

            // Draw the pearl over the target tile
            sf::Vector2f targetPos(targetPoint.x * tileSize + tileSize / 2.0f, targetPoint.y * tileSize + tileSize / 2.0f);

            // Load the texture only once (replace with texture holder at some point)
            static sf::Texture pearlTexture;
            static bool textureLoaded = false;
            if (!textureLoaded) {
                // Check if theres an issue loading the texture.
                if (!pearlTexture.loadFromFile("graphics/pearl.png")) {
                    // If there is print debug.
                    std::cout << "Couldnt load pearl texture.\n";
                } else {
                    textureLoaded = true; // Otherwise disable loading the texture again.
                }
            }
            
            // Create a sprite for the pearl
            sf::Sprite pearlSprite;
            pearlSprite.setTexture(pearlTexture); // Set the sprites texture.
            sf::FloatRect spriteBounds = pearlSprite.getLocalBounds(); // Get the sprites local bounds.
            pearlSprite.setScale(2.0f, 2.0f); // Make the pearl slightly bigger.
            pearlSprite.setOrigin(spriteBounds.width / 2.0f, spriteBounds.height / 2.0f); // Center the sprites origin.
            pearlSprite.setPosition(targetPos); // Position the sprite in the middle of the target tile.
            window.draw(pearlSprite); // Draw the pearl sprite.
        }
    }
}

// Check if a cluster can be placed without overlapping another cluster.
bool Map::canPlaceCluster(const std::vector<sf::Vector2i>& clusterTiles) const {
    // Iterate over each tile in the cluster.
    for (const auto& tilePosition : clusterTiles) {
        for (int offsetY = -1; offsetY <= 1; ++offsetY) {
            for (int offsetX = -1; offsetX <= 1; ++offsetX) {
                sf::Vector2i neighborPosition(tilePosition.x + offsetX, tilePosition.y + offsetY);
                // Skip the tile if its neighbour is not in bounds.
                if (neighborPosition.x < 0 || neighborPosition.x >= width || neighborPosition.y < 0 || neighborPosition.y >= height) {
                    continue;
                }
                // If the negihbouring tile is not part of the cluster and is a wall dont place it.
                if (std::find(clusterTiles.begin(), clusterTiles.end(), neighborPosition) == clusterTiles.end() &&
                    getTile(neighborPosition.x, neighborPosition.y).type == TileType::Wall) {
                    return false;
                }
            }
        }
    }
    return true; // Otherwise the tile can be placed.
}

// Returns true if a valid path exists from the 'start' point to the 'target' point.
bool Map::hasPath(const sf::Vector2i& start, const sf::Vector2i& target) const {
    Pathfinder pathfinder; // Create an instance of the pathfinder.

    auto path = pathfinder.findPath(*this, start, target); // Find a path from the start to the target.
    return !path.empty(); // If a path exists return true.
}

// Generates a random map: 1. Fills the map with ground tiles, 
// 2. Ensures the borders remain ground, 
// 3. Sets entry and target points for the enemies, 
// 4. Places wall clusters using a random walk, ensuring that clusters do not block paths from an entry point to the target point.
void Map::generateRandom() {
    srand(time(nullptr)); // Seed random number generator.

    // Fill the entire map with ground tiles.
    for (int tileY = 0; tileY < height; ++tileY) {
        for (int tileX = 0; tileX < width; ++tileX) {
            getTile(tileX, tileY).type = TileType::Ground;
        }
    }

    // Set the outer tiles to a ground type to reserve the border.
    for (int tileX = 0; tileX < width; ++tileX) {
        getTile(tileX, 0).type = TileType::Ground;
        getTile(tileX, height - 1).type = TileType::Ground;
    }
    for (int tileY = 0; tileY < height; ++tileY) {
        getTile(0, tileY).type = TileType::Ground;
        getTile(width - 1, tileY).type = TileType::Ground;
    }

    // Set entry and target points.
    entryPoints.clear();
    entryPoints.push_back(sf::Vector2i(0, 0));
    entryPoints.push_back(sf::Vector2i(width - 1, 0));
    entryPoints.push_back(sf::Vector2i(0, height - 1));
    entryPoints.push_back(sf::Vector2i(width - 1, height - 1));
    targetPoint = sf::Vector2i(width / 2, height / 2);

    // Step 4: Place wall clusters using a random walk, ie. pick a point,
    // generate a random number and direction and move the tiles based on it, and repeat.
    int clustersPlaced = 0;
    int clusterPlacementAttempts = 0;
    int maximumAttempts = numClusters * 100;  // Limit the number of attempts at placing a new cluster

    while (clustersPlaced < numClusters && clusterPlacementAttempts < maximumAttempts) {
        ++clusterPlacementAttempts;
        // Choose a random starting point thats within the inner area.
        int randomTileX = rand() % (width - 4) + 2;
        int randomTileY = rand() % (height - 4) + 2;
        sf::Vector2i currentPosition(randomTileX, randomTileY);

        // Generate a random cluster size between min and max clusterSize.
        int clusterSize = rand() % (maxClusterSize - minClusterSize + 1) + minClusterSize;
        std::vector<sf::Vector2i> clusterTilePositions;
        clusterTilePositions.push_back(currentPosition);

        // Make the cluster larger.
        for (int i = 1; i < clusterSize; ++i) {
            std::vector<sf::Vector2i> possibleDirections = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };
            sf::Vector2i chosenDirection = possibleDirections[rand() % possibleDirections.size()];
            currentPosition += chosenDirection;

            // Make sure the cluster stays within the inner area.
            currentPosition.x = std::max(1, std::min(width - 2, currentPosition.x));
            currentPosition.y = std::max(1, std::min(height - 2, currentPosition.y));
            if (std::find(clusterTilePositions.begin(), clusterTilePositions.end(), currentPosition) == clusterTilePositions.end()) {
                clusterTilePositions.push_back(currentPosition);
            }
        }

        // Check if the cluster is beside an existing wall, if it is replace it.
        if (!canPlaceCluster(clusterTilePositions)) {
            continue;
        }

        // Place the cluster, set each tile to a wall.
        for (const auto& tilePosition : clusterTilePositions) {
            getTile(tilePosition.x, tilePosition.y).type = TileType::Wall;
        }

        // Make sure the new cluster dosent block paths from entry points to the target point.
        if (!hasPath(entryPoints[0], targetPoint)) {
            // If it is blocked remove the cluster by changing the tile back to a ground type.
            for (const auto& tilePosition : clusterTilePositions) {
                getTile(tilePosition.x, tilePosition.y).type = TileType::Ground;
            }
        } else {
            ++clustersPlaced;
        }
    }
}
