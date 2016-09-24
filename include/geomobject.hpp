#ifndef _GEOM_OBJECT_HPP
#define _GEOM_OBJECT_HPP

#include <array>
#include <vector>
#include <iostream>

#include <irrlicht.h>
#include "generic.hpp"

class GeomObject {
public:
  GeomObject();
  ~GeomObject();

  // Getters
  irr::scene::ITriangleSelector* getTriangleSelector() const;
  irr::scene::ISceneNode* getSceneNode() const {
    return pSceneNode;
  }

  // Setters
  void setAlpha() {
    //material.MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL;
    pMeshSceneNode->setMaterialType(irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL);
  }

  inline void remove() {
    std::clog << pSceneNode << std::endl;
    if (pSceneNode != NULL) pSceneNode->remove();
  }

  void setParent(irr::scene::ISceneNode* parent);
  void setPosition(const irr::core::vector3df& pos);
  void setTexture(const std::string& tex, const int& texturelayer = 0);
  void setID(irr::s32 id);

  void initialise(irr::scene::ISceneManager* pmanager);
  void destroy();

  void showBoundingBox();

  void addTriStrip(struct TriStrip& tris, irr::u32 bufferNum);
  void clear();


protected:
  void setFlags();

  irr::scene::SMesh* pMesh;
  irr::scene::IMeshSceneNode* pMeshSceneNode;
  irr::scene::ISceneNode* pSceneNode;
};

#endif
