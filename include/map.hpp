// Copyright (c) 2016 Eddie Ellams All Rights Reserved.
#ifndef _MAP_MAP_HPP
#define _MAP_MAP_HPP

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

#include <vector>
#include <array>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>

#include "generic.hpp"

class Tile;
class TileType;
class Building;
class BuildingType;
class Path;
class PathType;

class Map {
public:
  Map();
  ~Map();

  // Getters
  std::string getName() const;
  irr::scene::IMetaTriangleSelector* getMetaTriangleSelectorP() const;

  // Setters

  void initialise(irr::video::IVideoDriver* driver, irr::scene::ISceneManager* smgr);
  void mineTile(const irr::u32& tileNumber);

  void setTile(const irr::u32& tileNumber, const irr::u32& tileType, const bool& enableCaveIn = true);

private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);

  // Initialise tile types
  //  e.g. load textures, etc.
  void initialiseTileTypes();
  void initialiseBuildingTypes();
  void initialisePathTypes();

  // Initialise tiles
  //  e.g. initialise tile models
  //  TODO probably add a lot in here...
  void initialiseTiles(irr::scene::ISceneNode* parentNode);
  void initialiseBuildings(irr::scene::ISceneNode* parentNode);
  void initialisePaths(irr::scene::ISceneNode* parentNode);

  // The heightmap is not serialised, as each Tile has a height value
  //  however, it is useful to have access to a dedicated heightmap
  void createHeightMap();
  void calculateTileCorners(const irr::u32& tileNumber);
  struct Surround calculateSurround(const irr::u32& tileNumber);

  void recalculateAll(const irr::u32& tileNumber, const bool &enableCaveIn = true);
  void recalculateTile(const irr::u32& tileNumber, const bool& enableCaveIn = true);
  void recalculateBuilding(const irr::u32& tileNumber);
  void recalculatePath(const irr::u32& tileNumber);

  void addBuilding(const irr::u32& tileNumber, const irr::u32& buildingType) {
    // TODO adding buildings
  }
  void addPath(const irr::u32& tileNumber, const irr::u32& pathType) {
    // TODO adding paths
  }


  void removeBuilding(const irr::u32& buildingId);
  void removePath(const irr::u32& pathId);

  irr::core::vector3df tileNumberToPosition(const int& tilenumber);

  void recalculateSurroundingTileModels(const int& tileNumber, const bool& enableCaveIn);

  // Serialised values
  std::string mName;
  std::string mDescription;
  std::string mRoofTexture;
  size_t mWidth;
  size_t mHeight;
  std::map<irr::u32, TileType> mTileTypes;
  std::map<irr::u32, BuildingType> mBuildingTypes;
  std::map<irr::u32, Building> mBuildings;
  std::map<irr::u32, PathType> mPathTypes;
  std::map<irr::u32, Path> mPaths;
  std::vector<Tile> mTiles;

  // Non-serialised values
  std::vector<float> mHeightmap;
  irr::scene::ISceneNode* pTileNode;
  irr::scene::ISceneNode* pBuildingNode;
  irr::scene::ISceneNode* pPathNode;
  irr::scene::IMetaTriangleSelector* pTileSelector;
};

#endif
