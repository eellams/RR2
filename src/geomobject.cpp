#include "geomobject.hpp"

GeomObject::GeomObject() :
  pMesh(NULL), pMeshSceneNode(NULL), pSelector(NULL), pParent(NULL) {

  }

GeomObject::~GeomObject() {
  if (pParent != NULL) pParent->drop();
  if (pMesh != NULL) pMesh->drop();
  if (pMeshSceneNode != NULL) pMeshSceneNode->drop();
  if (pSelector != NULL) pSelector->drop();
}

irr::scene::ITriangleSelector* GeomObject::getTriangleSelector() const {
  return pMeshSceneNode->getTriangleSelector();
}

void GeomObject::setParent(irr::scene::ISceneNode* parent) {
  pParent = parent;
  if (pMeshSceneNode != NULL) pMeshSceneNode->setParent(parent);
}

void GeomObject::setPosition(const irr::core::vector3df& pos) {
  pMeshSceneNode->setPosition(pos);
}

void GeomObject::setTexture(const std::string& tex) {
  // TODO this should be able to deal with multiple texture layers
  //  as and when (if?) they are implemented
  pMeshSceneNode->setMaterialTexture(0, pMeshSceneNode->getSceneManager()->getVideoDriver()->getTexture(tex.c_str()));
}

// TODO this needs to be user-configurable
void GeomObject::setDebug() {
  pMeshSceneNode->setDebugDataVisible(irr::scene::EDS_BBOX);
}

void GeomObject::setID(irr::s32 id) {
  pMeshSceneNode->setID(id);
}

void GeomObject::setFlags() {
  // TODO all of this
  //  needs to be configurable

  pMeshSceneNode->setMaterialFlag(irr::video::EMF_LIGHTING, false);
  pMeshSceneNode->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);
}

void GeomObject::addTriStrip(struct TriStrip& tris, irr::u32 bufferNum) {
  std::clog << "Adding triangle strip" << std::endl;
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

void GeomObject::clear() {
  // TODO is this memory safe?
  if (pMesh != NULL) {
    // Drop and reset pointer
    pMesh->drop();
    pMesh = NULL;

    // Remove and reset pointer
    pMeshSceneNode->remove();
    pMeshSceneNode = NULL;
  }
}
