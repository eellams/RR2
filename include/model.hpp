#ifndef _MODEL_HPP
#define _MODEL_HPP

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

#include <map>
#include <vector>
#include "generic.hpp"

class Model {
public:
  Model();
  Model(const Model &obj);
  ~Model();

  void setPosition(const irr::core::vector3df &pos);
  void setPosition(const irr::f32 &x, const irr::f32 &y, const irr::f32 &z);

  std::string getModelPath() const;
  irr::core::vector3df getPosition() const;

  void initialise(irr::scene::ISceneNode* parent);

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
    //setPosition(mPosX, mPosY, mPosZ);
  }

  void setFlags();

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
