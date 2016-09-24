#ifndef _PATH_TYPE_HPP
#define _PATH_TYPE_HPP

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

#include <string>
#include <algorithm>

class PathType {
public:
  PathType();

  std::string getName() const { return mName; }
  std::string getTextureName() const { return mTexture; }
  std::string getTextureConductingName() const { return mTextureConducting; }
  bool getConductive() const { return mConductive; }
  bool isAllowedTileType(const int &tileType) {
    //return mAllowedTiles.contains(tileType);
    return std::find(mAllowedTiles.begin(), mAllowedTiles.end(), tileType) != mAllowedTiles.end();
  }

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
