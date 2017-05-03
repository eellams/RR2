#include "pathfinder.hpp"
#include <iostream>

PathFinder::PathFinder() :
  mGrid(),
  mGridFlags(),
  mWidth(0),
  mHeight(0)
{

}
PathFinder::PathFinder(const PathFinder &obj) :
  mGrid(obj.mGrid),
  mGridFlags(obj.mGridFlags),
  mWidth(obj.mWidth),
  mHeight(obj.mHeight)
{

}

PathFinder::~PathFinder() {

}

void PathFinder::initialise(const irr::u32 &width, const irr::u32 &height) {
  mWidth = width;
  mHeight = height;

  std::map<irr::u32, irr::f32> asdf = std::map<irr::u32, irr::f32>();
  mGridFlags.resize(width*height, 0);
  mGrid.resize(width*height, asdf);
}

void PathFinder::updateGrid(const irr::u32 &tid,
  const std::map<irr::u32, irr::f32> &speed)
{
  std::printf("Setting grid tile %u", tid);

  for (const auto &p : speed)
    std::printf(" layer %u speed %f", p.first, p.second);

  // Create the bitmask
  irr::u32 flag = 0;
  for (const auto &i : speed)
    flag |= i.first;

  mGridFlags[tid] = flag;
  mGrid[tid] = speed;
}

std::vector<irr::core::vector2df> PathFinder::findPath (
  const irr::f32 &startX,
  const irr::f32 &startY,
  const irr::f32 &endX,
  const irr::f32 &endY
)
{
  std::printf("Pathfinding from (%f,%f) to (%f,%f)", startX, startY, endX, endY);
  irr::u32 startTile = 2;

  std::vector<std::vector<int> > openList;
  std::vector<int> baseRow;
  openList.reserve(mHeight);

  for (irr::u32 i=0; i<mHeight; i++)
      openList.push_back(baseRow);

  return std::vector<irr::core::vector2df>();
}
