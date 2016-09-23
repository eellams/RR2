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

class TileManager;
class BuildingManager;

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

  void addBuilding(const irr::u32& tileNumber, const irr::u32& buildingType);

  void removeBuilding(const irr::u32 &buildingid);

private:
  friend class boost::serialization::access;
  friend class BuildingManager;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);

  // Initialise tile types
  //  e.g. load textures, etc.
  //void initialiseTileTypes();
  //void initialiseBuildingTypes();
  void initialisePathTypes();

  // Initialise tiles
  //  e.g. initialise tile models
  //  TODO probably add a lot in here...
  //void initialiseTiles(irr::scene::ISceneNode* parentNode);
  //void initialiseBuildings(irr::scene::ISceneNode* parentNode);
  void initialisePaths(irr::scene::ISceneNode* parentNode);

  void recalculateAll(const irr::u32& tileNumber, const bool &enableCaveIn = true);
  void recalculatePath(const irr::u32& tileNumber);

  //void addBuilding(const irr::u32& tileNumber, const irr::u32& buildingType);
  void addPath(const irr::u32& tileNumber, const irr::u32& pathType);
  void removePath(const irr::u32& pathId);

  std::vector<irr::u32> getSurroundingTileNumbers(const irr::u32& tileNumber);
  void recalculateSurroundingTileModels(const int& tileNumber, const bool& enableCaveIn);

  void setPathConducting(const irr::u32 &pathid);
  void clearPathConducting(const irr::u32 &pathid);
  void recalculatePathPower();
  void turnOnPathNet(const int pathId, std::vector<int> &checkedList);

  // Serialised values
  std::string mName;
  std::string mDescription;
  std::string mRoofTexture;
  size_t mWidth;
  size_t mHeight;
  std::map<irr::u32, PathType> mPathTypes;
  std::map<irr::u32, Path> mPaths;

  TileManager *pTileManager;
  BuildingManager *pBuildingManager;
};

#endif
