// Copyright (c) 2016 Eddie Ellams All Rights Reserved.
#ifndef _BUILDING_TYPE_HPP
#define _BUILDING_TYPE_HPP

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

#include <irrlicht.h>

#include "model.hpp"
#include "generic.hpp"

class BuildingType {
public:
  BuildingType() :
    mName(""), mModel() {}

  //std::string getModel() const { return mModel; }
  //std::string getModel() const { return mModel.getModel(); }
  Model getModel() const { return mModel; }
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
