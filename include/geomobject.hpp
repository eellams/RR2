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

  // Setters
  void setParent(irr::scene::ISceneNode* parent);
  void setPosition(const irr::core::vector3df& pos);
  void setTexture(const std::string& tex);
  void setDebug();
  void setID(irr::s32 id);

  void addTriStrip(struct TriStrip& tris, irr::u32 bufferNum);
  void clear();

protected:
  void setFlags();

  irr::scene::ISceneNode* pParent;
  irr::scene::SMesh* pMesh;
  irr::scene::IMeshSceneNode* pMeshSceneNode;
  irr::scene::ITriangleSelector* pSelector;
};

#endif
