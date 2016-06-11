#ifndef _GENERIC_HPP
#define _GENERIC_HPP

#define TILE_SIZE 10.0f
#define TILE_WALL_HEIGHT 12.5f

#define COLLISION_MASK_TILE 1 << 30

struct TrianglePoint {
  irr::core::vector3df pos;
  irr::core::vector3df normal;
  irr::video::SColor colour;
  irr::core::vector2df uv;
};

struct TriStrip {
  std::vector<TrianglePoint> points;
};

enum MoveType {
  AIR = 0,
  GROUND = 1,
  WATER = 2
};

#endif
