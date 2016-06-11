#include <iostream>
#include <fstream>
#include <string>
//#include "Serialiser/Example.hpp"

//#include "Serialiser/ConfigFile.hpp"
#include "map.hpp"
#include "savefile.hpp"
#include "rtscamera.h"

#include <irrlicht.h>
#include "driverChoice.h"
//Namespaces for the engine
using namespace irr;
using namespace video;
using namespace core;
using namespace scene;
using namespace io;
using namespace gui;

// event reciever
class MyEventReceiver : public IEventReceiver {
public:
   // This is the one method that we have to implement
   virtual bool OnEvent(const SEvent& event) {
      // Remember whether each key is down or up
      if (event.EventType == irr::EET_KEY_INPUT_EVENT)
         KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

      return false;
   }

   // This is used to check whether a key is being held down
   virtual bool IsKeyDown(EKEY_CODE keyCode) const {
      return KeyIsDown[keyCode];
   }

   MyEventReceiver() {
      for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
         KeyIsDown[i] = false;
   }

private:
   // We use this array to store the current state of each key
   bool KeyIsDown[KEY_KEY_CODES_COUNT];
};



int main(int argc, char *argv[]) {
  std::cout << "Program startup!" << std::endl;

  // Demonstrating reading and writing from a config file
  /*
  std::ofstream ofs("testOut.xml");

  Map mapp("Some Name", "Some description", 10, 10);
  SaveFile MapF(mapp);
  boost::archive::xml_oarchive oa(ofs);
  oa << BOOST_SERIALIZATION_NVP(MapF);

  ofs.flush();
  */

  MyEventReceiver receiver;

  // ask user for driver
  video::E_DRIVER_TYPE driverType=driverChoiceConsole();
  if (driverType==video::EDT_COUNT)
      return 1;

  IrrlichtDevice* device = createDevice(driverType,
          core::dimension2du(800, 600), 32, false, false, false,
          &receiver);

  if(device == 0)
      return 1;

  IVideoDriver *driver = device->getVideoDriver();
  ISceneManager *smgr = device->getSceneManager();
  device->setWindowCaption(L"Irrlicht Example for SMesh usage.");

  // Read the map file
  std::ifstream ifs("testIn.xml");
  SaveFile readMap;
  boost::archive::xml_iarchive ia(ifs);

  ia >> BOOST_SERIALIZATION_NVP(readMap);

  std::cout << "Loading map " << readMap.mMap.getName() << std::endl;

  Map& map = readMap.mMap;

  // Initialise the map (load from the save file)
  map.initialise(driver, smgr);

  std::clog << "Finished initialising" << std::endl;

  // light is just for nice effects
  //  may not be enabled on all objects
  ILightSceneNode *node = smgr->addLightSceneNode(0, vector3df(0,100,0),
      SColorf(1.0f, 0.6f, 0.7f, 1.0f), 500.0f);

  if (node) {
      node->getLightData().Attenuation.set(0.f, 1.f/500.f, 0.f);
      ISceneNodeAnimator* anim = smgr->createFlyCircleAnimator(vector3df(0,150,0),250.0f);
      if (anim) {
          node->addAnimator(anim);
          anim->drop();
      }
  }

  //add RTS Camera
  RTSCamera* camera = new RTSCamera(device,smgr->getRootSceneNode(),smgr,-1,100.0f,10.0f,10.0f);
  camera->setPosition(vector3df(0,9,-14));
  camera->setTranslateSpeed(2.5);//speed of cam movement
  camera->setRotationSpeed(50);//speed of cam rotation

  //Set collision for camera
  scene::ISceneNodeAnimator *collision = smgr->createCollisionResponseAnimator(
    map.getMetaTriangleSelectorP(),
    camera,
    core::vector3df(20,40,20),
    core::vector3df(0,-2,0),
    core::vector3df(0,0,0),
    0.0f);
  camera->addAnimator(collision);

  // Drop it, as it is no longer used
  collision->drop();

  // Collision stuff
  scene::ISceneCollisionManager* collMan = smgr->getSceneCollisionManager();

  while(device->run()) {
    if(receiver.IsKeyDown(irr::KEY_KEY_1)) {
      map.mineTile(18);
    }

    if(receiver.IsKeyDown(irr::KEY_KEY_2)) {
      map.mineTile(28);
    }

    // Set up the ray used for moving the camera over terrain
    core::line3d<f32> ray;

    // Set the ray vertical
    ray.start = camera->getTarget();
    ray.start.Y = +1000.0f;

    ray.end = camera->getTarget();
    ray.end.Y = -1000.0f;

    // Tracks the current intersection point with the level or a mesh
    core::vector3df intersection;
    // Used to show with triangle has been hit
    core::triangle3df hitTriangle;

    // This call is all you need to perform ray/triangle collision on every scene node
    // that has a triangle selector, including the Quake level mesh.  It finds the nearest
    // collision point/triangle, and returns the scene node containing that point.
    // Irrlicht provides other types of selection, including ray/triangle selector,
    // ray/box and ellipse/triangle selector, plus associated helpers.
    // See the methods of ISceneCollisionManager
    scene::ISceneNode * selectedSceneNode =
    collMan->getSceneNodeAndCollisionPointFromRay(
        ray,
        intersection, // This will be the position of the collision
        hitTriangle, // This will be the triangle hit in the collision
        COLLISION_MASK_TILE, // This ensures that only nodes that we have
                       // set up to be pickable are considered
        0); // Check the entire scene (this is actually the implicit default)

    // If the ray hit anything, move the billboard to the collision position
    // and draw the triangle that was hit.
    if(selectedSceneNode) {
      // Check that the camera hit a tile, by bitmask
      if((selectedSceneNode->getID() & COLLISION_MASK_TILE) == COLLISION_MASK_TILE) {
        // Follow the height of the terrain
        camera->setTargetHeight(intersection.Y);
      }

    }

    if(!device->isWindowActive()) {
      device->sleep(100);
      continue;
    }

    driver->beginScene(true, true, SColor(0xff000000));
    smgr->drawAll();
    driver->endScene();
  }

  device->drop();

  std::cout << "Program finished!" << std::endl;

  return 0;
}
