#include "pathmanager.hpp"
#include "generic.hpp"
#include "path.hpp"
#include "pathtype.hpp"

#include <iostream>

PathManager::PathManager() : ITiledManager<PathType, Path>() {

}

PathManager::PathManager(const PathManager &obj) :
  ITiledManager<PathType, Path>(obj)
{
}

PathManager::~PathManager() {
}

// Add a new path
void PathManager::add(const irr::u32 &tilenumber,
  const irr::u32 &tid,
  const std::array<irr::f32, 4> &cornerHeights,
  const irr::u32 &knownid) {

  irr::core::vector3df pos;
  irr::u32 pid;

  bool isOccupied = false;

  // Check if the tile is occupied or not
  for (std::map<irr::u32, Path>::iterator it = mInstances.begin(); it != mInstances.end(); it++) {
    if (it->second.getTileNumber() == tilenumber)
      isOccupied = true;
  }

  // If the ID of the path is known
  //  e.g. when the path has previously been added (serialised from map file)
  if (knownid == 0) {
    pid = mId;

    // Don't add the path if there is already one present
    if (!isOccupied) {
      mId++;
      mInstances[pid] = Path(pid, tid, tilenumber);
    }
  }
  else {
    // Increase the id so that the next path's ID will be correct
    pid = knownid;
    if (pid >= mId) mId = pid + 1;
  }

  // If the path exists
  //  only wouldn't exist if the ID is not known, and there is already a path
  //  on the tile
  if (mInstances.find(pid) != mInstances.end()) {
    std::clog << "Adding new path, pid " << tid;
    std::clog << " (" << mTypes[tid].getName() << "), ";
    std::clog << " id " << pid << std::endl;
    std::clog << " on tile " << tilenumber << std::endl;

    // Get the posiition
    pos = tileNumberToPosition(tilenumber);

    // Initialise etc.
    mInstances[pid].initialise(pNode, pid, pos);
    mInstances[pid].createModel(cornerHeights);
    mInstances[pid].setAlpha();

    // Don't calculate the power if the tile's id is known
    //  i.e. has been serialised from a map tile
    // As all tiles need to be initialised before calculating power
    //  otherwise the whole power thing is erroneous
    if (knownid == 0)
      recalculatePower();
  }

}

// Remove a current path
void PathManager::remove(const irr::u32 &id){
  std::clog << "Removing path " << id << std::endl;

  // If the path exists
  if (mInstances.find(id) != mInstances.end()) {
    // Remove it (by iterator to avoid ambiguity)
    auto it = mInstances.find(id);
    mInstances.erase(it);

    // Recaluate the power situation, as something may have changed
    recalculatePower();
  }
}

// Recaluate a particular tile
void PathManager::recalculate(const irr::u32 &id) {
  // TODO
  // Is this ever called?
}

void PathManager::recalculateByTileNumber(const irr::u32 &tilenumber) {
  // TODO
}

// Turn on the paths that should be turned on
void PathManager::recalculatePower() {
  std::vector<irr::u32> pathIds;

  std::clog << "Recalculating power" << std::endl;

  // Create a list of all the path IDs
  for (std::map<irr::u32, Path>::iterator it = mInstances.begin(); it != mInstances.end(); it++) {
    pathIds.push_back(it->first);
  }

  // Assume that nothing is conducting
  //  (will be turned on later as required)
  for (const irr::u32 &pathid : pathIds) {
    clearPathConducting(pathid);
  }

  // Work out what should be conducting
  for (const irr::u32 &pathid : pathIds) {
    // If is powered, then turn on the relevant net
    if (mInstances[pathid].getPowered()) {
      turnOnPathNet(pathid);
    }
  }
}

// Set a path to be conducting
void PathManager::setPathConducting(const irr::u32 &pathid) {
  mInstances[pathid].turnOn();
  mInstances[pathid].setTexture(mTypes[mInstances[pathid].getTypeId()].getTextureConductingName());
}

// Clear a path from being conducting
void PathManager::clearPathConducting(const irr::u32 &pathid) {
  mInstances[pathid].turnOff();
  mInstances[pathid].setTexture(mTypes[mInstances[pathid].getTypeId()].getTextureName());
}

// Initialise the path types
void PathManager::initialiseTypes() {
  // Nothing to do here
}

// Initialise the paths themselves
void PathManager::initialiseInstances() {
  // TODO
}

// Turn on all paths that are 'connected' to a powered path
//  does some fancy maths to work this out (similar to pathfinding algorithms)
void PathManager::turnOnPathNet(const irr::u32 &pathid) {
  bool inClosedList;
  irr::u32 searchTileNumber;
  irr::u32 currentPathId;
  std::vector<irr::u32> openList;
  std::vector<irr::u32> closedList;
  std::vector<irr::u32> adjacentPaths;
  std::vector<irr::u32>::iterator it;

  std::clog << "Turning on path " << pathid << std::endl;

  // Start with this tile
  openList.push_back(pathid);

  // While there are unchecked tiles
  while (!openList.empty()) {
    // The path id of the tile currently being checked
    currentPathId = openList[0];

    // Get the paths that are directly adjacent to this one
    adjacentPaths = getAdjacentPathIds(currentPathId);

    // Remove the current path from the open list,
    //  and add to the closed [checked] list
    closedList.push_back(currentPathId);
    it = std::find(openList.begin(), openList.end(), currentPathId);
    openList.erase(it);

    // Turn the path on, if it can be switched on
    if ( mTypes[mInstances[currentPathId].getTypeId()].getConductive()) {
      setPathConducting(currentPathId);
    }

    // Add new paths to the open list
    //  these being adjacent tiles that have not already been checked
    for (const irr::u32 &checkid : adjacentPaths) {
      inClosedList = std::find(closedList.begin(), closedList.end(), checkid) != closedList.end();
      if (!inClosedList && mTypes[mInstances[checkid].getTypeId()].getConductive()) {
        openList.push_back(checkid);
      }
    }
  }
}

// Get a vector of the path ids of paths directly adjacent
std::vector<irr::u32> PathManager::getAdjacentPathIds(const irr::u32 &pathid) {
  std::vector<irr::u32> toReturn;
  irr::u32 tilenumber;
  std::vector<irr::u32> surroundingTileNumbers;
  bool inSurroundingTileNumbers;

  // Work out what tile we are on, and get the surrounding tiles
  tilenumber = mInstances[pathid].getTileNumber();
  surroundingTileNumbers = getSurroundingTileNumbers(tilenumber);

  // For each of the paths, work out whether the path is within the surrounding
  //  tile numbers
  for (std::map<irr::u32, Path>::iterator it = mInstances.begin(); it != mInstances.end(); it++) {
    tilenumber = mInstances[it->first].getTileNumber();
    inSurroundingTileNumbers = std::find(surroundingTileNumbers.begin(), surroundingTileNumbers.end(), tilenumber) != surroundingTileNumbers.end();

    // If in the surrounding tiles, add the path id to the list
    if (inSurroundingTileNumbers) {
      toReturn.push_back(it->first);
    }
  }

  return toReturn;
}
