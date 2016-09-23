#include "map.hpp"
#include "tile.hpp"
#include "tiletype.hpp"
#include "buildingtype.hpp"
#include "building.hpp"
#include "pathtype.hpp"
#include "path.hpp"
#include "tilemanager.hpp"
#include "buildingmanager.hpp"

Map::Map() :
  mName(""),
  mDescription(""),
  mWidth(0),
  mHeight(0),
  mPathTypes(),
  mPaths(),
  pTileManager(NULL),
  pBuildingManager(NULL)
{
  pTileManager = new TileManager();
  pBuildingManager = new BuildingManager();
}

Map::~Map() {

}

std::string Map::getName() const {
  return mName;
}

irr::scene::IMetaTriangleSelector* Map::getMetaTriangleSelectorP() const {
  //return pTileSelector;
  return pTileManager->getTileSelector();
}


void Map::initialise(irr::video::IVideoDriver* driver, irr::scene::ISceneManager* smgr) {
  std::clog << "Initialising map '" << mName << "'" << std::endl;

  pTileManager->initialise(smgr->getRootSceneNode());

  pBuildingManager->initialise(smgr->getRootSceneNode());
  
  for (auto& building : pBuildingManager->getBuildings()) {
    pBuildingManager->add(
      building.second.getTileNumber(),
      building.second.getBuildingType(),
      pTileManager->getMaxTileHeight(building.second.getTileNumber()),
      building.second.getBuildingId()
    );
  }

  // Initilise the types
  //initialiseTileTypes();
  //initialiseBuildingTypes();
  ////initialisePathTypes();

  // Initialise the actual instances of the types
  //initialiseTiles(smgr->getRootSceneNode());
  //initialiseBuildings(smgr->getRootSceneNode());
  ////initialisePaths(smgr->getRootSceneNode());
  ////recalculatePathPower();

}

void Map::mineTile(const irr::u32& tileNumber) {
  // Change the tile into the type
  setTile(tileNumber, pTileManager->getTileMineInto(tileNumber), true);

  // TODO dropping ore, etc.
}

void Map::setTile(const irr::u32& tileNumber, const irr::u32& tileType, const bool& enableCaveIn) {
  struct Surround newSurround;
  struct Surround oldSurround;

  std::clog << "Setting tile " << tileNumber;
  std::clog << " to " << tileType << std::endl;

  oldSurround = pTileManager->getTileSurround(tileNumber);

  pTileManager->setTileTileType(tileNumber, tileType);
  pTileManager->recalculate(tileNumber);

  newSurround = pTileManager->getTileSurround(tileNumber);

  if (!(oldSurround == newSurround)) {
    recalculateSurroundingTileModels(tileNumber, enableCaveIn);
  }
}

// Serialisation
template<class Archive>
void Map::serialize(Archive & ar, const unsigned int version) {
  try {
    std::map<irr::u32, TileType> TileTypes = pTileManager->getTileTypes();
    std::vector<Tile> Tiles = pTileManager->getTiles();

    std::map<irr::u32, BuildingType> BuildingTypes = pBuildingManager->getBuildingTypes();
    std::map<irr::u32, Building> Buildings = pBuildingManager->getBuildings();

    ar & BOOST_SERIALIZATION_NVP(mName);
    ar & BOOST_SERIALIZATION_NVP(mDescription);
    ar & BOOST_SERIALIZATION_NVP(mRoofTexture);
    ar & BOOST_SERIALIZATION_NVP(mWidth);
    ar & BOOST_SERIALIZATION_NVP(mHeight);
    ar & BOOST_SERIALIZATION_NVP(TileTypes);
    ar & BOOST_SERIALIZATION_NVP(BuildingTypes);
    ar & BOOST_SERIALIZATION_NVP(mPathTypes);
    ar & BOOST_SERIALIZATION_NVP(Buildings);
    ar & BOOST_SERIALIZATION_NVP(mPaths);
    ar & BOOST_SERIALIZATION_NVP(Tiles);

    pTileManager->setTileTypes(TileTypes);
    pTileManager->setTiles(Tiles);
    pTileManager->setWidth(mWidth);
    pTileManager->setHeight(mHeight);
    pTileManager->setRoofTexture(mRoofTexture);

    pBuildingManager->setBuildingTypes(BuildingTypes);
    pBuildingManager->setBuildings(Buildings);
    pBuildingManager->setWidth(mWidth);
    pBuildingManager->setHeight(mHeight);
  }
  catch (boost::archive::archive_exception& ex) {
    std::clog << ex.what() << std::endl;
  }
}

