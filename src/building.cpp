#include "building.hpp"
#include "buildingtype.hpp"

Building::Building() :
  mBuildingId(0),
  mBuildingType(0),
  mTileNumber(0),
  pModel(NULL)
{
}

Building::Building(const irr::u32 &bid, const irr::u32 &bt, const irr::u32 &tilenumber) :
  mBuildingId(bid),
  mBuildingType(bt),
  mTileNumber(tilenumber),
  pModel(NULL)
{
}

Building::Building(const Building &obj) :
  mBuildingId(obj.mBuildingId),
  mBuildingType(obj.mBuildingType),
  mTileNumber(obj.mTileNumber),
  pModel(NULL)
{
}

Building::~Building() {
  std::clog << "Deleting building " << mBuildingId << std::endl;
  if (pModel != NULL) delete pModel;
}

irr::u32 Building::getBuildingId() const {
  return mBuildingId;
}

irr::u32 Building::getBuildingType() const {
  return mBuildingType;
}

irr::u32 Building::getTileNumber() const {
  return mTileNumber;
}

void Building::setBuildingId(const irr::u32 &buildingid) {
  mBuildingId = buildingid;
}

void Building::initialise(irr::scene::ISceneNode *parent, const irr::u32 &buildingid, const irr::core::vector3df &pos, const Model &model) {
  std::clog << "Initalising building " << buildingid << std::endl;
  mBuildingId = buildingid;
  pModel = new Model(model);
  pModel->initialise(parent);
  pModel->setPosition(pos);
}
