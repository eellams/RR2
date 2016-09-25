#include "map.hpp"
#include "tile.hpp"
#include "tiletype.hpp"
#include "buildingtype.hpp"
#include "building.hpp"
#include "pathtype.hpp"
#include "path.hpp"
#include "tilemanager.hpp"
#include "buildingmanager.hpp"

#include "itiledmanager.hpp"

#include "pathmanager.hpp"

Map::Map() :
  mName(""),
  mDescription(""),
  mRoofTexture(""),
  mWidth(0),
  mHeight(0),
  pTileManager(new TileManager()),
  pBuildingManager(new BuildingManager()),
  pPathManager(new PathManager())
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
  pPathManager(new PathManager(*obj.pPathManager))
{
}

Map::~Map() {

}

std::string Map::getName() const {
  return mName;
}

irr::scene::IMetaTriangleSelector* Map::getMetaTriangleSelectorP() const {
  return pTileManager->getTileSelector();
}


void Map::initialise(irr::video::IVideoDriver* driver, irr::scene::ISceneManager* smgr) {
  std::clog << "Initialising map '" << mName << "'" << std::endl;

  pTileManager->initialise(smgr->getRootSceneNode());
  pBuildingManager->initialise(smgr->getRootSceneNode());
  pPathManager->initialise(smgr->getRootSceneNode());

  // Initialise buildings
  //  needs to be here, as need access to tile heights
  for (auto& building : pBuildingManager->getInstances()) {
    pBuildingManager->add(
      building.second.getTileNumber(),
      building.second.getBuildingType(),
      pTileManager->getTileHeight(building.second.getTileNumber()),
      building.first
    );
  }

  for (auto& path : pPathManager->getInstances()) {
    pPathManager->add(
      path.second.getTileNumber(),
      path.second.getPathType(),
      pTileManager->getTileHeight(path.second.getTileNumber()),
      path.first
    );
  }
  pPathManager->recalculatePower();
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
    std::map<irr::u32, TileType> TileTypes = pTileManager->getTypes();
    //std::vector<Tile> Tiles = pTileManager->getInstances();
    std::map<irr::u32, Tile> Tiles = pTileManager->getInstances();

    std::map<irr::u32, BuildingType> BuildingTypes = pBuildingManager->getTypes();
    std::map<irr::u32, Building> Buildings = pBuildingManager->getInstances();

    std::map<irr::u32, PathType> PathTypes = pPathManager->getTypes();
    std::map<irr::u32, Path> Paths = pPathManager->getInstances();

    ar & BOOST_SERIALIZATION_NVP(mName);
    ar & BOOST_SERIALIZATION_NVP(mDescription);
    ar & BOOST_SERIALIZATION_NVP(mRoofTexture);
    ar & BOOST_SERIALIZATION_NVP(mWidth);
    ar & BOOST_SERIALIZATION_NVP(mHeight);
    ar & BOOST_SERIALIZATION_NVP(TileTypes);
    ar & BOOST_SERIALIZATION_NVP(BuildingTypes);
    ar & BOOST_SERIALIZATION_NVP(PathTypes);
    ar & BOOST_SERIALIZATION_NVP(Buildings);
    ar & BOOST_SERIALIZATION_NVP(Paths);
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
  pTileManager->recalculate(tileNumber);
  pBuildingManager->recalculateByTileNumber(tileNumber);
  pPathManager->recalculateByTileNumber(tileNumber);
}

void Map::addBuilding(const irr::u32 &tileNumber, const irr::u32 &buildingType) {
  pBuildingManager->add(tileNumber, buildingType, pTileManager->getTileHeight(tileNumber));
}

void Map::removeBuilding(const irr::u32 &buildingid) {
  pBuildingManager->remove(buildingid);
}

void Map::addPath(const irr::u32 &tilenumber, const irr::u32 &pathtype, const irr::u32 &knownid) {
  // TODO adding paths
  pPathManager->add(
    tilenumber,
    pathtype,
    pTileManager->getTileHeight(tilenumber),
    knownid
  );
}

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
