#include "buildingmanager.hpp"

#include "buildingtype.hpp"
#include "building.hpp"

BuildingManager::BuildingManager() :
  mBuildingId(0),
  mBuildingTypes(),
  mBuildings(),
  pBuildingNode(NULL)
{

}

BuildingManager::~BuildingManager() {

}

void BuildingManager::initialise(irr::scene::ISceneNode *parentnode) {
  pBuildingNode = parentnode->getSceneManager()->addEmptySceneNode();
  pBuildingNode->setParent(parentnode);

  initialiseBuildingTypes();
  initialiseBuildings();
}

void BuildingManager::add(const irr::u32 &tilenumber, const irr::u32 &btypeid, const irr::f32 &bheight, const irr::u32 &knownid) {
  // TODO add
  irr::core::vector3df pos;
  irr::u32 bid;

  if (knownid == 0) {
    bid = mBuildingId;
    mBuildingId++;
    mBuildings[bid] = Building(bid, btypeid, tilenumber);
  }
  else {
    bid = knownid;
    if (bid > mBuildingId) mBuildingId = bid + 1;
  }

  std::clog << "Adding new building, bid " << btypeid;
  std::clog << " (" << mBuildingTypes[btypeid].getName() << "), ";
  std::clog << " id " << bid << std::endl;
  std::clog << " on tile " << tilenumber << std::endl;

  pos = tileNumberToPosition(tilenumber);
  pos.Y = bheight;

  mBuildings[bid].initialise(
    pBuildingNode,
    mBuildingTypes[btypeid],
    pos
  );
}

void BuildingManager::recalculateByTileNumber(const irr::u32 &tilenumber) {
  for (std::map<irr::u32, Building>::iterator it = mBuildings.begin(); it != mBuildings.end(); it++) {
    if (it->second.getTileNumber() == tilenumber) {
      recalculate(it->second.getTileNumber());
    }
  }
}

void BuildingManager::recalculate(const irr::u32 &buildingid) {
  // TODO whatever here
}

void BuildingManager::remove(const irr::u32 &buildingid) {
  std::clog << "Removng building " << buildingid << std::endl;
  mBuildings.erase(buildingid);
}

std::map<irr::u32, Building> BuildingManager::getBuildings() const {
  return mBuildings;
}

std::map<irr::u32, BuildingType> BuildingManager::getBuildingTypes() const {
  return mBuildingTypes;
}

void BuildingManager::setBuildings(const std::map<irr::u32, Building> &buildings) {
  mBuildings = buildings;
}

void BuildingManager::setBuildingTypes(const std::map<irr::u32, BuildingType> &btypes) {
  mBuildingTypes = btypes;
}

void BuildingManager::setWidth(const irr::u32 &width) {
  mWidth = width;
}

void BuildingManager::setHeight(const irr::u32 &height) {
  mHeight = height;
}

void BuildingManager::initialiseBuildingTypes() {
  // TODO may be needed
  // Nothing to do here
}

void BuildingManager::initialiseBuildings() {
  // This is empty, it's functionality is implemented in Map::initialise
  //  this is because, in order to know how high to place the building, the
  //  height of the tile needs to be known.
  //  This height is known in TileManager, which is inaccessible here, but not
  //  in Map (if that makes any sense)
}

// Converts a tile number into the bottom left position of a tile
irr::core::vector3df BuildingManager::tileNumberToPosition(const int& tilenumber) {
  irr::core::vector3df pos;
  pos.set(
    (tilenumber % mWidth)*TILE_SIZE,
    0,
    (tilenumber / mWidth)*TILE_SIZE
  );

  pos.X += TILE_SIZE * 0.5;
  pos.Z += TILE_SIZE * 0.5;
  return pos;
}
