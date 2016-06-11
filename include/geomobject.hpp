#ifndef _GEOM_OBJECT_HPP
#define _GEOM_OBJECT_HPP

#include <array>

#include <irrlicht.h>

struct TrianglePoint {
  irr::core::vector3df pos;
  irr::core::vector3df normal;
  irr::video::SColor colour;
  irr::core::vector2df uv;
};

struct TriStrip {
  std::vector<TrianglePoint> points;
};

class GeomObject {
public:
  GeomObject() {
    pMesh = NULL;
    pMeshSceneNode = NULL;
    pSelector = NULL;
  }

  ~GeomObject() {
    if (pMesh != NULL) pMesh->drop();
    if (pMeshSceneNode != NULL) pMeshSceneNode->drop();
    if (pSelector != NULL) pSelector->drop();
  }

  // Set the parent of the node
  void SetParent(irr::scene::ISceneNode* parent) {
    pParent = parent;
    if (pMeshSceneNode != NULL) pMeshSceneNode->setParent(parent);
  }

  // Set position of the node
  void SetPosition(irr::core::vector3df& pos) {
    pMeshSceneNode->setPosition(pos);
  }

  // TODO this should be able to deal with multiple texture layers
  //  as and when (if?) they are implemented
  void SetTexture(std::string& tex) {
    pMeshSceneNode->setMaterialTexture(0, pMeshSceneNode->getSceneManager()->getVideoDriver()->getTexture(tex.c_str()));
  }

  // TODO this needs to be user-configurable
  void SetDebug() {
    pMeshSceneNode->setDebugDataVisible(irr::scene::EDS_BBOX);
  }

  void SetID(irr::s32 id) {
    pMeshSceneNode->setID(id);
  }

  irr::scene::ITriangleSelector* GetTriangleSelector() {
    return pMeshSceneNode->getTriangleSelector();
  }

  void AddTriStrip(const TriStrip& tris, irr::u32 bufferNum) {
    if (pMesh == NULL) pMesh = new irr::scene::SMesh;

    irr::u16 i;
    irr::u16 noPoints;
    irr::u16 noTris;

    irr::scene::SMeshBuffer* pBuffer;

    noPoints = tris.points.size();
    noTris = noPoints-2;

    // If the buffer already exists
    if (bufferNum < pMesh->getMeshBufferCount()) {
      pBuffer = (irr::scene::SMeshBuffer*) pMesh->getMeshBuffer(bufferNum);
    }

    // Otherwise, we need to add it to the list of mesh buffers
    else {
      // create new buffer
      pBuffer = new irr::scene::SMeshBuffer();
      pMesh->addMeshBuffer(pBuffer);
      // to simplify things we drop here but continue using buf
      pBuffer->drop();
    }

    // Set the number of vertices that we expect
    pBuffer->Vertices.set_used(noPoints);

    for (i=0; i<noPoints; i++) {
      irr::video::S3DVertex& v = pBuffer->Vertices[i];

      v.Pos.set(tris.points[i].pos);
      v.Normal.set(tris.points[i].normal);
      v.Color = tris.points[i].colour;
      v.TCoords.set(tris.points[i].uv);
    }

    pBuffer->Indices.set_used(noTris*3);
    for (i=0; i < noTris; i++ ) {

      // The math to work out which indices go where
      pBuffer->Indices[i*3] = i;
      pBuffer->Indices[i*3+2] = i + 2 - (1 - (i%2));
      pBuffer->Indices[i*3+1] = i + 2 - (i%2);

      // Special casse that don't follow the above rules
      if (i==0) {
        pBuffer->Indices[0] = 0;
        pBuffer->Indices[2] = 2;
        pBuffer->Indices[1] = 1;
      }
      else if (i == 1) {
        pBuffer->Indices[i*3] = 0;
      }
    }

    // Set flags, and recalcuate bounding boxes
    pBuffer->recalculateBoundingBox();
    pMesh->setDirty();
    pMesh->recalculateBoundingBox();

    // Set the parent
    if (pMeshSceneNode == NULL) {
      pMeshSceneNode = pParent->getSceneManager()->addMeshSceneNode(pMesh);
      pMeshSceneNode->setParent(pParent);
    }

    // Set the flags
    setFlags();

    pSelector = pMeshSceneNode->getSceneManager()->createTriangleSelector(pMesh, pMeshSceneNode);
    pMeshSceneNode->setTriangleSelector(pSelector);
  }

protected:
  void setFlags() {
    // TODO all of this
    //  needs to be configurable
    pMeshSceneNode->setMaterialFlag(irr::video::EMF_LIGHTING, false);
    pMeshSceneNode->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);
  }

  irr::scene::ISceneNode* pParent;
  irr::scene::SMesh* pMesh;
  irr::scene::IMeshSceneNode* pMeshSceneNode;
  irr::scene::ITriangleSelector* pSelector;
  //irr::scene::ITriangleSelector* pSelector;
};

#endif
