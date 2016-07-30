#ifndef _PATH_TYPE_HPP
#define _PATH_TYPE_HPP

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

#include <string>
#include <algorithm>

class PathType {
public:
  PathType();

  std::string getTextureName() const { return mTexture; }
  bool isAllowedTileType(const int &tileType) {
    //return mAllowedTiles.contains(tileType);
    return std::find(mAllowedTiles.begin(), mAllowedTiles.end(), tileType) != mAllowedTiles.end();
  }

private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    ar & BOOST_SERIALIZATION_NVP(mTexture);
    ar & BOOST_SERIALIZATION_NVP(mAllowedTiles);
  }

  std::string mTexture;
  std::vector<int> mAllowedTiles;
};

#endif
