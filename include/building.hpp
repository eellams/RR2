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
    mBuildingId(0), mBuildingType(0), mTileX(0), mTileY(0), pMesh(NULL), pNode(NULL) {

    }

  ~Building() {
    if (pMesh != NULL) pMesh->drop();
    if (pNode != NULL) pNode->drop();
  }

  void setTileNumber(irr::u32 tileNumber) {
    mTileNumber = tileNumber;
  }

  irr::u32 getBuildingType() const { return mBuildingType; }
  irr::u32 getTileNumber() const { return mTileNumber; }
  irr::u32 getTileX() const { return mTileX; }
  irr::u32 getTileY() const { return mTileY; }

  void initialise(irr::scene::ISceneNode* parent, std::string model, irr::f32 height) {
    pMesh = parent->getSceneManager()->getMesh(model.c_str());

    pNode = parent->getSceneManager()->addAnimatedMeshSceneNode(pMesh);
    pNode->setParent(parent);

    pNode->setPosition(irr::core::vector3df((mTileX+0.5f)*TILE_SIZE, height, (mTileY+0.5f)*TILE_SIZE));
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

  irr::u32 mTileNumber;
  irr::scene::IAnimatedMesh* pMesh;
  irr::scene::IAnimatedMeshSceneNode* pNode;
};

#endif
