#include "buildingtype.hpp"
#include "model.hpp"

BuildingType::BuildingType() :
  mName(""),
  mModel()
{
}

BuildingType::BuildingType(const BuildingType &obj) :
  mName(obj.mName),
  mModel(obj.mModel)
{
}

BuildingType::~BuildingType() {

}

std::string BuildingType::getName() const {
  return mName;
}

Model* BuildingType::getPModel() {
  return &mModel;
}
