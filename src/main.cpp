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

#define RESOLUTION_X 800
#define RESOLUTION_Y 600

// event reciever
class MyEventReceiver : public irr::IEventReceiver {
public:
   // This is the one method that we have to implement
   virtual bool OnEvent(const irr::SEvent& event) {
      // Remember whether each key is down or up
      if (event.EventType == irr::EET_KEY_INPUT_EVENT)
         KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

      return false;
   }

   // This is used to check whether a key is being held down
   virtual bool IsKeyDown(irr::EKEY_CODE keyCode) const {
      return KeyIsDown[keyCode];
   }

   MyEventReceiver() {
      for (irr::u32 i=0; i<irr::KEY_KEY_CODES_COUNT; ++i)
         KeyIsDown[i] = false;
   }

private:
   // We use this array to store the current state of each key
   bool KeyIsDown[irr::KEY_KEY_CODES_COUNT];
};



int main(int argc, char *argv[]) {
  irr::video::IVideoDriver* driver;            // Driver
  irr::scene::ISceneManager* smgr;             // Scene manager
  irr::scene::ILightSceneNode *node;           // Node for the light
  irr::scene::ISceneNodeAnimator* anim;        // Node to animate light
  irr::scene::ISceneNodeAnimator *collision;   // Collision
  irr::scene::ISceneCollisionManager* collMan; // Collision
  irr::scene::ISceneNode * selectedSceneNode;  // Collided with node
  RTSCamera* camera;                           // The RTS style camera

  MyEventReceiver receiver;        // Receiver for events (keypresses etc.)
  std::ifstream ifs;               // For reading the input pMap->file
  std::ofstream ofs;               // For writing the output file
  SaveFile readMap;                // Where the file is read to
  irr::core::line3d<irr::f32> ray; // Ray used for collision

  std::cout << "Program startup!" << std::endl;

  // Demonstrating reading and writing from a config file
  /*
  std::ofstream ofs("testOut.xml");

  Map pMap->("Some Name", "Some description", 10, 10);
  SaveFile MapF(pMap->);
  boost::archive::xml_oarchive oa(ofs);
  oa << BOOST_SERIALIZATION_NVP(MapF);

  ofs.flush();
  */

  // Ask user for driver
  //  e.g. OpenGL, DirectX etc.
  irr::video::E_DRIVER_TYPE driverType = irr::driverChoiceConsole();
  if (driverType==irr::video::EDT_COUNT)
      return 1;

  // Create the device
  irr::IrrlichtDevice* device = createDevice(driverType,
          irr::core::dimension2du(RESOLUTION_X, RESOLUTION_Y), 32, false, false, false,
          &receiver);

  // If we can't create a device, stop before anything else happens
  if(device == 0)
      return 1;

  driver = device->getVideoDriver();
  smgr = device->getSceneManager();
  device->setWindowCaption(L"Irrlicht Example for SMesh usage.");

  // Read the pMap->file
  ifs.open("testIn.xml");

  // TODO this can't be declared at the top of the function
  //  due to default constructors
  //  minor annoyance
  boost::archive::xml_iarchive ia(ifs);
  ia >> BOOST_SERIALIZATION_NVP(readMap); // Read the map

  // TODO this can't be declared at the top of the function
  //Map& pMap->= readMap.mMap;
  Map *pMap = readMap.getPMap();

  // Initialise the pMap->(setup data from the save file)
  pMap->initialise(driver, smgr);

  std::clog << "Finished initialising" << std::endl;

  // Add some animated light, purely for aesthetics
  //  may not be enabled on all objects
  {
    node = smgr->addLightSceneNode(0, irr::core::vector3df(0,100,0),
        irr::video::SColorf(1.0f, 0.6f, 0.7f, 1.0f), 500.0f);

    if (node) {
        node->getLightData().Attenuation.set(0.f, 1.f/500.f, 0.f);

        anim = smgr->createFlyCircleAnimator(irr::core::vector3df(0,150,0),250.0f);
        if (anim) {
            node->addAnimator(anim);
            anim->drop();
        }
    }
  }

  // Add the RTS Camera
  {
    camera = new RTSCamera(device,smgr->getRootSceneNode(),smgr,-1,100.0f,10.0f,10.0f);
    camera->setPosition(irr::core::vector3df(0,9,-14));
    camera->setTranslateSpeed(2.5);//speed of cam movement
    camera->setRotationSpeed(50);//speed of cam rotation
  }

  //Set collision for camera
  {
    collision = smgr->createCollisionResponseAnimator(
      pMap->getMetaTriangleSelectorP(),
      camera,
      irr::core::vector3df(20,40,20),
      irr::core::vector3df(0,-2,0),
      irr::core::vector3df(0,0,0),
      0.0f);
    camera->addAnimator(collision);

    // Drop it, as it is no longer used
    collision->drop();

    // Collision stuff
    collMan = smgr->getSceneCollisionManager();
  }

  while(device->run()) {

    if (receiver.IsKeyDown(irr::KEY_ESCAPE)) {
      device->getTimer()->stop();
    }

    if(receiver.IsKeyDown(irr::KEY_KEY_1)) {
      pMap->mineTile(18);
    }

    if(receiver.IsKeyDown(irr::KEY_KEY_2)) {
      pMap->mineTile(28);
    }

    if(receiver.IsKeyDown(irr::KEY_KEY_3)) {
      pMap->mineTile(22);
    }

    if (receiver.IsKeyDown(irr::KEY_KEY_4)) {
      pMap->setTile(12, 0, true);
    }

    if (receiver.IsKeyDown(irr::KEY_KEY_5)) {
      pMap->setTile(12, 1, true);
    }

    // Collision region
    {
      // Set up the ray used for moving the camera over terrain
      // Set the ray vertical
      ray.start = camera->getTarget();
      ray.start.Y = +1000.0f;

      ray.end = camera->getTarget();
      ray.end.Y = -1000.0f;

      // Tracks the current intersection point with the level or a mesh
      irr::core::vector3df intersection;
      // Used to show with triangle has been hit
      irr::core::triangle3df hitTriangle;

      // This call is all you need to perform ray/triangle collision on every scene node
      // that has a triangle selector, including the Quake level mesh.  It finds the nearest
      // collision point/triangle, and returns the scene node containing that point.
      // Irrlicht provides other types of selection, including ray/triangle selector,
      // ray/box and ellipse/triangle selector, plus associated helpers.
      // See the methods of ISceneCollisionManager
      selectedSceneNode = collMan->getSceneNodeAndCollisionPointFromRay(
          ray,
          intersection, // This will be the position of the collision
          hitTriangle, // This will be the triangle hit in the collision
          COLLISION_MASK_TILE, // This ensures that only nodes that we have
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
    }

    // If the window is active, sleep for a bit
    //  TODO .. why is this needed?
    if(!device->isWindowActive()) {
      device->sleep(100);
      continue;
    }

    // Render the scene
    driver->beginScene(true, true, irr::video::SColor(0xff000000));
    smgr->drawAll();

    // End the scene
    driver->endScene();
  }

  // Drop the device once we're finished with it
  device->drop();

  // Write to the output file
  ofs.open("testOut.xml");
  boost::archive::xml_oarchive oa(ofs);
  readMap.setPMap(pMap);
  oa << BOOST_SERIALIZATION_NVP(readMap);
  ofs.flush();

  std::cout << "Program finished!" << std::endl;

  return 0;
}
