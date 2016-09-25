#include "pathtype.hpp"

PathType::PathType() :
  mName(""),
  mTexture(""),
  mTextureConducting(""),
  mConductive(false),
  mAllowedTiles()
{
}

PathType::PathType(const PathType &obj) :
  mName(obj.mName),
  mTexture(obj.mTexture),
  mTextureConducting(obj.mTextureConducting),
  mConductive(obj.mConductive),
  mAllowedTiles(obj.mAllowedTiles)
{
}

PathType::~PathType() {

}

std::string PathType::getName() const {
  return mName;
}

std::string PathType::getTextureName() const {
  return mTexture;
}

std::string PathType::getTextureConductingName() const {
  return mTextureConducting;
}

bool PathType::getConductive() const {
  return mConductive;
}

bool PathType::isAllowedTileType(const int &tileType) {
  return std::find(mAllowedTiles.begin(), mAllowedTiles.end(), tileType) != mAllowedTiles.end();
}
