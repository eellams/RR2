// Copyright (c) 2016 Eddie Ellams All Rights Reserved.
#ifndef _MAP_TILE_HPP
#define _MAP_TILE_HPP

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

class MapTile {
public:
  MapTile() :
    tileNumber(0), tileType(0) {}

  MapTile(size_t t) :
    tileNumber(t) {}

private:
  friend class boost::serialization::access;

  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    ar & BOOST_SERIALIZATION_NVP(tileNumber);
    ar & BOOST_SERIALIZATION_NVP(tileType);
  }

  size_t tileNumber;
  size_t tileType;
};

#endif
