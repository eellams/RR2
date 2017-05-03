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
  if (pModel != NULL) delete pModel;
}

irr::u32 Building::getTypeId() const {
  return mBuildingType;
}

irr::u32 Building::getTileNumber() const {
  return mTileNumber;
}

void Building::setId(const irr::u32 &id) {
  mBuildingId = id;
}

void Building::setTypeId(const irr::u32 &buildingid) {
  mBuildingId = buildingid;
}

void Building::initialise(irr::scene::ISceneNode *parent, const irr::u32 &id, const irr::core::vector3df &pos, const Model *pmodel) {
  std::printf("Intitialising building %u", id);

  mBuildingId = id;
  pModel = new Model(*pmodel);
  pModel->initialise(parent);
  pModel->setPosition(pos);
}
