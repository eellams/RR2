// Copyright (c) 2016 Eddie Ellams All Rights Reserved.
#ifndef _SAVE_FILE_HPP
#define _SAVE_FILE_HPP

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

#include "map.hpp"

class SaveFile {
public:
  SaveFile() : mMap() {}
  SaveFile(Map &m) : mMap(m) {}

  // Serialised values
  Map mMap;

private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    ar & BOOST_SERIALIZATION_NVP(mMap);
  }
};

#endif
