#include "tilemanager.hpp"
#include "tile.hpp"
#include "tiletype.hpp"

TileManager::TileManager() :
  mWidth(0),
  mHeight(0),
  mRoofTexture(),
  mTileTypes(),
  mTiles(),
  mHeightmap(),
  pTileNode(NULL),
  pTileSelector(NULL)
{

}

TileManager::~TileManager() {

}

// Used for seialisation
std::vector<Tile> TileManager::getTiles() const {
  return mTiles;
}

// Used for serialisation
std::map<irr::u32, TileType> TileManager::getTileTypes() const {
  return std::map<irr::u32, TileType>(mTileTypes);
}

// Get the tile selector for the Tiles
//  (for collisions)
irr::scene::IMetaTriangleSelector* TileManager::getTileSelector() const {
  return pTileSelector;
}

// Gets the surround info for a particular tile
struct Surround TileManager::getTileSurround(const irr::u32& tilenumber) {
  return calculateSurround(tilenumber);
}

// Get what a particular tile mines into
irr::u32 TileManager::getTileMineInto(const irr::u32 &tilenumber) {
  return mTileTypes[mTiles[tilenumber].getTileType()].getMineInto();
}

// Set the tile type of a particular tile
void TileManager::setTileTileType(const irr::u32& tilenumber, const irr::u32& tiletype) {
  mTiles[tilenumber].setTileType(tiletype);
}

// Get the maximum height of a tile
//  note, only takes base height (solid walls have no effect)
irr::f32 TileManager::getMaxTileHeight(const irr::u32 &tilenumber) {
  return mTiles[tilenumber].getCornerHeightMax();
}

std::array<irr::f32, 4> TileManager::getTileHeight(const irr::u32 &tilenumber) {
  return mTiles[tilenumber].getCornerHeights();
}

// Set tiles
//  to the values serialised from the map file
void TileManager::setTiles(const std::vector< Tile> &tiles) {
  mTiles = tiles;
}

// Set tile types
//  to the values serialised from the map file
void TileManager::setTileTypes(const std::map<irr::u32, TileType> &ttypes) {
  mTileTypes.clear();
  mTileTypes.insert(ttypes.begin(), ttypes.end());
}

// Set the map width
//  (number of tiles in the X direction)
void TileManager::setWidth(const irr::u32& width) {
  mWidth = width;
}

// Set the map height
//  (number of tiles in the Z direction)
void TileManager::setHeight(const irr::u32& height) {
  mHeight = height;
}

// Roof texture is global for all tiles
void TileManager::setRoofTexture(const std::string& texture) {
  mRoofTexture = texture;
}

// Initialises all tiles and tile types
//  from their serialised state
void TileManager::initialise(irr::scene::ISceneNode *parentnode) {
  pTileNode = parentnode->getSceneManager()->addEmptySceneNode();
  pTileNode->setParent(parentnode);

  createHeightMap();

  initialiseTileTypes();
  initialiseTiles();

  pTileSelector = pTileNode->getSceneManager()->createMetaTriangleSelector();
  for (auto child : pTileNode->getChildren()) {
    pTileSelector->addTriangleSelector(child->getTriangleSelector());
  }
}

// Recalculate a tile. This includes:
//  changing tile type as required (cave-ins etc.)
//  recalculating models as required
// Recursive, will check surrounding tiles if there is a change in a tile
void TileManager::recalculate(const irr::u32 &tilenumber, const bool &enableCaveIn) {
  struct Surround surround;

  surround = calculateSurround(tilenumber);

  if (mTiles[tilenumber].createModel(surround)) {
    // Set the texture
    //  texture depends on 'visibility' of the tile
    if (surround.left && surround.right && surround.above && surround.below && surround.current &&
      surround.belowLeft && surround.belowRight && surround.aboveLeft && surround.aboveRight) {
      // A roof tile
      mTiles[tilenumber].setTexture(mRoofTexture);
    }

    else {
      // A normal tile
      //  i.e. to be calculated by the tile
      mTiles[tilenumber].setTexture(mTileTypes[mTiles[tilenumber].getTileType()].getTextureName());
    }

    // Set the position of the tile
    mTiles[tilenumber].setPosition(irr::core::vector3df((tilenumber%mWidth) * TILE_SIZE, 0, (tilenumber/mHeight) * TILE_SIZE));
  }

  // Sometimes we don't want cave-ins
  //  (e.g. during development)
  else if (enableCaveIn) {
    // Can't make a tile - illegal layout
    //  causes 'cave-in' effect
    setTileTileType(tilenumber, getTileMineInto(tilenumber));
    recalculate(tilenumber);
  }
}

// Initialises the tiles for the first time
void TileManager::initialiseTiles() {
  std::clog << "Initilising tiles" << std::endl;
  createHeightMap();

  // For each tile, initialise
  //  initialise, set parent etc.
  for (irr::u32 i=0; i<mTiles.size(); i++) {

    // Create the tile model
    mTiles[i].initialise(pTileNode->getSceneManager(), calculateSurround(i));
    mTiles[i].setParent(pTileNode);
  }

  // Once all tiles have been initialised, then we can see what needs
  //  to cave in, etc.
  for (irr::u32 i=0; i<mTiles.size(); i++) {
    recalculate(i);
  }
}

void TileManager::initialiseTileTypes() {
  // Nothing to see here
}

// Creates the heightmap from the vector of tiles
void TileManager::createHeightMap() {
  // In case the heightmap already contains data
  //  at time of writing, can't actually see this situation arising, but better
  //  to be safe than sorry.
  mHeightmap.clear();

  // Fill the heightmap with the tile data
  for (irr::u32 i=0; i<mTiles.size(); i++) {
    mHeightmap.push_back(mTiles[i].getHeight());
    //calculateTileCorners(i);
  }

  for (irr::u32 i=0; i<mTiles.size(); i++) {
    calculateTileCorners(i);
  }
}

// Calculates whether surrounding tiles are 'solid' or not
//  if the tile is out-of-range (i.e. outside edges of map), assumes solid
struct Surround TileManager::calculateSurround(const irr::u32& tileNumber) {

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

// Caclulates the corner heights of the tile as a mean of surrounding tile
//  heights
void TileManager::calculateTileCorners(const irr::u32& tileNumber) {
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

// Converts a tile number into the bottom left position of a tile
/*irr::core::vector3df TileManager::tileNumberToPosition(const int& tilenumber) {
  irr::core::vector3df pos;
  pos.set(
    (tilenumber % mWidth)*TILE_SIZE,
    mTiles[tilenumber].getCornerHeightMax(),
    (tilenumber / mWidth)*TILE_SIZE
  );
  return pos;
}*/
