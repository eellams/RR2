#ifndef _MODEL_HPP
#define _MODEL_HPP

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

#include <map>
#include <vector>
#include "generic.hpp"

class Model {
public:
  Model() :
    mModelPath(""), mPosX(0), mPosY(0), mPosZ(0), mTextures(), mChildren(),
    pMesh(NULL), pNode(NULL) {}

  ~Model() {
    if (pMesh != NULL) pMesh->drop();
    if (pNode != NULL) pNode->drop();
  }


  void setPosition(irr::core::vector3df pos) {
    mPos = pos;
    if (pNode != NULL) pNode->setPosition(pos);
   }
  void setPosition(irr::f32 x, irr::f32 y, irr::f32 z) {
    setPosition(irr::core::vector3df(x,y,z));
  }

  std::string getModelPath() const { return mModelPath; }
  irr::core::vector3df getPosition() const { return mPos; }

  void initialise(irr::scene::ISceneNode* parent) {
    pMesh = parent->getSceneManager()->getMesh(mModelPath.c_str());

    pNode = parent->getSceneManager()->addAnimatedMeshSceneNode(pMesh);
    pNode->setParent(parent);
    pNode->setPosition(mPos);

    for (auto texture : mTextures) {
      pNode->setMaterialTexture(
        texture.first,
        parent->getSceneManager()->getVideoDriver()->getTexture(texture.second.c_str()));
    }

    setFlags();
  }

private:
  friend class boost::serialization::access;

  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    // TODO serialise vectors properly

    // Ensure serialised values correct
    //  (when writing to archive)
    mPosX = mPos.X;
    mPosY = mPos.Y;
    mPosZ = mPos.Z;

    ar & BOOST_SERIALIZATION_NVP(mModelPath);
    ar & BOOST_SERIALIZATION_NVP(mPosX);
    ar & BOOST_SERIALIZATION_NVP(mPosY);
    ar & BOOST_SERIALIZATION_NVP(mPosZ);
    ar & BOOST_SERIALIZATION_NVP(mTextures);
    ar & BOOST_SERIALIZATION_NVP(mChildren);

    // Ensure position correct
    //  (when reading from archive)
    setPosition(mPosX, mPosY, mPosZ);
  }

  void setFlags() {
    pNode->setMaterialFlag(irr::video::EMF_LIGHTING, true);
    pNode->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);
  }

  // Serialised
  std::string mModelPath;
  irr::f32 mPosX;
  irr::f32 mPosY;
  irr::f32 mPosZ;
  std::map<irr::u32, std::string> mTextures;
  std::vector<Model> mChildren;

  irr::core::vector3df mPos;
  irr::scene::IAnimatedMesh* pMesh;
  irr::scene::IAnimatedMeshSceneNode* pNode;
};

#endif
