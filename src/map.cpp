#include "map.hpp"
#include "tile.hpp"
#include "tiletype.hpp"
#include "buildingtype.hpp"
#include "building.hpp"
#include "pathtype.hpp"
#include "path.hpp"
#include "unittype.hpp"
#include "unit.hpp"
#include "tilemanager.hpp"
#include "buildingmanager.hpp"
#include "pathmanager.hpp"
#include "unitmanager.hpp"

Map::Map() :
  mName(""),
  mDescription(""),
  mRoofTexture(""),
  mWidth(0),
  mHeight(0),
  pTileManager(new TileManager()),
  pBuildingManager(new BuildingManager()),
  pPathManager(new PathManager()),
  pUnitManager(new UnitManager()),
  pPathFinder(new PathFinder())
{
}

Map::Map(const Map &obj) :
  mName(obj.mName),
  mDescription(obj.mDescription),
  mRoofTexture(obj.mRoofTexture),
  mWidth(obj.mWidth),
  mHeight(obj.mHeight),
  pTileManager(new TileManager(*obj.pTileManager)),
  pBuildingManager(new BuildingManager(*obj.pBuildingManager)),
  pPathManager(new PathManager(*obj.pPathManager)),
  pUnitManager(new UnitManager(*obj.pUnitManager)),
  pPathFinder(new PathFinder(*obj.pPathFinder))
{
}

Map::~Map() {
  std::clog << "Deleting map" << std::endl;
  delete pTileManager;
  delete pBuildingManager;
  delete pPathManager;
  delete pUnitManager;
  delete pPathFinder;
}

std::string Map::getName() const {
  return mName;
}

irr::scene::IMetaTriangleSelector* Map::getMetaTriangleSelectorP() const {
  return pTileManager->getTileSelector();
}


void Map::initialise(irr::video::IVideoDriver* driver, irr::scene::ISceneManager* smgr) {
  std::clog << "Initialising map '" << mName << "'" << std::endl;

  pPathFinder->initialise(mWidth, mHeight);

  pTileManager->initialise(smgr->getRootSceneNode(), pPathFinder);
  pBuildingManager->initialise(smgr->getRootSceneNode());
  pPathManager->initialise(smgr->getRootSceneNode());
  pUnitManager->initialise(smgr->getRootSceneNode());

  // Initialise buildings
  //  needs to be here, as need access to tile heights
  for (auto& building : pBuildingManager->getInstances()) {
    pBuildingManager->add(
      building.second.getTileNumber(),
      building.second.getTypeId(),
      pTileManager->getTileHeight(building.second.getTileNumber()),
      building.first
    );
  }

  // Initialise paths
  for (auto& path : pPathManager->getInstances()) {
    pPathManager->add(
      path.second.getTileNumber(),
      path.second.getTypeId(),
      pTileManager->getTileHeight(path.second.getTileNumber()),
      path.first
    );
  }
  // Ensure power is properly set up
  pPathManager->recalculatePower();

  // Initialise units
  for (auto& unit : pUnitManager->getInstances()) {
    pUnitManager->add(
      unit.second.getTypeId(),
      unit.second.getPosition(),
      unit.first
    );
  }

  pPathFinder->findPath (0.0f, 0.0f, 0.0f, 0.0f);
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

  // Store old surround for comparison
  oldSurround = pTileManager->getTileSurround(tileNumber);

  // Set and update the tile manager
  pTileManager->setTileTileType(tileNumber, tileType);
  pTileManager->recalculate(tileNumber, pPathFinder);

  // If we need to update the surrounding tile's information
  newSurround = pTileManager->getTileSurround(tileNumber);
  if (!(oldSurround == newSurround)) {
    recalculateSurroundingTileModels(tileNumber, enableCaveIn);
  }
}

// Serialisation
//  not as scary as it looks like
template<class Archive>
void Map::serialize(Archive & ar, const unsigned int version) {
  try {
    std::map<irr::u32, TileType> TileTypes = pTileManager->getTypes();
    std::map<irr::u32, Tile> Tiles = pTileManager->getInstances();

    std::map<irr::u32, BuildingType> BuildingTypes = pBuildingManager->getTypes();
    std::map<irr::u32, Building> Buildings = pBuildingManager->getInstances();

    std::map<irr::u32, PathType> PathTypes = pPathManager->getTypes();
    std::map<irr::u32, Path> Paths = pPathManager->getInstances();

    std::map<irr::u32, UnitType> UnitTypes = pUnitManager->getTypes();
    std::map<irr::u32, Unit> Units = pUnitManager->getInstances();

    ar & BOOST_SERIALIZATION_NVP(mName);
    ar & BOOST_SERIALIZATION_NVP(mDescription);
    ar & BOOST_SERIALIZATION_NVP(mRoofTexture);
    ar & BOOST_SERIALIZATION_NVP(mWidth);
    ar & BOOST_SERIALIZATION_NVP(mHeight);
    ar & BOOST_SERIALIZATION_NVP(TileTypes);
    ar & BOOST_SERIALIZATION_NVP(BuildingTypes);
    ar & BOOST_SERIALIZATION_NVP(PathTypes);
    ar & BOOST_SERIALIZATION_NVP(UnitTypes);
    ar & BOOST_SERIALIZATION_NVP(Buildings);
    ar & BOOST_SERIALIZATION_NVP(Paths);
    ar & BOOST_SERIALIZATION_NVP(Units);
    ar & BOOST_SERIALIZATION_NVP(Tiles);

    pTileManager->setTypes(TileTypes);
    pTileManager->setInstances(Tiles);
    pTileManager->setWidth(mWidth);
    pTileManager->setHeight(mHeight);
    pTileManager->setRoofTexture(mRoofTexture);

    pBuildingManager->setTypes(BuildingTypes);
    pBuildingManager->setInstances(Buildings);
    pBuildingManager->setWidth(mWidth);
    pBuildingManager->setHeight(mHeight);

    pPathManager->setTypes(PathTypes);
    pPathManager->setInstances(Paths);
    pPathManager->setWidth(mWidth);
    pPathManager->setHeight(mHeight);

    pUnitManager->setTypes(UnitTypes);
    pUnitManager->setInstances(Units);
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

// Recalculates everything to do with a specific tile
void Map::recalculateAll(const irr::u32 &tileNumber, const bool &enableCaveIn) {
  pTileManager->recalculate(tileNumber, pPathFinder);
  pBuildingManager->recalculateByTileNumber(tileNumber);
  pPathManager->recalculateByTileNumber(tileNumber);
}

// Add a new building
void Map::addBuilding(const irr::u32 &tileNumber, const irr::u32 &buildingType) {
  pBuildingManager->add(tileNumber, buildingType, pTileManager->getTileHeight(tileNumber));
}

// Remove a building by id
void Map::removeBuilding(const irr::u32 &buildingid) {
  pBuildingManager->remove(buildingid);
}

// Add a new path
void Map::addPath(const irr::u32 &tilenumber, const irr::u32 &pathtype, const irr::u32 &knownid) {
  // TODO adding paths
  pPathManager->add(
    tilenumber,
    pathtype,
    pTileManager->getTileHeight(tilenumber),
    knownid
  );
}

// Remove a path
void Map::removePath(const irr::u32& pathid) {
  pPathManager->remove(pathid);
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
