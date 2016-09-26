#include "buildingtype.hpp"
#include "model.hpp"

BuildingType::BuildingType() :
  mName(""),
  pModel(new Model())
{
}

BuildingType::BuildingType(const BuildingType &obj) :
  mName(obj.mName),
  pModel(new Model(*obj.pModel))
{
}

BuildingType::~BuildingType() {
  delete pModel;
}

std::string BuildingType::getName() const {
  return mName;
}

Model* BuildingType::getPModel() const {
  return pModel;
}
