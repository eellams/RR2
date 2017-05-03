// Copyright (c) 2016 Eddie Ellams All Rights Reserved.
#ifndef _BUILDING_TYPE_HPP
#define _BUILDING_TYPE_HPP

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

#include "model.hpp"

#include <irrlicht.h>

class Model;

class BuildingType {
public:
  BuildingType();
  BuildingType(const BuildingType &obj);
  ~BuildingType();

  std::string getName() const;
  Model* getPModel();

private:
  friend class boost::serialization::access;

  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    ar & BOOST_SERIALIZATION_NVP(mName);
    ar & BOOST_SERIALIZATION_NVP(mModel);
  }

  std::string mName;
  Model mModel;
};

#endif
