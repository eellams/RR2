// Copyright (c) 2016 Eddie Ellams All Rights Reserved.
#ifndef _BUILDING_HPP
#define _BUILDING_HPP

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

#include <irrlicht.h>

#include "model.hpp"
#include "generic.hpp"

class BuildingType;

class Building {
public:
  Building();
  Building(const irr::u32 &bid, const irr::u32 &bt, const irr::u32 &tilenumber);
  Building(const Building &obj);
  ~Building();

  irr::u32 getBuildingId() const;
  irr::u32 getBuildingType() const;
  irr::u32 getTileNumber() const;

  void initialise(irr::scene::ISceneNode* parent, const BuildingType& btype, const irr::core::vector3df& pos);

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

  Model *pModel;
};

#endif
