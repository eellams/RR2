#include "Map/Map.hpp"

void Map::Initialise(irr::video::IVideoDriver* driver, irr::scene::ISceneManager* smgr) {
  std::clog << "Initialising map '" << mName << "'" << std::endl;

  initialiseTileTypes(driver);

  // Check that the number of tiles is as expected
  if (mTiles.size() != mWidth*mHeight) {
    std::cerr << "Unexpected number of tiles found.  Found: " << mTiles.size() << " expected: " << mWidth*mHeight << std::endl;
    std::cerr << "For development purposes, just going to pad it out" << std::endl;

    // Pad out with (very empty) tiles
    for (size_t i=mTiles.size(); i<mWidth*mHeight; i++) {
      //MapTile x(i);
      mTiles.push_back(MapTile(i));
    }
  }
  else {
    std::clog << "Correct number of tiles found" << std::endl;
  }

  initialiseTiles(driver, smgr);
}

void Map::initialiseTileTypes(irr::video::IVideoDriver* driver) {
  typedef std::map<irr::u32, MapType>::iterator it_type;

  for(it_type iterator = mTypes.begin(); iterator != mTypes.end(); iterator++) {
      // iterator->first = key
      // iterator->second = value
      // Repeat if you also want to iterate through the second map.
      //iterator->second.Initialise(driver);
  }
}

void Map::initialiseTiles(irr::video::IVideoDriver* driver, irr::scene::ISceneManager* smgr) {
  std::clog << "Initilising tiles" << std::endl;

  // Create the heightmap from the deserialised tiles
  createHeightMap();

  // For each tile
  for (irr::u32 i=0; i<mWidth*mHeight; i++) {
    // Create the tile model
    CalculateTileModel(i);

    // Add the node to the map
    irr::scene::IMeshSceneNode* meshnode = smgr -> addMeshSceneNode(mTiles[i].pMesh);

    // Set position
    meshnode->setPosition(irr::core::vector3df((i%mWidth) * TILE_SIZE, (i/mHeight) * TILE_SIZE,0));


    meshnode->setMaterialFlag(irr::video::EMF_LIGHTING, false);
    meshnode->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);
    //meshnode->setMaterialTexture(0, NewTex);
    std::clog << mTypes[mTiles[i].GetTileType()].GetTextureName() << std::endl;

    // Set the texture
    // TODO does this optimise out in the wash?
    //  or do we have <tile number> different textures in memory?
    //  if so, this is likely a waste in meemory
    meshnode->setMaterialTexture(0, driver->getTexture(mTypes[mTiles[i].GetTileType()].GetTextureName().c_str()));

    // Debugging flags
    //meshnode->setMaterialFlag(irr::video::EMF_BACK_FACE_CULLING, false);
    meshnode->setDebugDataVisible(irr::scene::EDS_BBOX);
  }
}

// Creates the heightmap from the vector of tiles
void Map::createHeightMap() {
  std::clog << "Creaing heightmap" << std::endl;
  // In case the heightmap already contains data
  //  at time of writing, can't actually see this situation arising, but better
  //  to be safe than sorry.
  mHeightmap.clear();

  // Fill the heightmap with the tile data
  for (size_t i=0; i<mWidth*mHeight; i++) {
    mHeightmap.push_back(mTiles[i].GetHeight());
  }

  for (size_t i=0; i<mWidth*mHeight; i++) {
    calculateTileCorners(i);
  }
}