// These two functions are needed in the souce file as a bit of a quirk that
//  is required in order to not get linker errors when compiling
//  Otherwise get errors when compiling the template to a .o file
template void Map::serialize<boost::archive::xml_iarchive>(
    boost::archive::xml_iarchive & ar,
    const unsigned int version
);
template void Map::serialize<boost::archive::xml_oarchive>(
    boost::archive::xml_oarchive & ar,
    const unsigned int version
);

void Map::initialisePathTypes() {
  // TODO is anything needed here?
}

// Initialises all the paths on the map
void Map::initialisePaths(irr::scene::ISceneNode* parentNode) {
  //std::clog << "Initialising paths" << std::endl;
  //irr::core::vector3df pos;

  //pPathNode = parentNode->getSceneManager()->addEmptySceneNode();
  //pPathNode->setParent(parentNode);

  /*for (auto& path : mPaths) {
    //pos = tileNumberToPosition(path.second.getTileNumber());
    pos.set(
      (path.second.getTileNumber() % mWidth)*TILE_SIZE,
      0,//mTiles[tilenumber].getCornerHeightMax(),
      (path.second.getTileNumber() / mWidth)*TILE_SIZE
    );

    //pos.Y = 0;

    path.second.initialise(
      pPathNode,
      mPathTypes[path.second.getPathType()],
      pos
    );

    //path.second.setCornerHeights(mTiles[path.second.getTileNumber()].getCornerHeigts());
    path.second.setCornerHeights(createTileCorners(path.second.getTileNumber()));
    path.second.createModel();
    path.second.setTexture( mPathTypes[path.second.getPathType()].getTextureName() );
    path.second.setAlpha();
  }*/
}

// Recalculates everything to do with a specific tile
void Map::recalculateAll(const irr::u32 &tileNumber, const bool &enableCaveIn) {
  /*recalculateTile(tileNumber, enableCaveIn);
  //recalculateBuilding(tileNumber);
  recalculatePath(tileNumber);*/

  pTileManager->recalculate(tileNumber);
  pBuildingManager->recalculateByTileNumber(tileNumber);
  //recalculatePath(tileNumber);
}

// Recalculates a path (on a tile)
void Map::recalculatePath(const irr::u32& tileNumber) {

}

void Map::addBuilding(const irr::u32 &tileNumber, const irr::u32 &buildingType) {
  pBuildingManager->add(tileNumber, buildingType, pTileManager->getMaxTileHeight(tileNumber));
}

void Map::removeBuilding(const irr::u32 &buildingid) {
  pBuildingManager->remove(buildingid);
}

void Map::addPath(const irr::u32& tileNumber, const irr::u32& pathType) {
  // TODO adding paths
}

void Map::removePath(const irr::u32& pathId) {
  mPaths.erase(pathId);
}

// Returns a list of the tile numbers of the tiles surrounding a particular tile
std::vector<irr::u32> Map::getSurroundingTileNumbers(const irr::u32& tileNumber) {
  std::vector<irr::u32> toreturn;

  bool above = false;
  bool below = false;
  bool left = false;
  bool right = false;

  // Tile below
  if (tileNumber >= mWidth) {
    below = true;
  }

  if (tileNumber < (mHeight-1)*mWidth) {
    below = true;
  }

  if ((tileNumber > 0) && (tileNumber % mWidth != 0)) {
    left = true;
  }

  if ((tileNumber + 1) % mWidth != 0) {
    right = true;
  }

  if (above) {
    toreturn.push_back(tileNumber + mWidth);
    if (left) toreturn.push_back(tileNumber + mWidth - 1);
    if (right) toreturn.push_back(tileNumber + mWidth + 1);
  }

  if (below) {
    toreturn.push_back(tileNumber - mWidth);
    if (left) toreturn.push_back(tileNumber - mWidth - 1);
    if (right) toreturn.push_back(tileNumber - mWidth + 1);
  }

  if (left) toreturn.push_back(tileNumber - 1);

  if (right) toreturn.push_back(tileNumber + 1);

  return toreturn;
}

