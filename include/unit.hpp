#ifndef _UNIT_HPP
#define _UNIT_HPP

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

#include "generic.hpp"
#include <irrlicht.h>
#include <string>

class Model;

class Unit {
public:
  Unit();
  Unit(const Unit &obj);
  ~Unit();

  irr::u32 getId() const;
  irr::u32 getTypeId() const;
  irr::core::vector3df getPosition() const;

  void setId(const irr::u32 &id);
  void setTypeId(const irr::u32 &tid) ;

  void initialise(irr::scene::ISceneNode *parent, const irr::u32 &id, const irr::core::vector3df &pos, const Model *pmodel);

private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    irr::u32 X, Y, Z;
    X = mPos.X;
    Y = mPos.Y;
    Z = mPos.Z;

    ar & BOOST_SERIALIZATION_NVP(mUnitType);
    ar & BOOST_SERIALIZATION_NVP(X);
    ar & BOOST_SERIALIZATION_NVP(Y);
    ar & BOOST_SERIALIZATION_NVP(Z);

    mPos.X = X;
    mPos.Y = Y;
    mPos.Z = Z;
  }

  irr::u32 mUnitId;
  irr::u32 mUnitType;

  irr::core::vector3df mPos;

  Model *pModel;
};

#endif
