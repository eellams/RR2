// Copyright (c) 2016 Eddie Ellams All Rights Reserved.
#include "Map/MapTile.hpp"

void MapTile::CreateModel(struct Surround s) {
  std::clog << "Creating model" << std::endl;

  //std::array<irr::core::vector3df, 4> argument;

  /*
  if (s.current) {
    // Outward corners
    if (!s.above && s.below && !s.right && s.left) {
      // Outward corner, [peak at] below left
      //argument[0].set(0,0,);
      //argument[1].set(0,1,0)
    }
    else if (s.above && !s.below && !s.right && s.left) {
      // Outward corner, above left
    }
    else if (s.above && !s.below && s.right && !s.left) {
      // Outward corner, above right
    }
    else if (!s.above && s.below && s.right && !s.left) {
      // Outward corner, below right
    }

    // Flat walls
    else if (!s.above && s.below && s.right && s.left) {
      // Facing above
    }

    else if (s.above && s.below && !s.right && s.left) {
      // Facing right
    }

    else if (s.above && !s.below && s.right && s.left) {
      // Facing below
    }

    else if (s.above && s.below && s.right && !s.left) {
      // Facing left
    }
  }

  else {
    // A standard flat tile
    createFlat();
  }
  */

  createFlat();
}


void MapTile::createInwardCorner(std::array<irr::core::vector3df, 4> points) {

}

void MapTile::createFlat() {
  std::clog << "Creating a flat model" << std::endl;

  struct TriStrip x;
  struct TrianglePoint t0, t1, t2, t3;

  // TODO normals

  t0.pos.set(0,0,mCornerHeights[0]);
  t0.normal.set(0,0,1);
  //t0.colour.set(255,255,0,0);
  t0.colour=irr::video::SColor(255,255,255,255);
  t0.uv.set(0,0);

  t1.pos.set(0,TILE_SIZE,mCornerHeights[1]);
  t1.normal.set(0,0,1);
  //t1.colour.set(255,0,255,0);
  t1.colour=irr::video::SColor(255,0,0,0);
  t1.uv.set(0,1);

  t2.pos.set(TILE_SIZE,TILE_SIZE,mCornerHeights[2]);
  t2.normal.set(0,0,1);
  //t2.colour.set(255,0,0,255);
  t2.colour=irr::video::SColor(255,255,255,255);
  t2.uv.set(1,1);

  t3.pos.set(TILE_SIZE,0,mCornerHeights[3]);
  t3.normal.set(0,0,1);
  //t3.colour.set(255,255,255,255);
  t3.colour=irr::video::SColor(255,255,255,255);
  t3.uv.set(1,0);

  x.points.push_back(t0);
  x.points.push_back(t1);
  x.points.push_back(t2);
  x.points.push_back(t3);

  AddTriStrip(x, 1);
}
