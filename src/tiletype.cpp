#include "tiletype.hpp"

TileType::TileType() :
  mName(""),
  mTexture(""),
  mSolid(false),
  mDrillTime(0),
  mMineInto(0),
  mMoveSpeeds()
{
}

TileType::TileType(const TileType &obj) :
  mName(obj.mName),
  mTexture(obj.mTexture),
  mSolid(obj.mSolid),
  mDrillTime(obj.mDrillTime),
  mMineInto(obj.mMineInto),
  mMoveSpeeds(obj.mMoveSpeeds)
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

std::map<irr::u32, irr::f32> TileType::getMoveSpeed() {
  return mMoveSpeeds;
}
