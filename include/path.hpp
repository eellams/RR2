// Copyright (c) 2016 Eddie Ellams All Rights Reserved.
#ifndef _PATH_HPP
#define _PATH_HPP

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

#include <irrlicht.h>

#include "generic.hpp"

class GeomObject;
class PathType;

class Path {
public:
  Path();
  //Path(const Path &obj);
  ~Path();

  irr::u32 getPathType() const;
  irr::u32 getTileNumber() const;
  bool getPowered() const;

  void setCornerHeights(const std::array<irr::f32, 4>& cornerHeights);
  void setTexture(const std::string& tex);
  void setAlpha();

  void initialise(irr::scene::ISceneNode* parent, const PathType& ptype, const irr::core::vector3df& pos);
  void destroy();

  void createModel();

  void turnOff();
  void turnOn();

private:
  friend class boost::serialization::access;

  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    // TODO should it be serialised using X,Y or just tileNumber?
    ar & BOOST_SERIALIZATION_NVP(mPathType);
    ar & BOOST_SERIALIZATION_NVP(mTileNumber);
    ar & BOOST_SERIALIZATION_NVP(mPowered);
  }

  irr::u32 mPathId;
  irr::u32 mPathType;
  irr::u32 mTileNumber;
  bool mPowered;

  bool mConducting;
  std::array<irr::f32, 4> mCornerHeights;

  GeomObject *pGeom;
};

#endif
