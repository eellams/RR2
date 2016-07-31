#include "model.hpp"

Model::Model() :
  mModelPath(""),
  mPosX(0),
  mPosY(0),
  mPosZ(0),
  mTextures(),
  mChildren(),
  pMesh(NULL),
  pNode(NULL)
{

}

Model::~Model() {
  //if (pMesh != NULL) pMesh->drop();
  //if (pNode != NULL) pNode->drop();
}


void Model::setPosition(const irr::core::vector3df &pos) {
  mPos = pos;
  if (pNode != NULL) pNode->setPosition(pos);
}

void Model::setPosition(const irr::f32 &x, const irr::f32 &y, const irr::f32 &z) {
  setPosition(irr::core::vector3df(x,y,z));
}

std::string Model::getModelPath() const {
  return mModelPath;
}

irr::core::vector3df Model::getPosition() const {
  return mPos;
}

void Model::initialise(irr::scene::ISceneNode* parent) {
  pMesh = parent->getSceneManager()->getMesh(mModelPath.c_str());

  pNode = parent->getSceneManager()->addAnimatedMeshSceneNode(pMesh);
  pNode->setParent(parent);
  pNode->setPosition(mPos);

  for (auto texture : mTextures) {
    pNode->setMaterialTexture(
      texture.first,
      parent->getSceneManager()->getVideoDriver()->getTexture(texture.second.c_str()));
  }

  setFlags();
}

void Model::setFlags() {
  pNode->setMaterialFlag(irr::video::EMF_LIGHTING, true);
  pNode->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);
}
