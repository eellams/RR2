// Copyright (c) 2016 Eddie Ellams All Rights Reserved.
#ifndef _MAP_MAP_HPP
#define _MAP_MAP_HPP

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

#include <vector>
#include <array>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>

#include "maptile.hpp"
#include "maptype.hpp"

class Map {
public:
  Map() :
    mName(""), mDescription(""), mWidth(0), mHeight(0), pNode(NULL) {}

  Map(std::string n, std::string d, size_t w, size_t h) :
    mName(n), mDescription(d), mWidth(w), mHeight(h), pNode(NULL) {}

  std::string GetName() const { return mName; }

  void Initialise(irr::video::IVideoDriver* driver, irr::scene::ISceneManager* smgr);

private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & BOOST_SERIALIZATION_NVP(mName);
    ar & BOOST_SERIALIZATION_NVP(mDescription);
    ar & BOOST_SERIALIZATION_NVP(mRoofTexture);
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
  void initialiseTiles(irr::video::IVideoDriver* driver, irr::scene::ISceneManager* smgr);

  // The heightmap is not serialised, as each Tile has a height value
  //  however, it is useful to have access to a dedicated heightmap
  void createHeightMap();

  void calculateTileCorners(irr::u32 tileNumber);

  struct Surround calculateSurround(irr::u32 tileNumber);

  // Serialised values
  std::string mName;
  std::string mDescription;
  std::string mRoofTexture;
  size_t mWidth;
  size_t mHeight;
  std::map<irr::u32, MapType> mTypes;
  std::vector<MapTile> mTiles;

  // Non-serialised values
  std::vector<float> mHeightmap;
  irr::scene::ISceneNode* pNode;
};

#endif
