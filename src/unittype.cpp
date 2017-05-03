#include "unittype.hpp"
#include "model.hpp"

UnitType::UnitType() :
  mName(""),
  mModel()
{
}

UnitType::UnitType(const UnitType &obj) :
  mName(obj.mName),
  mModel(obj.mModel)
{
}

UnitType::~UnitType() {
}

std::string UnitType::getName() const {
  return mName;
}

Model* UnitType::getPModel() {
  return &mModel;
}
