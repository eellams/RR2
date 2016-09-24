#ifndef _GENERIC_HPP
#define _GENERIC_HPP

#include <irrlicht.h>
#include <vector>

#define TILE_SIZE 10.0f
#define TILE_WALL_HEIGHT 12.5f

#define COLLISION_MASK_TILE 1 << 30

enum MoveType {
  AIR = 0,
  GROUND = 1,
  WATER = 2
};

struct TrianglePoint {
  irr::core::vector3df pos;
  irr::core::vector3df normal;
  irr::video::SColor colour;
  irr::core::vector2df uv;
};

struct TriStrip {
  std::vector<TrianglePoint> points;
};

struct Surround {
  bool current, below, left, right, above;
  bool belowLeft, belowRight, aboveLeft, aboveRight;

  inline bool operator==(Surround &a) {
    if (a.current == current &&
      a.below == below &&
      a.left == left &&
      a.right == right &&
      a.above == above &&
      a.belowLeft == belowLeft &&
      a.belowRight == belowRight &&
      a.aboveLeft == aboveLeft &&
      a.aboveRight == aboveRight)
      return true;
    else
      return false;
  }
};

#endif
