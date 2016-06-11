#include "maptype.hpp"

MapType::MapType() :
  mTexture(""), mSolid(false), mDrillTime(0), mMoveType(GROUND) {

  }

MapType::MapType(std::string t, bool s, irr::f32 dt, MoveType mt) :
  mTexture(t), mSolid(s), mDrillTime(dt), mMoveType(mt) {

  }

bool MapType::getSolid() {
  return mSolid;
}

std::string MapType::getTextureName() {
  return mTexture;
}
