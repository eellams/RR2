#include "tiletype.hpp"

TileType::TileType() :
  mTexture(""),
  mSolid(false),
  mDrillTime(0),
  mMoveType(GROUND)
{
}


std::string TileType::getName() const {
  return mName;
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
