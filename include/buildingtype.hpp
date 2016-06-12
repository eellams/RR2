// Copyright (c) 2016 Eddie Ellams All Rights Reserved.
#ifndef _BUILDING_TYPE_HPP
#define _BUILDING_TYPE_HPP

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

#include <irrlicht.h>

#include "generic.hpp"

class BuildingType {
public:
  BuildingType() :
    mName("") {

    }

private:
  friend class boost::serialization::access;

  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    ar & BOOST_SERIALIZATION_NVP(mName);
  }

  std::string mName;
};

#endif
