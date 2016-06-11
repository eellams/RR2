#ifndef _MAP_STRUCTS_HPP
#define _MAP_STRUCTS_HPP

#define TILE_SIZE 10.0f // The 3D size of a tile
#define TILE_WALL_HEIGHT 12.5f

struct Surround {
  bool current, below, left, right, above;
};

#endif