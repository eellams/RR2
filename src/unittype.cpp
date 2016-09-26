#include "unittype.hpp"
#include "model.hpp"

UnitType::UnitType() :
  mName(""),
  pModel(new Model())
{
}

UnitType::UnitType(const UnitType &obj) :
  mName(obj.mName),
  pModel(new Model(*obj.pModel))
{
}

UnitType::~UnitType() {
  delete pModel;
}

std::string UnitType::getName() const {
  return mName;
}

Model* UnitType::getPModel() const {
  return pModel;
}
