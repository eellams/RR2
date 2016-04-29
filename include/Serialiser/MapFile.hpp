// Copyright (c) 2016 Eddie Ellams All Rights Reserved.
#ifndef _MAP_FILE_HPP
#define _MAP_FILE_HPP

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

#include <vector>
#include <boost/serialization/vector.hpp>

#include "MapTile.hpp"
#include "MapType.hpp"

class MapFile {
public:
  MapFile() :
  name(""), description(""), width(0), height(0) {
    MapType ty;
    types.push_back(ty);

    MapTile t;
    tiles.push_back(t);
    tiles.push_back(t);
  }


  MapFile(std::string n, std::string d, size_t w, size_t h) :
    name(n), description(d), width(w), height(h) {}

private:
  friend class boost::serialization::access;

  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    ar & BOOST_SERIALIZATION_NVP(name);
    ar & BOOST_SERIALIZATION_NVP(width);
    ar & BOOST_SERIALIZATION_NVP(height);
    ar & BOOST_SERIALIZATION_NVP(types);
    ar & BOOST_SERIALIZATION_NVP(tiles);
  }

  std::string name;
  std::string description;
  size_t width;
  size_t height;
  std::vector<MapType> types;
  std::vector<MapTile> tiles;
};

#endif
