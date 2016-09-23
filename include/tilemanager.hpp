#ifndef _TILE_MANAGER_HPP
#define _TILE_MANAGER_HPP

#include <irrlicht.h>
#include <map>
#include <vector>

class Tile;
class TileType;

class TileManager {
public:
  TileManager();
  ~TileManager();

  std::vector<Tile> getTiles() const;
  std::map<irr::u32, TileType> getTileTypes() const;
  irr::scene::IMetaTriangleSelector* getTileSelector() const;

  struct Surround getTileSurround(const irr::u32& tilenumber);
  irr::u32 getTileMineInto(const irr::u32 &tilenumber);
  void setTileTileType(const irr::u32& tilenumber, const irr::u32& tiletype);
  irr::f32 getMaxTileHeight(const irr::u32 &tilenumber);

  void setTiles(const std::vector<Tile> &tiles);
  void setTileTypes(const std::map<irr::u32, TileType> &ttypes);

  void setWidth(const irr::u32& width);
  void setHeight(const irr::u32& height);
  void setRoofTexture(const std::string& texture);

  void initialise(irr::scene::ISceneNode *parentnode);
  void recalculate(const irr::u32 &tilenumber, const bool &enableCaveIn = true);

private:
  void initialiseTiles();
  void initialiseTileTypes();

  void createHeightMap();
  void calculateTileCorners(const irr::u32& tileNumber);

  struct Surround calculateSurround(const irr::u32& tileNumber);
  irr::core::vector3df tileNumberToPosition(const int& tilenumber);

  irr::u32 mWidth;
  irr::u32 mHeight;
  std::string mRoofTexture;
  std::map<irr::u32, TileType> mTileTypes;
  std::vector<Tile> mTiles;
  std::vector<irr::f32> mHeightmap;

  irr::scene::ISceneNode *pTileNode;
  irr::scene::IMetaTriangleSelector* pTileSelector;
};

#endif
