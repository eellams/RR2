// Copyright (c) 2016 Eddie Ellams All Rights Reserved.
#ifndef _MAP_TILE_HPP
#define _MAP_TILE_HPP

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

#include <irrlicht.h>

//#include "geomobject.hpp"
#include "generic.hpp"

#include <iostream>
#include <utility>

class GeomObject;

class Tile {
public:
  Tile(size_t t=0);
  ~Tile();

  // Getters
  irr::u32 getTileNumber()  const;
  irr::f32 getHeight() const;
  irr::f32 getCornerHeightMax() const;
  irr::u32 getTileType() const;
  irr::scene::ITriangleSelector* getTriangleSelector();

  inline std::array<irr::f32, 4> getCornerHeights() const {
    return mCornerHeights;
  }

  // Setters
  void setParent(irr::scene::ISceneNode* parent);
  void setPosition(const irr::core::vector3df& position);
  void setTexture(const std::string& tex);
  void setDebug();
  void setCornerHeights(const std::array<irr::f32, 4>& cornerHeights);
  void setTileType(const irr::u32& tileType);

  bool createModel(const struct Surround s);
  void initialise(irr::scene::ISceneManager* pmanager, const struct Surround& tilesurround);

private:
  void createTile(const std::array< std::pair<bool,bool>, 4>& points, const irr::u32 noHigh, const bool invertHigh=false);

  friend class boost::serialization::access;
  template<class Archive>
  inline void serialize(Archive & ar, const unsigned int version)
  {
    ar & BOOST_SERIALIZATION_NVP(mTileNumber);
    ar & BOOST_SERIALIZATION_NVP(mTileType);
    ar & BOOST_SERIALIZATION_NVP(mHeight);
  }

  irr::u32 mTileNumber;
  irr::u32 mTileType;
  irr::f32 mHeight;

  std::array<irr::f32, 4> mCornerHeights;

  GeomObject *pGeom;

};

#endif
