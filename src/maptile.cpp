// Copyright (c) 2016 Eddie Ellams All Rights Reserved.
#include "maptile.hpp"

MapTile::MapTile(size_t t) :
  mTileNumber(t), mHeight(0), mTileType(0) {

}

irr::u32 MapTile::getTileNumber() const {
  return mTileNumber;
}

irr::f32  MapTile::getHeight() const {
  return mHeight;
}

irr::u32  MapTile::getTileType() const {
  return mTileType;
}

irr::scene::ITriangleSelector* MapTile::getTriangleSelector() {
  return mGo.getTriangleSelector();
}

struct Surround MapTile::getPrevSurround() {
  return mPrevSurround;
}

void MapTile::setParent(irr::scene::ISceneNode* parent) {
  mGo.setParent(parent);
}

void MapTile::setPosition(const irr::core::vector3df& position) {
  mGo.setPosition(position);
}

void MapTile::setTexture(const std::string& tex) {
  mGo.setTexture(tex);
}

void MapTile::setDebug() {
  mGo.setDebug();
}

void MapTile::setCornerHeights(const std::array<irr::f32, 4>& cornerHeights) {
  mCornerHeights = cornerHeights;
}

void MapTile::setTileType(const irr::u32& tileType) {
  mTileType = tileType;
}

bool MapTile::createModel(struct Surround s) {
  std::array< std::pair<bool,bool>, 4> args;

  mPrevSurround = s;

  // TODO I'm sure a loop could be used here...
  if (s.current) {
    // Outward corners
    if (!s.above && s.below && !s.right && s.left &&
      s.belowLeft) {
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
    else if (s.above && !s.below && !s.right && s.left &&
      s.aboveLeft) {
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
    else if (s.above && !s.below && s.right && !s.left &&
      s.aboveRight) {
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
    else if (!s.above && s.below && s.right && !s.left &&
      s.belowRight) {
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
    else if (!s.above && s.below && s.right && s.left &&
      s.belowRight && s.belowLeft) {
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
    else if (s.above && s.below && !s.right && s.left &&
      s.aboveLeft && s.belowLeft) {
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
    else if (s.above && !s.below && s.right && s.left &&
      s.aboveRight && s.aboveRight) {
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
    else if (s.above && s.below && s.right && !s.left &&
      s.aboveRight && s.belowRight) {
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

    // Inward corners
    else if (s.above && s.below && s.right && s.left &&
      !s.belowLeft && s.belowRight && s.aboveLeft && s.aboveRight) {
      // Inward corner, [trough at] below left
      args[0].first = 0;
      args[0].second = 0;

      args[1].first = 0;
      args[1].second = 1;

      args[2].first = 1;
      args[2].second = 1;

      args[3].first = 1;
      args[3].second = 0;

      createTile(args, 1, true);
    }
    else if (s.above && s.below && s.right && s.left &&
      s.belowLeft && s.belowRight && !s.aboveLeft && s.aboveRight) {
      // Inward corner, above left
      args[0].first = 0;
      args[0].second = 1;

      args[1].first = 1;
      args[1].second = 1;

      args[2].first = 1;
      args[2].second = 0;

      args[3].first = 0;
      args[3].second = 0;

      createTile(args, 1, true);
    }
    else if (s.above && s.below && s.right && s.left &&
      s.belowLeft && s.belowRight && s.aboveLeft && !s.aboveRight) {
      // Inward corner, above right
      args[0].first = 1;
      args[0].second = 1;

      args[1].first = 1;
      args[1].second = 0;

      args[2].first = 0;
      args[2].second = 0;

      args[3].first = 0;
      args[3].second = 1;

      createTile(args, 1, true);
    }
    else if (s.above && s.below && s.right && s.left &&
      s.belowLeft && !s.belowRight && s.aboveLeft && s.aboveRight) {
      // Inward corner, below right
      args[0].first = 1;
      args[0].second = 0;

      args[1].first = 0;
      args[1].second = 0;

      args[2].first = 0;
      args[2].second = 1;

      args[3].first = 1;
      args[3].second = 1;

      createTile(args, 1, true);
    }

    // A 'normal' roof tile
    else if (s.above && s.below && s.right && s.left &&
      s.belowLeft && s.belowRight && s.aboveLeft && s.aboveRight) {

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

    // A tile combination not of the above
    //  creates a 'flat' tile
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
      return false;
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

  return true;
}

void MapTile::createTile(const std::array< std::pair<bool,bool>, 4>& points, const irr::u32 noHigh, bool inward) {
  // array< pair<x,y>, 4>
  struct TriStrip tris;
  struct TrianglePoint tpoint;
  irr::u32 highCount = 0;

  mGo.clear();

  // TODO normals

  // For each point in points
  for (const auto& point : points) {
    // Set potition
    //  Correct Y-up system
    tpoint.pos.set(point.first * TILE_SIZE,
      mCornerHeights[2*point.first + point.second + ((point.first && point.second)*-1) + ((point.first && !point.second)*1)],
      point.second * TILE_SIZE);

    // The first points are assumed 'high'
    if (!inward) {
      if (highCount < noHigh) {
        tpoint.pos.Y += TILE_WALL_HEIGHT;
        highCount++;
      }
    } else {
      if (highCount < noHigh) {
        highCount++;
      } else {
        tpoint.pos.Y += TILE_WALL_HEIGHT;
      }
    }

    // Set normal, colour and UV
    tpoint.normal.set(0,0,1); // TODO
    tpoint.colour = irr::video::SColor(255,255,255,255);
    tpoint.uv.set(point.first, point.second);

    tris.points.push_back(tpoint);
  }

  mGo.addTriStrip(tris, 1);

  // This flag should be set on all terrain tiles
  mGo.setID(COLLISION_MASK_TILE);
}
