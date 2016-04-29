// Copyright (c) 2016 Eddie Ellams All Rights Reserved.
#ifndef _CONFIG_FILE_HPP
#define _CONFIG_FILE_HPP

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

#include "ConfigGraphics.hpp"

class ConfigFile {
public:
  ConfigFile() :
    GraphicsConfig() {}

  ConfigFile(int x, int y) :
    GraphicsConfig(x, y) {}

private:
  friend class boost::serialization::access;

  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    ar & BOOST_SERIALIZATION_NVP(GraphicsConfig);
  }

  ConfigGraphics GraphicsConfig;
};

#endif
