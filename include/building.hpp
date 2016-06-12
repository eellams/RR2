// Copyright (c) 2016 Eddie Ellams All Rights Reserved.
#ifndef _BUILDING_HPP
#define _BUILDING_HPP

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

#include <irrlicht.h>

#include "generic.hpp"

class Building {
public:
  Building() :
    mBuildingId(0), mBuildingType(0), mTileX(0), mTileY(0) {

    }

private:
  friend class boost::serialization::access;

  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    ar & BOOST_SERIALIZATION_NVP(mBuildingId);
    ar & BOOST_SERIALIZATION_NVP(mBuildingType);
    ar & BOOST_SERIALIZATION_NVP(mTileX);
    ar & BOOST_SERIALIZATION_NVP(mTileY);
  }

  irr::u32 mBuildingId;
  irr::u32 mBuildingType;
  irr::u32 mTileX;
  irr::u32 mTileY;
};

#endif
