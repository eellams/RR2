// Copyright (c) 2016 Eddie Ellams All Rights Reserved.
#ifndef _MAP_LOADER_HPP
#define _MAP_LOADER_HPP

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

class ToSerialise {
public:
  ToSerialise() :
    degrees(0), minutes(0), seconds(0) {}
    
  ToSerialise(int d, int m, float s) :
    degrees(d), minutes(m), seconds(s) {}

private:
  friend class boost::serialization::access;

  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    ar & BOOST_SERIALIZATION_NVP(degrees);
    ar & BOOST_SERIALIZATION_NVP(minutes);
    ar & BOOST_SERIALIZATION_NVP(seconds);
  }

  int degrees;
  int minutes;
  float seconds;

};

#endif
