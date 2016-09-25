#ifndef _TILED_MANAGER_HPP
#define _TILED_MANAGER_HPP

#include <irrlicht.h>
#include <map>

#include "generic.hpp"

template<class Type, class Instance>
class ITiledManager {
public:
  ITiledManager() :
    mId(0),
    mTypes(),
    mInstances(),
    pNode(NULL)
  {
  }

  ITiledManager(const ITiledManager &obj) :
    mId(obj.mId),
    mTypes(obj.mTypes),
    mInstances(obj.mInstances),
    pNode(NULL)
  {
  }

  ~ITiledManager() {
    if (pNode) {
      pNode->removeAll();
      pNode->remove();
    }
  }

  void setWidth(const irr::u32 &width) {
    mWidth = width;
  }

  void setHeight(const irr::u32 &height) {
    mHeight = height;
  }

  void setTypes(const std::map<irr::u32, Type> &types) {
    mTypes = types;
  }

  void setInstances(const std::map<irr::u32, Instance> &instances) {
    mInstances = instances;
  }

  std::map<irr::u32, Type> getTypes() const {
    return mTypes;
  }

  std::map<irr::u32, Instance> getInstances() const {
    return mInstances;
  }

  virtual void initialise(irr::scene::ISceneNode *parentnode) {
    pNode = parentnode->getSceneManager()->addEmptySceneNode();
    pNode->setParent(parentnode);

    initialiseTypes();
    initialiseInstances();
  }

  virtual void add(const irr::u32 &tilenumber,
    const irr::u32 &tid,
    const std::array<irr::f32, 4> &cornerheights,
    const irr::u32 &knownid = 0) = 0;
  virtual void remove(const irr::u32 &id) = 0;
  // TODO is this needed?
  //virtual void removeByTileNumber(const irr::u32 &tilenumber) = 0;
  virtual void recalculate(const irr::u32 &id) = 0;
  virtual void recalculateByTileNumber(const irr::u32 &tilenumber) = 0;

protected:
  virtual void initialiseTypes() = 0;
  virtual void initialiseInstances() = 0;

  irr::core::vector3df tileNumberToPosition(const irr::u32 &tilenumber) {
    irr::core::vector3df pos;
    pos.set(
      (tilenumber % mWidth)*TILE_SIZE,
      0,
      (tilenumber / mWidth)*TILE_SIZE
    );

    return pos;
  }

  // Returns a list of the tile numbers of the tiles surrounding a particular tile
  std::vector<irr::u32> getSurroundingTileNumbers(const irr::u32& tileNumber) {
    std::vector<irr::u32> toreturn;

    bool above = false;
    bool below = false;
    bool left = false;
    bool right = false;

    // Tile below
    if (tileNumber >= mWidth) {
      below = true;
    }

    if (tileNumber < (mHeight-1)*mWidth) {
      below = true;
    }

    if ((tileNumber > 0) && (tileNumber % mWidth != 0)) {
      left = true;
    }

    if ((tileNumber + 1) % mWidth != 0) {
      right = true;
    }

    if (above) {
      toreturn.push_back(tileNumber + mWidth);
      if (left) toreturn.push_back(tileNumber + mWidth - 1);
      if (right) toreturn.push_back(tileNumber + mWidth + 1);
    }

    if (below) {
      toreturn.push_back(tileNumber - mWidth);
      if (left) toreturn.push_back(tileNumber - mWidth - 1);
      if (right) toreturn.push_back(tileNumber - mWidth + 1);
    }

    if (left) toreturn.push_back(tileNumber - 1);

    if (right) toreturn.push_back(tileNumber + 1);

    return toreturn;
  }

  irr::u32 mWidth;
  irr::u32 mHeight;
  irr::u32 mId;

  std::map<irr::u32, Type> mTypes;
  std::map<irr::u32, Instance> mInstances;

  irr::scene::ISceneNode *pNode;
};

#endif
