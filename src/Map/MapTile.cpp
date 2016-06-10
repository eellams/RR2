// Copyright (c) 2016 Eddie Ellams All Rights Reserved.
#include "Map/MapTile.hpp"

void MapTile::CreateModel(struct Surround s) {
  std::clog << "Creating model" << std::endl;

  std::array< std::pair<bool,bool>, 4> args;

  if (s.current) {
    // Outward corners
    if (!s.above && s.below && !s.right && s.left) {
      // Outward corner, [peak at] below left
      args[0].first = 0;
      args[0].second = 0;

      args[1].first = 0;
      args[1].second = 1;

      args[2].first = 1;
      args[2].second = 1;

      args[3].first = 1;
      args[3].second = 0;

      createTile(args, 1);
    }
    else if (s.above && !s.below && !s.right && s.left) {
      // Outward corner, above left
      args[0].first = 0;
      args[0].second = 1;

      args[1].first = 1;
      args[1].second = 1;

      args[2].first = 1;
      args[2].second = 0;

      args[3].first = 0;
      args[3].second = 0;

      createTile(args, 1);
    }
    else if (s.above && !s.below && s.right && !s.left) {
      // Outward corner, above right
      args[0].first = 1;
      args[0].second = 1;

      args[1].first = 1;
      args[1].second = 0;

      args[2].first = 0;
      args[2].second = 0;

      args[3].first = 0;
      args[3].second = 1;

      createTile(args, 1);
    }
    else if (!s.above && s.below && s.right && !s.left) {
      // Outward corner, below right
      args[0].first = 1;
      args[0].second = 0;

      args[1].first = 0;
      args[1].second = 0;

      args[2].first = 0;
      args[2].second = 1;

      args[3].first = 1;
      args[3].second = 1;

      createTile(args, 1);
    }

    // Flat walls
    else if (!s.above && s.below && s.right && s.left) {
      // Facing above
      args[0].first = 1;
      args[0].second = 0;

      args[1].first = 0;
      args[1].second = 0;

      args[2].first = 0;
      args[2].second = 1;

      args[3].first = 1;
      args[3].second = 1;

      createTile(args, 2);
    }

    else if (s.above && s.below && !s.right && s.left) {
      // Facing right
      args[0].first = 0;
      args[0].second = 0;

      args[1].first = 0;
      args[1].second = 1;

      args[2].first = 1;
      args[2].second = 1;

      args[3].first = 1;
      args[3].second = 0;

      createTile(args, 2);
    }

    else if (s.above && !s.below && s.right && s.left) {
      // Facing below
      args[0].first = 0;
      args[0].second = 1;

      args[1].first = 1;
      args[1].second = 1;

      args[2].first = 1;
      args[2].second = 0;

      args[3].first = 0;
      args[3].second = 0;

      createTile(args, 2);
    }

    else if (s.above && s.below && s.right && !s.left) {
      // Facing left
      args[0].first = 1;
      args[0].second = 1;

      args[1].first = 1;
      args[1].second = 0;

      args[2].first = 0;
      args[2].second = 0;

      args[3].first = 0;
      args[3].second = 1;

      createTile(args, 2);
    }

    else if (s.above && s.below && s.right && s.left) {
      // Create a 'roof' tile
      args[0].first = 0;
      args[0].second = 0;

      args[1].first = 0;
      args[1].second = 1;

      args[2].first = 1;
      args[2].second = 1;

      args[3].first = 1;
      args[3].second = 0;

      createTile(args, 4);
    }

    else {
      // Error, this should never occur
      std::clog << "Incorrect tile model type!" << std::endl;
      args[0].first = 0;
      args[0].second = 0;

      args[1].first = 0;
      args[1].second = 1;

      args[2].first = 1;
      args[2].second = 1;

      args[3].first = 1;
      args[3].second = 0;

      createTile(args, 0);
    }
  }

  else {
    // A standard flat tile
    args[0].first = 0;
    args[0].second = 0;

    args[1].first = 0;
    args[1].second = 1;

    args[2].first = 1;
    args[2].second = 1;

    args[3].first = 1;
    args[3].second = 0;

    createTile(args, 0);
  }
}

// tuple < tuple <x,y>, index_mCornerHeights >
//
/*
void MapTile::createInwardCorner(const std::array< std::pair<bool, bool>, 4>& points) {
  struct TriStrip tris;
  struct TrianglePoint tpoint;
  bool isHigh = true;

  // TODO normals

  // For each point in points
  for (const auto& point : points) {
    // Set potition
    tpoint.pos.set(point.first * TILE_SIZE,
      point.second * TILE_SIZE,
      mCornerHeights[point.first + 2*point.second]);

    // If the first point, we assume that it is 'high'
    if (isHigh) {
      tpoint.pos.Z += TILE_WALL_HEIGHT;
      isHigh = false;
    }

    // Set normal, colour and UV
    tpoint.normal.set(0,0,1); // TODO
    tpoint.colour = irr::video::SColor(255,255,255,255);
    tpoint.uv.set(point.first, point.second);

    tris.points.push_back(tpoint);
  }

  AddTriStrip(tris, 1);
}
*/

void MapTile::createTile(const std::array< std::pair<bool,bool>, 4>& points, const irr::u32 noHigh) {
  std::clog << "Creating a tile model" << std::endl;
  struct TriStrip tris;
  struct TrianglePoint tpoint;
  irr::u32 highCount = 0;

  // TODO normals

  // For each point in points
  for (const auto& point : points) {
    // Set potition
    //  Correct Y-up system
    tpoint.pos.set(point.first * TILE_SIZE,
      mCornerHeights[2*point.first + point.second + ((point.first && point.second)*-1) + ((point.first && !point.second)*1)],
      point.second * TILE_SIZE);

    // The first points are assumed 'high'
    if (highCount < noHigh) {
      tpoint.pos.Y += TILE_WALL_HEIGHT;
      highCount++;
    }

    // Set normal, colour and UV
    tpoint.normal.set(0,0,1); // TODO
    tpoint.colour = irr::video::SColor(255,255,255,255);
    tpoint.uv.set(point.first, point.second);

    tris.points.push_back(tpoint);
  }

  AddTriStrip(tris, 1);
}

/*
void MapTile::createFlat() {
  std::clog << "Creating a flat model" << std::endl;

  struct TriStrip tris;
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

  tris.points.push_back(t0);
  tris.points.push_back(t1);
  tris.points.push_back(t2);
  tris.points.push_back(t3);

  AddTriStrip(tris, 1);
}
*/
