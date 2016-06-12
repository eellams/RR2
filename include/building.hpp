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
    mBuildingId(0), mBuildingType(0), mTileNumber(0), pMesh(NULL), pNode(NULL) {

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

  void initialise(irr::scene::ISceneNode* parent, std::string model, irr::core::vector3df& pos) {
    pMesh = parent->getSceneManager()->getMesh(model.c_str() );

    pNode = parent->getSceneManager()->addAnimatedMeshSceneNode(pMesh);
    pNode->setParent(parent);

    // TODO textures
    //  need to create a Model class which can be serialised
    //  including multiple models and textures
    //  big TODO
    //pNode->setMaterialTexture(0,parent->getSceneManager()->getVideoDriver()->getTexture(textureFile.c_str()));

    //pNode->setPosition(irr::core::vector3df((mTileX+0.5f)*TILE_SIZE, height, (mTileY+0.5f)*TILE_SIZE));
    pNode->setPosition(pos);
  }


private:
  friend class boost::serialization::access;

  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    // TODO should it be serialised using X,Y or just tileNumber?
    ar & BOOST_SERIALIZATION_NVP(mBuildingId);
    ar & BOOST_SERIALIZATION_NVP(mBuildingType);
    ar & BOOST_SERIALIZATION_NVP(mTileNumber);
  }

  irr::u32 mBuildingId;
  irr::u32 mBuildingType;
  irr::u32 mTileNumber;
  
  irr::scene::IAnimatedMesh* pMesh;
  irr::scene::IAnimatedMeshSceneNode* pNode;
};

#endif
