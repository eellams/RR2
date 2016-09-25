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

void Building::initialise(irr::scene::ISceneNode* parent, const BuildingType& btype, const irr::core::vector3df& pos) {
  //std::memcpy(pModel, btype.getModel(), sizeof(Model));
  pModel = new Model(btype.getModel());
  pModel->initialise(parent);
  pModel->setPosition(pos);
}
