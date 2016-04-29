// Copyright (c) 2016 Eddie Ellams All Rights Reserved.
#ifndef _MAP_TYPE_HPP
#define _MAP_TYPE_HPP

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

class MapType {
public:

  enum MoveType {
    AIR = 0,
    GROUND = 1,
    WATER = 2
  };

  MapType() :
    typeNumber(0), texture(""), solid(false), drillTime(0), moveType(GROUND) {}
  /*MapTypes(int t, std::string tex, ) :
    tileNumber(t) {}*/

private:
  friend class boost::serialization::access;

  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    ar & BOOST_SERIALIZATION_NVP(typeNumber);
    ar & BOOST_SERIALIZATION_NVP(texture);
    ar & BOOST_SERIALIZATION_NVP(solid);
    ar & BOOST_SERIALIZATION_NVP(drillTime);
    ar & BOOST_SERIALIZATION_NVP(moveType);
  }

  size_t typeNumber;
  std::string texture;
  bool solid;
  float drillTime;

  MapType::MoveType moveType;
};

#endif
