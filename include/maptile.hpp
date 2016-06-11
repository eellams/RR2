// Copyright (c) 2016 Eddie Ellams All Rights Reserved.
#ifndef _MAP_TILE_HPP
#define _MAP_TILE_HPP

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

#include <irrlicht.h>

#include "geomobject.hpp"

#include <iostream>
#include <utility>

#define TILE_SIZE 10.0f
#define TILE_WALL_HEIGHT 12.5f
#define COLLISION_MASK_TILE 1 << 30

struct Surround {
  bool current, below, left, right, above;
};

class MapTile {
public:
  MapTile(size_t t=0) :
    mTileNumber(t), mHeight(0), mTileType(0) {}

  size_t GetTileNumber() const { return mTileNumber; }
  float GetHeight() const { return mHeight; }
  irr::u32 GetTileType() { return mTileType; }

  irr::scene::ITriangleSelector* GetTriangleSelector() { return mGo.GetTriangleSelector(); }

  void SetParent(irr::scene::ISceneNode* parent) { mGo.SetParent(parent); }
  void SetPosition(irr::core::vector3df position) { mGo.SetPosition(position); }
  void SetTexture(std::string tex) { mGo.SetTexture(tex); }
  void SetDebug() { mGo.SetDebug(); }

  void SetCornerHeights(const std::array<irr::f32, 4>& cornerHeights) { mCornerHeights = cornerHeights; }

  void CreateModel(struct Surround s);

private:
  void createTile(const std::array< std::pair<bool,bool>, 4>& points, const irr::u32 noHigh);

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

  GeomObject mGo;

  std::array<irr::f32, 4> mCornerHeights;
};

#endif