void Map::calculateTileCorners(irr::u32 tileNumber) {
  //  1|A |2
  // --------
  // L |  |R
  // --------
  //  0|B |3
  //  where: numbers are indices of cornerHeights
  //   A: above, B: below, L: left, R: right
  //   therefore, 0 is below left.
  //
  // Corner heghts are calculated as an average of the height othe the 4
  //  (including the current tile) tiles surrounding each corner

  irr::u32 n0, n1, n2, n3;                 // Number of corner (adjacent) tiles
  bool above, below;
  irr::u32 maxTiles;
  std::array<irr::f32, 4> cornerHeights;

  // Initallilise
  maxTiles = mWidth * mHeight;
  cornerHeights[0] = cornerHeights[1] = cornerHeights[2] = cornerHeights[3] = mHeightmap[tileNumber];
  n0 = n1 = n2 = n3 = 1;
  above = below = false;

  // TODO This could be optimised to remove the conditionals
  //  hopefully the compiler will deal with this, though

  // If there is a tile below
  if (tileNumber >= mWidth) {
    cornerHeights[0] += mHeightmap[tileNumber-mWidth];
    cornerHeights[3] += mHeightmap[tileNumber-mWidth];

    n0++;
    n3++;

    below = true;
  }

  // If there is a tile above
  if (tileNumber < (mHeight-1)*mWidth) {
    cornerHeights[1] += mHeightmap[tileNumber+mWidth];
    cornerHeights[2] += mHeightmap[tileNumber+mWidth];

    n1++;
    n2++;

    above = true;
  }

  // If there is a tile left
  if ((tileNumber > 0) && ((tileNumber) % mWidth != 0)) {
    cornerHeights[0] += mHeightmap[tileNumber-1];
    cornerHeights[1] += mHeightmap[tileNumber-1];

    n0++;
    n1++;

    // If there is a tile above left
    if (above) {
      cornerHeights[1] += mHeightmap[tileNumber+mWidth-1];

      n1++;
    }

    // If there is a tile below left
    if (below) {
      cornerHeights[0] += mHeightmap[tileNumber-mWidth-1];

      n0++;
    }
  }

  // If there is a tile right
  if ( (tileNumber + 1) % mWidth != 0) {
    cornerHeights[2] += mHeightmap[tileNumber+1];
    cornerHeights[3] += mHeightmap[tileNumber+1];

    n2++;
    n3++;

    // If there is a tile above right
    if (above) {
      cornerHeights[2] += mHeightmap[tileNumber+mWidth+1];

      n2++;
    }

    // If there is a tile below right
    if (below) {
      cornerHeights[3] += mHeightmap[tileNumber-mWidth+1];

      n3++;
    }
  }

  // Calculate the average
  cornerHeights[0] /= n0;
  cornerHeights[1] /= n1;
  cornerHeights[2] /= n2;
  cornerHeights[3] /= n3;

  // Set the corner heights to the average that has been calculated
  //setCornerHeights(c0,c1,c2,c3);

  mTiles[tileNumber].SetCornerHeights(cornerHeights);
}



struct Surround Map::calculateSurround(irr::u32 tileNumber) {

  /*
  n := tileNumber

  n  |n+1 |n+2 |... |2n-1
  -----------------------
  0  |1   |2   |... |n-1

  */

  struct Surround toReturn;
  toReturn.above = toReturn.below = toReturn.left = toReturn.right = false;

  std::clog << "Calculating surround for: " << tileNumber << std::endl;

  // Current tile (whether solid or not)
  toReturn.current = mTypes[mTiles[tileNumber].GetTileType()].GetSolid();

  // Below
  if (tileNumber >= mWidth) {
    toReturn.below = mTypes[mTiles[tileNumber - mWidth].GetTileType()].GetSolid();
  } else {
    // Botton row, assume solid
    toReturn.below = true;
  }

  // Above
  if (tileNumber < (mHeight-1)*mWidth) {
    toReturn.above = mTypes[mTiles[tileNumber + mWidth].GetTileType()].GetSolid();
  } else {
    // Top row, assume solid
    toReturn.above = true;
  }

  // Left
  if ((tileNumber > 0) && ((tileNumber) % mWidth != 0)) {
    toReturn.left = mTypes[mTiles[tileNumber - 1].GetTileType()].GetSolid();
  } else {
    // Furthest left on each row
    toReturn.left = true;
  }

  // Right
  if ( (tileNumber + 1) % mWidth != 0) {
    toReturn.right = mTypes[mTiles[tileNumber + 1].GetTileType()].GetSolid();
  } else {
    // Furthest right of each row
    toReturn.right = true;
  }

  return toReturn;
}
