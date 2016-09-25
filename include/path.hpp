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
  Path(const irr::u32 &pid, const irr::u32 &pt, const irr::u32 &tilenumber);
  Path(const Path &obj);

  ~Path();

  std::string getName() const;
  irr::u32 getPathType() const;
  irr::u32 getTileNumber() const;
  bool getPowered() const;

  void setTexture(const std::string& tex);
  void setAlpha();

  void initialise(irr::scene::ISceneNode* parent, const irr::u32 &pathid, const irr::core::vector3df &pos);
  void destroy();

  void createModel(const std::array<irr::f32, 4> cornerheights);
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
  bool mConductive;

  bool mConducting;

  GeomObject *pGeom;
};

#endif