// Recalculates the surrounding tiles for a given tile number
//  i.e. takes into account the tiles that don't exist (by the edges)
void Map::recalculateSurroundingTileModels(const int& tileNumber, const bool& enableCaveIn) {
  bool above;
  bool below;

  above = false;
  below = false;

  // If there is a tile below
  if (tileNumber >= mWidth) {
    recalculateAll(tileNumber-mWidth, enableCaveIn);
    below = true;
  }

  // If there is a tile above
  if (tileNumber < (mHeight-1)*mWidth) {
    recalculateAll(tileNumber+mWidth, enableCaveIn);
    above = true;
  }

  // If there is a tile left
  if ((tileNumber > 0) && ((tileNumber) % mWidth != 0)) {
    recalculateAll(tileNumber-1, enableCaveIn);

    // If there is a tile above left
    if (above) {
      recalculateAll(tileNumber+mWidth-1, enableCaveIn);
    }

    // If there is a tile below left
    if (below) {
      recalculateAll(tileNumber-mWidth-1, enableCaveIn);
    }
  }

  // If there is a tile right
  if ( (tileNumber + 1) % mWidth != 0) {
    recalculateAll(tileNumber+1, enableCaveIn);

    // If there is a tile above right
    if (above) {
      recalculateAll(tileNumber+mWidth+1, enableCaveIn);
    }

    // If there is a tile below right
    if (below) {
      recalculateAll(tileNumber-mWidth+1, enableCaveIn);
    }
  }
}

void Map::setPathConducting(const irr::u32 &pathid) {
  mPaths[pathid].turnOn();
  mPaths[pathid].setTexture( mPathTypes[mPaths[pathid].getPathType()].getTextureConductingName() );
}

void Map::clearPathConducting(const irr::u32 &pathid) {
  mPaths[pathid].turnOff();
  mPaths[pathid].setTexture( mPathTypes[mPaths[pathid].getPathType()].getTextureName() );
}

void Map::recalculatePathPower() {
  std::vector<irr::u32> pathIds;
  std::vector<int> closedList;

  for (const auto &path : mPaths) {
    irr::u32 currentTile = path.second.getTileNumber();
    pathIds.push_back(path.first);
  }

  // Assume that nothing is conducting
  for (const irr::u32 &pathid : pathIds) {
    // If is powered, then turn on
    clearPathConducting(pathid);
  }

  // Work out what shoudl be condu
  for (const irr::u32 &pathid : pathIds) {
    // If is powered, then turn on
    if (mPaths[pathid].getPowered()) {
      closedList.clear();
      turnOnPathNet(pathid, closedList);
    }
  }
}

void Map::turnOnPathNet(const int pathid, std::vector<int> &closedList) {
  bool inSurroundingTileNumbers;
  bool inCheckedList;
  irr::u32 searchPathId;
  irr::u32 searchTileNumber;

  irr::u32 tileNumber = mPaths[pathid].getTileNumber();
  std::vector<irr::u32> surroundingTileNumbers = getSurroundingTileNumbers(tileNumber);

  closedList.push_back(pathid);
  setPathConducting(pathid);

  for (const std::pair<irr::u32, Path> &idpathpair : mPaths) {
    searchPathId = idpathpair.first;
    searchTileNumber = idpathpair.second.getTileNumber();

    inSurroundingTileNumbers = std::find(surroundingTileNumbers.begin(), surroundingTileNumbers.end(), searchTileNumber) != surroundingTileNumbers.end();

    if (!closedList.empty())
      inCheckedList = std::find(closedList.begin(), closedList.end(), searchPathId) != closedList.end();
    else
      inCheckedList = false;


    if (inSurroundingTileNumbers && !inCheckedList) {
      closedList.push_back(pathid);
      turnOnPathNet(idpathpair.first, closedList);
    }
  }
}
