#ifndef _BUILDING_MANAGER_HPP
#define _BUILDING_MANAGER_HPP

#include <irrlicht.h>
#include <map>

class Building;
class BuildingType;

class BuildingManager {
public:
  BuildingManager();
  ~BuildingManager();

  std::map<irr::u32, Building> getBuildings() const;
  std::map<irr::u32, BuildingType> getBuildingTypes() const;

  void setBuildings(const std::map<irr::u32, Building> buildings);
  void setBuildingTypes(const std::map<irr::u32, BuildingType> &btypes);
  void setWidth(const irr::u32 &width);
  void setHeight(const irr::u32 &height);

  void add(const irr::u32 &tilenumber, const irr::u32 &btypeid, const irr::f32 &bheight, const irr::u32 &knownid = 0);
  void initialise(irr::scene::ISceneNode *parentnode);
  void recalculateByTileNumber(const irr::u32 &tilenumber);
  void recalculate(const irr::u32 &buildingid);
  void remove(const irr::u32 &buildingid);

private:
  void initialiseBuildingTypes();
  void initialiseBuildings();

  irr::core::vector3df tileNumberToPosition(const int& tilenumber);

  irr::u32 mWidth;
  irr::u32 mHeight;
  irr::u32 mBuildingId;
  std::map<irr::u32, BuildingType> mBuildingTypes;
  std::map<irr::u32, Building> mBuildings;

  irr::scene::ISceneNode *pBuildingNode;
};

#endif
