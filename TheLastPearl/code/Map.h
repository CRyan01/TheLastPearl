// Name: Conor Ryan
// K-NUM: K00286377

#pragma once

#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "Tile.h"

// The map class represents a grid based world made up of various tiles.
class Map {
public:
	// Constructor - creates a map using specified parameters.
	Map(int width, int height, int numClusters, int minClusterSize, int maxClusterSize);

	// Loads a map layout from a file.
	bool loadFromFile(const std::string& filename);

	// Generates a random map.
	void generateRandom();

	// Returns the width of the map in number of tiles.
	int getWidth() const;

	// Returns the height of the map in number of tiles.
	int getHeight() const;

	// Returns a constant reference to the tile at (x, y) in the grid.
	const Tile& getTile(int x, int y) const;

	// Returns a  reference to the tile at (x, y) to alter it.
	Tile& getTile(int x, int y);

	// Checks if the tile at (x, y) is not a wall.
	bool isWalkable(int x, int y) const;

	// Sets the tile at (x, y) to a specified type.
	void setTile(int x, int y, TileType type);

	// Draws the map onto the SFML view, and sets the pixel size of tiles.
	void draw(sf::RenderWindow& window, int tileSize) const;

	// Determines if there is a valid path from the start to target tile.
	bool hasPath(const sf::Vector2i& start, const sf::Vector2i& target) const;

	sf::Vector2i targetPoint; // The target tile (Center of the map).
	std::vector<sf::Vector2i> entryPoints; // The enemies entry points (Corners of the map).

private:
	// Checks if a cluster of wall tiles can be placed without overlapping with.
	// tiles that were already placed
	bool canPlaceCluster(const std::vector<sf::Vector2i>& clusterTiles) const;

	int width; // The width of the map in tiles.
	int height; // The height of the map in tiles.
	std::vector<Tile> grid; // A 1D vector which stores the grid of tiles.

	int numClusters; // The number of clusters to place.
	int minClusterSize; // The minimum cluster size in tiles.
	int maxClusterSize; // The maximum cluster size in tiles.
};
