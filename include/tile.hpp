// Copyright (c) 2016 Eddie Ellams All Rights Reserved.
#ifndef _MAP_TILE_HPP
#define _MAP_TILE_HPP

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

#include <irrlicht.h>

#include "generic.hpp"

#include <iostream>
#include <utility>

class GeomObject;

class Tile {
public:
  Tile();
  Tile(const Tile &obj);
  ~Tile();

  // Getters
  irr::u32 getTileNumber()  const;
  irr::u32 getTypeId() const;

  irr::f32 getHeight() const;
  std::array<irr::f32, 4> getCornerHeights() const;
  irr::scene::ITriangleSelector* getTriangleSelector();

  // Setters
  void setParent(irr::scene::ISceneNode* parent);
  void setPosition(const irr::core::vector3df& position);
  void setTexture(const std::string& tex);
  void setDebug();
  void setCornerHeights(const std::array<irr::f32, 4> &cornerHeights);
  void setTypeId(const irr::u32 &tileType);
  void setTileNumber(const irr::u32 &tilenumber);

  bool createModel(const struct Surround s);
  void initialise(irr::scene::ISceneManager *pmanager, const irr::u32 &tilenumber, const struct Surround &tilesurround);

private:
  void createTile(const std::array< std::pair<bool,bool>, 4>& points, const irr::u32 noHigh, const bool invertHigh=false);

  friend class boost::serialization::access;
  template<class Archive>
  inline void serialize(Archive & ar, const unsigned int version)
  {
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
