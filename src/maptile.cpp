// Copyright (c) 2016 Eddie Ellams All Rights Reserved.
#include "maptile.hpp"

void MapTile::CreateModel(struct Surround s) {
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

// pair <x, y>
void MapTile::createTile(const std::array< std::pair<bool,bool>, 4>& points, const irr::u32 noHigh) {
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

  mGo.AddTriStrip(tris, 1);

  // This flag should be set on all terrain tiles
  mGo.SetID(COLLISION_MASK_TILE);
}
