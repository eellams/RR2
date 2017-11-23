#include "pathfinder.hpp"
#include <iostream>
#include <cstdlib>

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
  std::printf("Setting grid tile %u\r\n", tid);

  for (const auto &p : speed)
    std::printf(" layer %u speed %f\r\n", p.first, p.second);

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
  std::printf("Pathfinding from (%f,%f) to (%f,%f)\r\n", startX, startY, endX, endY);
  irr::u32 startTile = 11;
  irr::u32 endTile = 14;
  bool finished = false;

  irr::u32 current = startTile;

  // openList consists of g values
  //std::vector<std::vector<irr::f32> > openList(mWidth*mHeight, 0.0);
  std::vector<irr::f32> gValues(mWidth*mHeight, 0.0); // Cost to reach each node
  std::vector<irr::f32> fValues(mWidth*mHeight, 0.0); // Estimated to reach goal

  std::vector<std::vector<bool> > closedList(mWidth*mHeight, false);
  std::vector<std::vector<irr::f32> > parentList(mWidth*mHeight, 0.0);

  gValues[startTile] = 1;

  while (!finished)
  {
    // Add to closed list
    closedList[current] = true;

    // Calculate possible neighbours
    std::vector<irr::f32> neighbours;
    if (current % mWidth != 0) neighbours.append(current-1);
    if (current % mWidth != mWidth-1) neighbours.append(current+1);
    if (current < (mHeight-1)*mWidth) neighbours.append(current+mWidth);
    if (current > mWidth) neighbours.append(current-mWidth);

    // Add neighbours to open list
    for const irr::u32 &n : neighbours
    {
      // If not closed, and not previously analysed
      if (!closedList[n] && (gValues[n] == 0.0 || gValues[n] > gValues[current] + 1 ) )
      {
        // Add to openList
        gValues[n] = gvalues[current] + 1;
        fValues[n] = abs((n%width) - (current%width)) + abs(n/width + current/width);
        parentList[n] = current;
      }
    }

    // Get next node (lowest f score)

  }

    // Add neighbours to open list

    // If not already closed, and if weight from
    if (!closedList[current] && gValues[current] != 0 && gValues[current] <)
    openList[current+mWidth] = openList[current] + 1;
    openList[current+1] = openList[current] + 1;


  return std::vector<irr::core::vector2df>();
}
