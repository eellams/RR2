#include "map.hpp"

Map::Map() :
  mName(""), mDescription(""), mWidth(0), mHeight(0),
  pTileNode(NULL), pBuildingNode(NULL), pTileSelector(NULL) {

  }

/*
Map::Map(std::string n, std::string d, size_t w, size_t h) :
  mName(n), mDescription(d), mWidth(w), mHeight(h), pTileNode(NULL), pTileSelector(NULL) {

  }
*/

Map::~Map() {
  if (pTileNode != NULL) pTileNode->drop();
  if (pBuildingNode != NULL) pBuildingNode->drop();
  if (pTileSelector != NULL) pTileSelector->drop();
}

std::string Map::getName() const {
  return mName;
}

irr::scene::IMetaTriangleSelector* Map::getMetaTriangleSelectorP() const {
  return pTileSelector;
}


void Map::initialise(irr::video::IVideoDriver* driver, irr::scene::ISceneManager* smgr) {
  std::clog << "Initialising map '" << mName << "'" << std::endl;

  initialiseTileTypes();
  initialiseBuildingTypes();

  // Check that the number of tiles is as expected
  if (mTiles.size() != mWidth*mHeight) {
    std::cerr << "Unexpected number of tiles found.  Found: " << mTiles.size() << " expected: " << mWidth*mHeight << std::endl;
    std::cerr << "For development purposes, just going to pad it out" << std::endl;

    // Pad out with (very empty) tiles
    for (size_t i=mTiles.size(); i<mWidth*mHeight; i++) {
      mTiles.push_back(Tile(i));
    }
  }
  else {
    std::clog << "Correct number of tiles found" << std::endl;
  }

  initialiseTiles(smgr->getRootSceneNode());
  initialiseBuildings(smgr->getRootSceneNode());

  pTileSelector = pTileNode->getSceneManager()->createMetaTriangleSelector();

  for (auto child : pTileNode->getChildren()) {
    pTileSelector->addTriangleSelector(child->getTriangleSelector());
  }
}

void Map::mineTile(irr::u32 tileNumber) {
  struct Surround surround;
  struct Surround prevSurround;
  bool above, below;

  // Set the new tile type

  std::clog << "Mining tile " << tileNumber;
  std::clog << " from " << mTiles[tileNumber].getTileType();
  std::clog << " to " << mTileTypes[mTiles[tileNumber].getTileType()].getMineInto() << std::endl;

  mTiles[tileNumber].setTileType( mTileTypes[mTiles[tileNumber].getTileType()].getMineInto() );

  prevSurround = mTiles[tileNumber].getPrevSurround();
  above = false;
  below = false;

  recalculateTileModel(tileNumber);

  if (!(prevSurround == surround)) {
    // The surround has changed
    //  need to recalculate the surrounding tile's models

    // If there is a tile below
    if (tileNumber >= mWidth) {
      recalculateTileModel(tileNumber-mWidth);
      below = true;
    }

    // If there is a tile above
    if (tileNumber < (mHeight-1)*mWidth) {
      recalculateTileModel(tileNumber+mWidth);
      above = true;
    }

    // If there is a tile left
    if ((tileNumber > 0) && ((tileNumber) % mWidth != 0)) {
      recalculateTileModel(tileNumber-1);

      // If there is a tile above left
      if (above) {
        recalculateTileModel(tileNumber+mWidth-1);
      }

      // If there is a tile below left
      if (below) {
        recalculateTileModel(tileNumber-mWidth-1);
      }
    }

    // If there is a tile right
    if ( (tileNumber + 1) % mWidth != 0) {
      recalculateTileModel(tileNumber+1);

      // If there is a tile above right
      if (above) {
        recalculateTileModel(tileNumber+mWidth+1);
      }

      // If there is a tile below right
      if (below) {
        recalculateTileModel(tileNumber-mWidth+1);
      }
    }
  }
}

void Map::initialiseTileTypes() {
  for(auto iterator = mTileTypes.begin(); iterator != mTileTypes.end(); iterator++) {
      // TODO there isn't anything here
      //  it used to be used to load textures into the tile type, which would
      //  then be used in each tile of that class
      //  but, I couldn't get this to work, so instead textures are loaded
      //  when tiles initialised

      // iterator->first = key
      // iterator->second = value
      // Repeat if you also want to iterate through the second map.
  }
}

void Map::initialiseBuildingTypes() {
  for(auto iterator = mBuildingTypes.begin(); iterator != mBuildingTypes.end(); iterator++) {
    // TODO there isn't anything here yet
  }
}

void Map::initialiseTiles(irr::scene::ISceneNode* parentNode) {
  std::clog << "Initilising tiles" << std::endl;

  // Create the heightmap from the deserialised tiles
  //  heightmap is calculated from serialised tile data
  createHeightMap();

  // Set the parent of the 'master' node
  //  the 'master'' node is the parent of all Tile models
  pTileNode = parentNode->getSceneManager()->addEmptySceneNode();
  pTileNode->setParent(parentNode);

  // For each tile, initialise
  //  calculate tile, set texture etc.
  for (irr::u32 i=0; i<mWidth*mHeight; i++) {
    // Create the tile model
    recalculateTileModel(i);
  }
}

