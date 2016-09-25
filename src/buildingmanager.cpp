#include "buildingmanager.hpp"

#include "buildingtype.hpp"
#include "building.hpp"

BuildingManager::BuildingManager() :
  TiledManager<BuildingType, Building>()
{

}

BuildingManager::BuildingManager(const BuildingManager &obj) :
  TiledManager<BuildingType, Building>(obj)
{
}

BuildingManager::~BuildingManager() {

}

// Add a new Building
//  or initialise a serialised building
void BuildingManager::add(const irr::u32 &tilenumber, const irr::u32 &tid, const std::array<irr::f32, 4> &cornerheights, const irr::u32 &knownid) {
  irr::core::vector3df pos;
  irr::u32 bid;
  bool isOccupied;

  // Check if the tile is occupied [by another building] or not
  isOccupied = false;
  for (std::map<irr::u32, Building>::iterator it = mInstances.begin(); it != mInstances.end(); it++) {
    if (it->second.getTileNumber() == tilenumber)
      isOccupied = true;
  }

  // If the ID of the building is known
  //  e.g. whether need to create an entirely new building with new ID
  if (knownid == 0) {
    bid = mId;

    // Don't add if there is already a building present
    if (!isOccupied) {
      mId++;
      mInstances[bid] = Building(bid, tid, tilenumber);
    }
  }
  else {
    // Ensure that mId is always bigger than the highest value on the map
    bid = knownid;
    if (bid >= mId) mId = bid + 1;
  }

  // If the path exists
  if (mInstances.find(bid) != mInstances.end()) {
    std::clog << "Adding new building, bid " << tid;
    std::clog << " (" << mTypes[tid].getName() << "), ";
    std::clog << " id " << bid << std::endl;
    std::clog << " on tile " << tilenumber << std::endl;

    // Get position
    //  increase X and Z so that the buiding sits in the middle of the tile
    pos = tileNumberToPosition(tilenumber);
    pos.X += 0.5 * TILE_SIZE;
    pos.Z += 0.5 * TILE_SIZE;

    // Ensure that the building is at the correct height
    pos.Y = *std::max_element(cornerheights.begin(), cornerheights.end());

    // Initialise the building
    mInstances[bid].initialise(
      pNode,
      mTypes[tid],
      pos
    );
  }
}

// Recaluate by tile number
void BuildingManager::recalculateByTileNumber(const irr::u32 &tilenumber) {
  // Finds buildings by given tile number
  //  then recalculates the building by building id
  for (std::map<irr::u32, Building>::iterator it = mInstances.begin(); it != mInstances.end(); it++) {
    if (it->second.getTileNumber() == tilenumber) {
      recalculate(it->second.getTileNumber());
    }
  }
}

// Recaluates the building
void BuildingManager::recalculate(const irr::u32 &buildingid) {
  // TODO whatever here
}

// Removes the building
void BuildingManager::remove(const irr::u32 &buildingid) {
  std::clog << "Removng building " << buildingid << std::endl;
  std::map<irr::u32, Building>::iterator it = mInstances.find(buildingid);
  if (it != mInstances.end()) {
    std::clog << " erasing" << std::endl;
    mInstances.erase(it);
  }
    //mInstances.erase(buildingid);
}

void BuildingManager::initialiseTypes() {
  // TODO may be needed
  // Nothing to do here
}

void BuildingManager::initialiseInstances() {
  // This is empty, it's functionality is implemented in Map::initialise
  //  this is because, in order to know how high to place the building, the
  //  height of the tile needs to be known.
  //  This height is known in TileManager, which is inaccessible here, but not
  //  in Map (if that makes any sense)
}
