#include "path.hpp"
#include "pathtype.hpp"
#include "geomobject.hpp"

Path::Path() :
  //mPathId(0),
  mPathType(0),
  mTileNumber(0),
  mCornerHeights(),
  pGeom(NULL)
{
  pGeom = new GeomObject();
}

Path::~Path() {
  //delete pGeom;
}

irr::u32 Path::getPathType() const { return mPathType; }
irr::u32 Path::getTileNumber() const { return mTileNumber; }
bool Path::getPowered() const { return mPowered; }

void Path::setCornerHeights(const std::array<irr::f32, 4>& cornerHeights) {
  mCornerHeights = cornerHeights;
}
void Path::setTexture(const std::string& tex) {
  pGeom->setTexture(tex);
}
void Path::setAlpha() {
  pGeom->setAlpha();
}

void Path::initialise(irr::scene::ISceneNode* parent, const PathType& ptype, const irr::core::vector3df& pos) {
  pGeom->initialise(parent->getSceneManager());
  pGeom->setPosition(pos);
}

void Path::createModel() {
  struct TriStrip tris;
  struct TrianglePoint tpoint;

  pGeom->clear();
  tpoint.normal.set(0,1,0); // TODO normals
  tpoint.colour = irr::video::SColor(255,255,255,255);

  tpoint.pos.set(0,mCornerHeights[0]+0.01,0);
  tpoint.uv.set(0,0);
  tris.points.push_back(tpoint);

  tpoint.pos.set(0,mCornerHeights[0]+0.01,TILE_SIZE);
  tpoint.uv.set(0,1);
  tris.points.push_back(tpoint);

  tpoint.pos.set(TILE_SIZE,mCornerHeights[2]+0.01,TILE_SIZE);
  tpoint.uv.set(1,1);
  tris.points.push_back(tpoint);

  tpoint.pos.set(TILE_SIZE,mCornerHeights[3]+0.01,0);
  tpoint.uv.set(1,0);
  tris.points.push_back(tpoint);

  pGeom->addTriStrip(tris, 1);
}

void Path::turnOff() {
  mConducting = false;
}

void Path::turnOn() {
  mConducting = true;
}