void Map::initialiseBuildings(irr::scene::ISceneNode* parentNode) {
  std::clog << "Initialising buildings" << std::endl;
  irr::core::vector3df pos;

  pBuildingNode = parentNode->getSceneManager()->addEmptySceneNode();
  pBuildingNode->setParent(parentNode);

  // For each building
  for (auto& building : mBuildings) {
    // Set the tile number, so it refers to the correct tile
    //building.second.setTileNumber(building.second.getTileX() + building.second.getTileY()*mWidth);

    //std::clog << "BUILDING: " << building.second.getTileX() << "," << building.second.getTileY() << " " << building.second.getTileNumber() << std::endl;

    pos.set(
      ((building.second.getTileNumber() % mWidth) + 0.5f)*TILE_SIZE,
      mTiles[building.second.getTileNumber()].getCornerHeightMax(),
      ((building.second.getTileNumber() / mWidth) + 0.5f)*TILE_SIZE
    );

    // Initialise
    //  set parent, and load the model

    building.second.initialise(
      pBuildingNode, // Parent node
      mBuildingTypes[building.second.getBuildingType()].getModel(), // Model file string
      pos
    );
  }
}

// Creates the heightmap from the vector of tiles
void Map::createHeightMap() {
  // In case the heightmap already contains data
  //  at time of writing, can't actually see this situation arising, but better
  //  to be safe than sorry.
  mHeightmap.clear();

  // Fill the heightmap with the tile data
  for (size_t i=0; i<mWidth*mHeight; i++) {
    mHeightmap.push_back(mTiles[i].getHeight());
  }

  for (size_t i=0; i<mWidth*mHeight; i++) {
    calculateTileCorners(i);
  }
}

// Caclulates the corner heights of the tile as a mean of surrounding tile
//  heights
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

  mTiles[tileNumber].setCornerHeights(cornerHeights);
}

// Calculates whether surrounding tiles are 'solid' or not
//  if the tile is out-of-range (i.e. outside edges of map), assumes solid
struct Surround Map::calculateSurround(irr::u32 tileNumber) {

  /*
  n := tileNumber

  n  |n+1 |n+2 |... |2n-1
  -----------------------
  0  |1   |2   |... |n-1

  */

  struct Surround toReturn;
  bool above, below;

  toReturn.above = toReturn.below = toReturn.left = toReturn.right = false;
  toReturn.belowLeft = toReturn.belowRight = toReturn.aboveLeft = toReturn.aboveRight = false;
  above = below = false;

  // Current tile (whether solid or not)
  toReturn.current = mTileTypes[mTiles[tileNumber].getTileType()].getSolid();

  // Below
  if (tileNumber >= mWidth) {
    toReturn.below = mTileTypes[mTiles[tileNumber - mWidth].getTileType()].getSolid();
    below = true;
  } else {
    // Botton row, assume solid
    toReturn.below = true;
    toReturn.belowLeft = true;
    toReturn.belowRight = true;
  }

  // Above
  if (tileNumber < (mHeight-1)*mWidth) {
    toReturn.above = mTileTypes[mTiles[tileNumber + mWidth].getTileType()].getSolid();
    above = true;
  } else {
    // Top row, assume solid
    toReturn.above = true;
    toReturn.aboveRight = true;
    toReturn.aboveLeft = true;
  }

  // Left
  if ((tileNumber > 0) && ((tileNumber) % mWidth != 0)) {
    toReturn.left = mTileTypes[mTiles[tileNumber - 1].getTileType()].getSolid();

    if (above) {
      toReturn.aboveLeft =  mTileTypes[mTiles[tileNumber + mWidth - 1].getTileType()].getSolid();
    }

    if (below) {
      toReturn.belowLeft =  mTileTypes[mTiles[tileNumber - mWidth - 1].getTileType()].getSolid();
    }

  } else {
    // Furthest left on each row
    toReturn.left = true;
    toReturn.aboveLeft = true;
    toReturn.belowLeft = true;
  }

  // Right
  if ( (tileNumber + 1) % mWidth != 0) {
    toReturn.right = mTileTypes[mTiles[tileNumber + 1].getTileType()].getSolid();

    if (above) {
      toReturn.aboveRight =  mTileTypes[mTiles[tileNumber + mWidth + 1].getTileType()].getSolid();
    }

    if (below) {
      toReturn.belowRight =  mTileTypes[mTiles[tileNumber - mWidth + 1].getTileType()].getSolid();
    }

  } else {
    // Furthest right of each row
    toReturn.right = true;
    toReturn.aboveRight = true;
    toReturn.belowRight = true;
  }

  return toReturn;
}

void Map::recalculateTileModel(irr::u32 tileNumber) {
  struct Surround surround;

  surround = calculateSurround(tileNumber);

  mTiles[tileNumber].setParent(pTileNode);
  if (mTiles[tileNumber].createModel(surround)) {
    //mTiles[tileNumber].createModel(surround);

    // Set the texture
    // TODO does this optimise out in the wash?
    //  or do we have <tile number> different textures in memory?
    //  if so, this is likely a waste in memory
    // Texture depends on 'visibility' of the tile
    if (surround.left && surround.right && surround.above && surround.below && surround.current &&
      surround.belowLeft && surround.belowRight && surround.aboveLeft && surround.aboveRight) {
      // A roof tile
      mTiles[tileNumber].setTexture(mRoofTexture);
    }

    else {
      // A normal tile
      mTiles[tileNumber].setTexture( mTileTypes[mTiles[tileNumber].getTileType()].getTextureName() );
    }

    // Set the position of the tile
    mTiles[tileNumber].setPosition(irr::core::vector3df((tileNumber%mWidth) * TILE_SIZE, 0, (tileNumber/mHeight) * TILE_SIZE));
  }

  else {
    mineTile(tileNumber);
  }
}
