#ifndef _PATH_TYPE_HPP
#define _PATH_TYPE_HPP

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

#include <string>
#include <algorithm>

class PathType {
public:
  PathType();
  PathType(const PathType &obj);
  ~PathType();

  std::string getName() const;
  std::string getTextureName() const;
  std::string getTextureConductingName() const;
  bool getConductive() const;

  bool isAllowedTileType(const int &tileType);

private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    ar & BOOST_SERIALIZATION_NVP(mName);
    ar & BOOST_SERIALIZATION_NVP(mTexture);
    ar & BOOST_SERIALIZATION_NVP(mTextureConducting);
    ar & BOOST_SERIALIZATION_NVP(mConductive);
    ar & BOOST_SERIALIZATION_NVP(mAllowedTiles);
  }

  std::string mName;
  std::string mTexture;
  std::string mTextureConducting;
  bool mConductive;
  std::vector<int> mAllowedTiles;
};

#endif
