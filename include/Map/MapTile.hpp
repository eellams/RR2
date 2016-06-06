// Copyright (c) 2016 Eddie Ellams All Rights Reserved.
#ifndef _MAP_TILE_HPP
#define _MAP_TILE_HPP

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

#include <irrlicht.h>

#include "Map/MapStructs.hpp"
#include "Map/MapType.hpp"
#include "GeomObject.hpp"

#include <iostream>

class MapTile : public GeomObject {
public:
  MapTile(size_t t=0) :
    mTileNumber(t), mHeight(0), mTileType(0) {}

  size_t GetTileNumber() const { return mTileNumber; }
  float GetHeight() const { return mHeight; }
  irr::u32 GetTileType() { return mTileType; }

  void CreateModel(struct Surround s);

  void SetCornerHeights(std::array<irr::f32, 4> cornerHeights) {
    mCornerHeights = cornerHeights;
  }

private:
  // For a [peak at] below left outward corner
  //  points[0] BL (oeak)
  //  points[1] AL
  //  points[2] AR
  //  points[3] BR
  void createInwardCorner(std::array<irr::core::vector3df, 4> points);

  void createFlat();

  friend class boost::serialization::access;

  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    ar & BOOST_SERIALIZATION_NVP(mTileNumber);
    ar & BOOST_SERIALIZATION_NVP(mTileType);
    ar & BOOST_SERIALIZATION_NVP(mHeight);
  }

  irr::u32 mTileNumber;
  irr::u32 mTileType;
  irr::f32 mHeight;

  //irr::f32 mCornerHeights[4];
  std::array<irr::f32, 4> mCornerHeights;
};

#endif
