// Name: Conor Ryan
// K-NUM: K00286377

#pragma once

// The possible states for tiles - ground is walkable, walls are not walkable.
enum class TileType {
    Ground,
    Wall
};

// A representation of a single tile in the grid - all tiles are ground by default.
struct Tile {
    TileType type = TileType::Ground;
};
