#ifndef _UNIT_MANAGER_HPP
#define _UNIT_MANAGER_HPP

#include <irrlicht.h>
#include <map>

class Unit;
class UnitType;

class UnitManager {
public:
  UnitManager();
  UnitManager(const UnitManager &obj);
  ~UnitManager();

  std::map<irr::u32, UnitType> getTypes() const;
  std::map<irr::u32, Unit> getInstances() const;

  void setTypes(const std::map<irr::u32, UnitType> &types);
  void setInstances(const std::map<irr::u32, Unit> &instances);

  void add(const irr::u32 &tid, const irr::core::vector3df &pos, const irr::u32 &knownid = 0);

  void initialise(irr::scene::ISceneNode *parentnode);

private:
  void initialiseTypes();
  void initialiseInstances();

  irr::u32 mId;
  std::map<irr::u32, UnitType> mTypes;
  std::map<irr::u32, Unit> mInstances;

  irr::scene::ISceneNode *pNode;
};

#endif
