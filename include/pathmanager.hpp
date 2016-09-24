#ifndef _PATH_MANAGER_HPP
#define _PATH_MANAGER_HPP

#include "tiledmanager.hpp"
#include <irrlicht.h>
#include <map>

class Path;
class PathType;

class PathManager : public TiledManager<PathType, Path> {
public:
  PathManager();

  void add(const irr::u32 &tilenumber,
    const irr::u32 &tid,
    const std::array<irr::f32, 4> &cornerheights,
    const irr::u32 &knownid = 0);
  void remove(const irr::u32 &id);

  void recalculate(const irr::u32 &id);
  void recalculateByTileNumber(const irr::u32 &tilenumber);

  void recalculatePower();
  void setPathConducting(const irr::u32 &pathid);
  void clearPathConducting(const irr::u32 &pathid);

private:
  void initialiseTypes();
  void initialiseInstances();

  void turnOnPathNet(const irr::u32 &pathid);
  std::vector<irr::u32> getAdjacentPathIds(const irr::u32 &pathid);
};

#endif
