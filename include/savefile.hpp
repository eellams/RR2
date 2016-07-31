// Copyright (c) 2016 Eddie Ellams All Rights Reserved.
#ifndef _SAVE_FILE_HPP
#define _SAVE_FILE_HPP

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

//#include "map.hpp"
#include "generic.hpp"

class Map;

class SaveFile {
public:
  SaveFile() {}

  // Serialised values
  // TODO this breaks encapsulation!

  Map* getPMap();

  void setPMap(Map* map);

private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);

  Map *pMap;
};

#endif
