// Copyright (c) 2016 Eddie Ellams All Rights Reserved.
#ifndef _MAP_TYPE_HPP
#define _MAP_TYPE_HPP

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

#include <irrlicht.h>

class MapType {
public:

  enum MoveType {
    AIR = 0,
    GROUND = 1,
    WATER = 2
  };

  MapType();
  MapType(std::string t, bool s, irr::f32 dt, MoveType mt);

  // Getters
  bool getSolid() const;
  std::string getTextureName() const;
  irr::f32 getMineInto() const;

  // Setters

private:
  friend class boost::serialization::access;

  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    ar & BOOST_SERIALIZATION_NVP(mTexture);
    ar & BOOST_SERIALIZATION_NVP(mSolid);
    ar & BOOST_SERIALIZATION_NVP(mDrillTime);
    ar & BOOST_SERIALIZATION_NVP(mMineInto);
    ar & BOOST_SERIALIZATION_NVP(mMoveType);
  }

  std::string mTexture;
  bool mSolid;
  irr::f32 mDrillTime;
  irr::f32 mMineInto;

  MapType::MoveType mMoveType;
};

#endif
