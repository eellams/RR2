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

  MapType() :
    mTexture(""), mSolid(false), mDrillTime(0), mMoveType(GROUND) {}

  bool GetSolid() { return mSolid; }
  irr::video::ITexture* GetTexture() { return pTex; }

  void Initialise(irr::video::IVideoDriver* driver) {
    std::clog << "Loading tile: " << mTexture << std::endl;
    pTex = driver->addRenderTargetTexture(irr::core::dimension2d<irr::u32>(256,256), mTexture.c_str());
  }

private:
  friend class boost::serialization::access;

  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    ar & BOOST_SERIALIZATION_NVP(mTexture);
    ar & BOOST_SERIALIZATION_NVP(mSolid);
    ar & BOOST_SERIALIZATION_NVP(mDrillTime);
    ar & BOOST_SERIALIZATION_NVP(mMoveType);
  }

  std::string mTexture;
  bool mSolid;
  irr::f32 mDrillTime;

  MapType::MoveType mMoveType;

  irr::video::ITexture* pTex;
};

#endif
