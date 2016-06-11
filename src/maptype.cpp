#include "maptype.hpp"

MapType::MapType() :
  mTexture(""), mSolid(false), mDrillTime(0), mMoveType(GROUND) {

  }

MapType::MapType(std::string t, bool s, irr::f32 dt, MoveType mt) :
  mTexture(t), mSolid(s), mDrillTime(dt), mMoveType(mt) {

  }

bool MapType::getSolid() const {
  return mSolid;
}

std::string MapType::getTextureName() const {
  return mTexture;
}

irr::f32 MapType::getMineInto() const {
  return mMineInto;
}
