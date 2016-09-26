#ifndef _UNIT_TYPE_HPP
#define _UNIT_TYPE_HPP

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

#include <irrlicht.h>
#include <string>

class Model;

class UnitType {
public:
  UnitType();
  UnitType(const UnitType &obj);
  ~UnitType();

  std::string getName() const;
  Model* getPModel() const;

private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & BOOST_SERIALIZATION_NVP(mName);
    ar & BOOST_SERIALIZATION_NVP(pModel);
  }

  std::string mName;
  Model *pModel;
};

#endif
