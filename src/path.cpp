#include "path.hpp"
#include "pathtype.hpp"
#include "geomobject.hpp"

Path::Path() :
  mPathId(0),
  mPathType(0),
  mTileNumber(0),
  pGeom(NULL)
{
}

Path::Path(const irr::u32 &pid, const irr::u32 &pt, const irr::u32 &tilenumber) :
  mPathId(pid),
  mPathType(pt),
  mTileNumber(tilenumber),
  pGeom(NULL)
{
}

Path::~Path() {
  std::clog << "Deleting path " << mPathId << std::endl;
  if (pGeom != NULL) {
    pGeom->clear();
  }
}

irr::u32 Path::getPathId() const { return mPathId; }
irr::u32 Path::getPathType() const { return mPathType; }
irr::u32 Path::getTileNumber() const { return mTileNumber; }
bool Path::getPowered() const { return mPowered; }

void Path::setTexture(const std::string& tex) {
  pGeom->setTexture(tex);
}
void Path::setAlpha() {
  pGeom->setAlpha();
}

void Path::initialise(irr::scene::ISceneNode* parent, const PathType& ptype, const irr::core::vector3df& pos) {
  std::clog << "Initialising path: " << mPathId << std::endl;
  pGeom = new GeomObject();
  pGeom->initialise(parent->getSceneManager());
  pGeom->setPosition(pos);
}

void Path::createModel(const std::array<irr::f32, 4> cornerheights) {
  struct TriStrip tris;
  struct TrianglePoint tpoint;

  pGeom->clear();
  tpoint.normal.set(0,1,0); // TODO normals
  tpoint.colour = irr::video::SColor(255,255,255,255);

  tpoint.pos.set(0,cornerheights[0]+0.01,0);
  tpoint.uv.set(0,0);
  tris.points.push_back(tpoint);

  // TODO is this meant to be 0?
  tpoint.pos.set(0,cornerheights[1]+0.01,TILE_SIZE);
  tpoint.uv.set(0,1);
  tris.points.push_back(tpoint);

  tpoint.pos.set(TILE_SIZE,cornerheights[2]+0.01,TILE_SIZE);
  tpoint.uv.set(1,1);
  tris.points.push_back(tpoint);

  tpoint.pos.set(TILE_SIZE,cornerheights[3]+0.01,0);
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
