// Copyright (c) 2016 Eddie Ellams All Rights Reserved.
#ifndef _BUILDING_HPP
#define _BUILDING_HPP

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

#include <irrlicht.h>

#include "model.hpp"
#include "generic.hpp"

class Building {
public:
  inline Building() :
    mBuildingId(0), mBuildingType(0), mTileNumber(0) {

    }

  inline Building(const irr::u32 &bid, const irr::u32 &bt, const irr::u32 &tilenumber) :
    mBuildingId(bid),
    mBuildingType(bt),
    mTileNumber(tilenumber)
  {

  }

  ~Building() {
    
  }

  irr::u32 getBuildingId() const { return mBuildingId; }
  irr::u32 getBuildingType() const { return mBuildingType; }
  irr::u32 getTileNumber() const { return mTileNumber; }

  void initialise(irr::scene::ISceneNode* parent, const BuildingType& btype, const irr::core::vector3df& pos) {
    mModel = btype.getModel();
    mModel.initialise(parent);
    mModel.setPosition(pos);
  }

private:
  friend class boost::serialization::access;

  template<class Archive>
  inline void serialize(Archive & ar, const unsigned int version)
  {
    // TODO should it be serialised using X,Y or just tileNumber?
    ar & BOOST_SERIALIZATION_NVP(mBuildingId);
    ar & BOOST_SERIALIZATION_NVP(mBuildingType);
    ar & BOOST_SERIALIZATION_NVP(mTileNumber);
  }

  irr::u32 mBuildingId;
  irr::u32 mBuildingType;
  irr::u32 mTileNumber;

  Model mModel;
};

#endif
