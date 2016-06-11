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

  inline bool operator==(Surround a) {
    if (a.current==current && a.below==below && a.left==left && a.right==right && a.above==above)
      return true;
    else
      return false;
  }
};

class MapTile {
public:
  MapTile(size_t t=0);

  // Getters
  irr::u32 getTileNumber()  const;
  irr::f32 getHeight() const;
  irr::u32 getTileType() const;
  irr::scene::ITriangleSelector* getTriangleSelector();
  struct Surround getPrevSurround();

  // Setters
  void setParent(irr::scene::ISceneNode* parent);
  void setPosition(const irr::core::vector3df& position);
  void setTexture(const std::string& tex);
  void setDebug();
  void setCornerHeights(const std::array<irr::f32, 4>& cornerHeights);
  void setTileType(const irr::u32& tileType);

  void createModel(const struct Surround s);

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
  std::array<irr::f32, 4> mCornerHeights;
  struct Surround mPrevSurround;

  GeomObject mGo;

};

#endif
