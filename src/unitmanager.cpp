#include "unitmanager.hpp"
#include "unit.hpp"
#include "unittype.hpp"

UnitManager::UnitManager() :
  mId(0),
  mTypes(),
  mInstances(),
  pNode(NULL)
{
}

UnitManager::UnitManager(const UnitManager &obj) :
  mId(0),
  mTypes(obj.mTypes),
  mInstances(obj.mInstances),
  pNode(NULL)
{
}

UnitManager::~UnitManager() {
}

std::map<irr::u32, UnitType> UnitManager::getTypes() const {
  return mTypes;
}

std::map<irr::u32, Unit> UnitManager::getInstances() const {
  return mInstances;
}

void UnitManager::setTypes(const std::map<irr::u32, UnitType> &types) {
  mTypes = types;
}
void UnitManager::setInstances(const std::map<irr::u32, Unit> &instances) {
  mInstances = instances;
}

void UnitManager::add(const irr::u32 &tid, const irr::core::vector3df &pos, const irr::u32 &knownid) {
  irr::u32 id;

  // If the ID of the building is known
  //  e.g. whether need to create an entirely new building with new ID
  if (knownid == 0) {
    id = mId;
    mId++;
    mInstances[id] = Unit();
  }
  else {
    // Ensure that mId is always bigger than the highest known id
    id = knownid;
    if (id >= mId) mId = id + 1;
  }

  std::printf("Adding new unit id %u type %u (%s) at position (%f,%f,%f)", id, tid, mTypes[tid].getName().c_str(), pos.X, pos.Y, pos.Z);

  // Initialise the unit
  mInstances[id].setId(id);
  mInstances[id].initialise(
    pNode,
    id,
    pos,
    mTypes[tid].getPModel()
  );
}

void UnitManager::initialise(irr::scene::ISceneNode *parentnode) {
  pNode = parentnode->getSceneManager()->addEmptySceneNode();
  pNode->setParent(parentnode);

  initialiseTypes();
  initialiseInstances();
}

void UnitManager::initialiseTypes() {

}

void UnitManager::initialiseInstances() {

}
