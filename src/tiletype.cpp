#include "tiletype.hpp"

TileType::TileType() :
  mName(""),
  mTexture(""),
  mSolid(false),
  mDrillTime(0),
  mMineInto(0),
  mMoveType(GROUND)
{
}

TileType::TileType(const TileType &obj) :
  mName(obj.mName),
  mTexture(obj.mTexture),
  mSolid(obj.mSolid),
  mDrillTime(obj.mDrillTime),
  mMineInto(obj.mMineInto),
  mMoveType(obj.mMoveType)
{
}

TileType::~TileType() {
  
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
