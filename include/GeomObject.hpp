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
  }

  ~GeomObject() {
    if (pMesh != NULL) pMesh->drop();
  }

  void SetTexture(irr::video::ITexture* pTex) {
    //pMesh->getMaterial().MaterialType = irr::video::EMT_SOLID;
    //pMesh->getMaterial().setTexture( 0, pTex );
    //pMesh->getMaterial().Lighting = false;

    //pMesh->setMaterialTexture( 0, pTex );
    //pMesh->setMaterialType( irr::video::EMT_SOLID );

    //pMesh->getMaterial(0).Textures[0] = pTex;

    /*
    for (irr::u32 i=0; i<pMesh->getMeshBufferCount(); i++) {
      //pMesh->getMeshBuffer(i)->getMaterial().setTexture(0, pTex);

      pMesh->getMeshBuffer(i)->getMaterial().MaterialType = irr::video::EMT_SOLID;
      pMesh->getMeshBuffer(i)->getMaterial().setTexture( 0, pTex );
      pMesh->getMeshBuffer(i)->getMaterial().Lighting = false;
    }
    */
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
      pBuffer->Indices[i*3+1] = i + 2 - (1 - (i%2));
      pBuffer->Indices[i*3+2] = i + 2 - (i%2);

      // Special casse that don't follow the above rules
      if (i==0) {
        pBuffer->Indices[0] = 0;
        pBuffer->Indices[1] = 2;
        pBuffer->Indices[2] = 1;
      }
      else if (i == 1) {
        pBuffer->Indices[i*3] = 0;
      }
    }

    pBuffer->recalculateBoundingBox();
    pMesh->setDirty();
    pMesh->recalculateBoundingBox();
  }

//protected:
  irr::scene::SMesh* pMesh;
};

#endif
