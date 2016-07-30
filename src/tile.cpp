// Copyright (c) 2016 Eddie Ellams All Rights Reserved.
#include "tile.hpp"
#include "geomobject.hpp"

Tile::Tile(size_t t) :
  mTileNumber(t),
  mHeight(0),
  pGeom(NULL)
{
  pGeom = new GeomObject();
}

Tile::~Tile() {
  //delete pGeom;
}

irr::u32 Tile::getTileNumber() const {
  return mTileNumber;
}

irr::f32  Tile::getHeight() const {
  return mHeight;
}

irr::f32  Tile::getCornerHeightMax() const {
  // TODO this could be made neater
  irr::f32 toReturn = 0;

  for (auto h : mCornerHeights) {
    if (toReturn < h) toReturn = h;
  }

  return toReturn;
}

irr::u32  Tile::getTileType() const {
  return mTileType;
}

irr::scene::ITriangleSelector* Tile::getTriangleSelector() {
  return pGeom->getTriangleSelector();
}

struct Surround Tile::getSurround() const {
  return mSurround;
}

void Tile::setParent(irr::scene::ISceneNode* parent) {
  pGeom->setParent(parent);
}

void Tile::setPosition(const irr::core::vector3df& position) {
  pGeom->setPosition(position);
}

void Tile::setTexture(const std::string& tex) {
  pGeom->setTexture(tex);
}

void Tile::setDebug() {
  pGeom->showBoundingBox();
}

void Tile::setCornerHeights(const std::array<irr::f32, 4>& cornerHeights) {
  mCornerHeights = cornerHeights;
}

void Tile::setTileType(const irr::u32& tileType) {
  mTileType = tileType;
}

void Tile::initialise(irr::scene::ISceneManager* pmanager) {
  pGeom->clear();
  pGeom->initialise(pmanager);
}

bool Tile::createModel(struct Surround s) {
  std::array< std::pair<bool,bool>, 4> args;

  mSurround = s;

  // TODO I'm sure a loop could be used here...
  //  double TODO as I feel this is important
  //  but not that important that I'm going to do it right now :p
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

void Tile::createTile(const std::array< std::pair<bool,bool>, 4>& points, const irr::u32 noHigh, bool invertHigh) {
  // array< pair<x,y>, 4>
  struct TriStrip tris;
  struct TrianglePoint tpoint;
  irr::u32 highCount = 0;

  // Clear any old models
  pGeom->clear();

  // TODO normals

  // For each point in points
  for (const auto& point : points) {
    // Set potition
    //  Correct Y-up system
    tpoint.pos.set(point.first * TILE_SIZE,
      mCornerHeights[2*point.first + point.second + ((point.first && point.second)*-1) + ((point.first && !point.second)*1)],
      point.second * TILE_SIZE);

    if (!invertHigh) {
      // The first points are assumed 'high'
      if (highCount < noHigh) {
        tpoint.pos.Y += TILE_WALL_HEIGHT;
        highCount++;
      }
    } else {
      // The first points are assumed 'low'
      if (highCount < noHigh) {
        highCount++;
      } else {
        tpoint.pos.Y += TILE_WALL_HEIGHT;
      }
    }

    // Set normal, colour and UV
    tpoint.normal.set(0,1,0); // TODO UV
    tpoint.colour = irr::video::SColor(255,255,255,255);
    tpoint.uv.set(point.first, point.second);

    tris.points.push_back(tpoint);
  }

  // Add to 1st buffer
  pGeom->addTriStrip(tris, 1);

  // This flag should be set on all terrain tiles
  pGeom->setID(COLLISION_MASK_TILE);
}
