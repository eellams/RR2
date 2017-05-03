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
class Unit;
class UnitType;

class TileManager;
class BuildingManager;
class PathManager;
class UnitManager;
class PathFinder;

class Map {
public:
  Map();
  Map(const Map &obj);
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

  void addPath(const irr::u32 &tilenumber, const irr::u32 &pathtype, const irr::u32 &knownid=0);
  void removePath(const irr::u32 &pathid);

private:
  friend class boost::serialization::access;
  friend class BuildingManager;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);

  void recalculateAll(const irr::u32& tileNumber, const bool &enableCaveIn = true);
  void recalculatePath(const irr::u32& tileNumber);
  void recalculateSurroundingTileModels(const int& tileNumber, const bool& enableCaveIn);

  // Serialised values
  std::string      mName;
  std::string      mDescription;
  std::string      mRoofTexture;
  size_t           mWidth;
  size_t           mHeight;

  TileManager      *pTileManager;
  BuildingManager  *pBuildingManager;
  PathManager      *pPathManager;
  UnitManager      *pUnitManager;
  PathFinder       *pPathFinder;
};

#endif
