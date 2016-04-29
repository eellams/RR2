// Copyright (c) 2016 Eddie Ellams All Rights Reserved.
#ifndef _CONFIG_GRAPHICS_HPP
#define _CONFIG_GRAPHICS_HPP

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

class ConfigGraphics {
public:
  ConfigGraphics() :
    ResolutionX(800), ResolutionY(600) {}
    
  ConfigGraphics(int x, int y) :
    ResolutionX(x), ResolutionY(y) {}

private:
  friend class boost::serialization::access;

  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    ar & BOOST_SERIALIZATION_NVP(ResolutionX);
    ar & BOOST_SERIALIZATION_NVP(ResolutionY);
  }

  int ResolutionX;
  int ResolutionY;
};

#endif
