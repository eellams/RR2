#ifndef _BUILDING_MANAGER_HPP
#define _BUILDING_MANAGER_HPP

#include "itiledmanager.hpp"
#include <irrlicht.h>
#include <map>

class Building;
class BuildingType;

class BuildingManager : public ITiledManager<BuildingType, Building> {
public:
  BuildingManager();
  BuildingManager(const BuildingManager &obj);
  ~BuildingManager();

  void add(const irr::u32 &tilenumber,
    const irr::u32 &tid,
    const std::array<irr::f32, 4> &cornerheights,
    const irr::u32 &knownid = 0);
  void remove(const irr::u32 &id);

  void recalculate(const irr::u32 &id);
  void recalculateByTileNumber(const irr::u32 &tilenumber);

private:
  void initialiseTypes();
  void initialiseInstances();
};

#endif
