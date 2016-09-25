#ifndef _TILE_MANAGER_HPP
#define _TILE_MANAGER_HPP

#include "itiledmanager.hpp"
#include <irrlicht.h>
#include <map>
#include <vector>

class Tile;
class TileType;

class TileManager : public ITiledManager<TileType, Tile> {
public:
  TileManager();
  TileManager(const TileManager &obj);
  ~TileManager();

  irr::scene::IMetaTriangleSelector* getTileSelector() const;

  struct Surround getTileSurround(const irr::u32& tilenumber);
  irr::u32 getTileMineInto(const irr::u32 &tilenumber);
  void setTileTileType(const irr::u32& tilenumber, const irr::u32& tiletype);
  std::array<irr::f32, 4> getTileHeight(const irr::u32 &tilenumber);
  void setRoofTexture(const std::string& texture);

  virtual void add(const irr::u32 &tilenumber,
    const irr::u32 &tid,
    const std::array<irr::f32, 4> &cornerheights,
    const irr::u32 &knownid = 0) {};
  virtual void remove(const irr::u32 &id) {};

  void initialise(irr::scene::ISceneNode *parentnode);

  void recalculate(const irr::u32 &tilenumber);
  void recalculateByTileNumber(const irr::u32 &tilenumber) { recalculate(tilenumber); };

private:
  void initialiseTypes();
  void initialiseInstances();

  void createHeightMap();
  void calculateTileCorners(const irr::u32& tileNumber);

  struct Surround calculateSurround(const irr::u32& tileNumber);

  std::string mRoofTexture;
  std::vector<irr::f32> mHeightmap;

  irr::scene::IMetaTriangleSelector* pTileSelector;
};

#endif
