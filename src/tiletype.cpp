#include "tiletype.hpp"

TileType::TileType() :
  mTexture(""), mSolid(false), mDrillTime(0), mMoveType(GROUND) {

  }

TileType::TileType(std::string t, bool s, irr::f32 dt, MoveType mt) :
  mTexture(t), mSolid(s), mDrillTime(dt), mMoveType(mt) {

  }

bool TileType::getSolid() const {
  return mSolid;
}

std::string TileType::getTextureName() const {
  return mTexture;
}

irr::f32 TileType::getMineInto() const {
  return mMineInto;
}
