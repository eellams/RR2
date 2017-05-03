#ifndef _PATH_FINDER_HPP
#define _PATH_FINDER_HPP

#include <irrlicht.h>
#include <vector>
#include <map>

class PathFinder {
public:
  PathFinder();
  PathFinder(const PathFinder &obj);
  ~PathFinder();

  void initialise(const irr::u32 &width, const irr::u32 &height);

  void updateGrid(const irr::u32 &tid, const std::map<irr::u32, irr::f32> &speed);

  std::vector<irr::core::vector2df> findPath (
    const irr::f32 &startX,
    const irr::f32 &startY,
    const irr::f32 &endX,
    const irr::f32 &endY
  );

private:
  // map< grid layer, move speed >
  std::vector<irr::u32> mGridFlags;
  std::vector<std::map<irr::u32, irr::f32> > mGrid;

  irr::u32 mWidth;
  irr::u32 mHeight;
};

#endif
