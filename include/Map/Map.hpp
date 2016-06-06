// Copyright (c) 2016 Eddie Ellams All Rights Reserved.
#ifndef _MAP_MAP_HPP
#define _MAP_MAP_HPP

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

#include <vector>
#include <array>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>

#include "Map/MapTile.hpp"
#include "Map/MapType.hpp"

class Map {
public:
  Map::Map() :
    mName(""), mDescription(""), mWidth(0), mHeight(0) {}

  Map::Map(std::string n, std::string d, size_t w, size_t h) :
    mName(n), mDescription(d), mWidth(w), mHeight(h) {}

  std::string GetName() const { return mName; }

  // Recaclculate a specific tile's model
  void CalculateTileModel(irr::u32 tileNumber) {
    mTiles[tileNumber].CreateModel(calculateSurround(tileNumber));
  }

  void Initialise(irr::video::IVideoDriver* driver, irr::scene::ISceneManager* smgr);
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & BOOST_SERIALIZATION_NVP(mName);
    ar & BOOST_SERIALIZATION_NVP(mWidth);
    ar & BOOST_SERIALIZATION_NVP(mHeight);
    ar & BOOST_SERIALIZATION_NVP(mTypes);
    ar & BOOST_SERIALIZATION_NVP(mTiles);
  }

  // Initialise tile types
  //  e.g. load textures, etc.
  void initialiseTileTypes(irr::video::IVideoDriver* driver);

  // Initialise tiles
  //  e.g. initialise tile models
  //  TODO probably add a lot in here...
  void initialiseTiles(irr::scene::ISceneManager *smgr);

  // The heightmap is not serialised, as each Tile has a height value
  //  however, it is useful to have access to a dedicated heightmap
  void createHeightMap();

  void calculateTileCorners(irr::u32 tileNumber);

  struct Surround calculateSurround(irr::u32 tileNumber);

  // Serialised values
  std::string mName;
  std::string mDescription;
  size_t mWidth;
  size_t mHeight;
  //std::vector<MapType> mTypes;
  std::map<irr::u32, MapType> mTypes;
  std::vector<MapTile> mTiles;

  //irr::scene::IMeshSceneNode* pTileNode;

  // Non-serialised values
  std::vector<float> mHeightmap;
};

#endif
