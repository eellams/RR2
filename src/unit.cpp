#include "unit.hpp"
#include "model.hpp"

Unit::Unit() :
  mUnitId(0),
  mUnitType(0),
  mPos(),
  pModel(NULL)
{
}

Unit::Unit(const Unit &obj) :
  mUnitId(obj.mUnitId),
  mUnitType(obj.mUnitType),
  mPos(obj.mPos),
  pModel(NULL)
{
}

Unit::~Unit() {
}

irr::u32 Unit::getId() const {
  return mUnitId;
}

irr::u32 Unit::getTypeId() const {
  return mUnitType;
}

irr::core::vector3df Unit::getPosition() const {
  return mPos;
}

void Unit::setId(const irr::u32 &id) {
  mUnitId = id;
}

void Unit::setTypeId(const irr::u32 &tid) {
  mUnitType = tid;
}

void Unit::initialise(irr::scene::ISceneNode *parent, const irr::u32 &id, const irr::core::vector3df &pos, const Model *pmodel) {
  std::clog << "Initialising unit " << id << std::endl;
  mUnitId = id;
  pModel = new Model(*pmodel);
  pModel->initialise(parent);
  pModel->setPosition(pos);
}
