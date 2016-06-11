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

  //MyEventReceiver receiver;
  IrrlichtDevice* device = createDevice(driverType,
          core::dimension2du(800, 600), 32, false, false, false,
          NULL);

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

  std::cout << "Loading map " << readMap.mMap.GetName() << std::endl;

  Map& map = readMap.mMap;

  // Initialise the map (load from the save file)
  map.Initialise(driver, smgr);

  std::clog << "Finished initialising" << std::endl;

  // light is just for nice effects
  //  may not be enabled on all objects
  ILightSceneNode *node = smgr->addLightSceneNode(0, vector3df(0,100,0),
      SColorf(1.0f, 0.6f, 0.7f, 1.0f), 500.0f);

  if (node)
  {
      node->getLightData().Attenuation.set(0.f, 1.f/500.f, 0.f);
      ISceneNodeAnimator* anim = smgr->createFlyCircleAnimator(vector3df(0,150,0),250.0f);
      if (anim)
      {
          node->addAnimator(anim);
          anim->drop();
      }
  }

  /*
  ICameraSceneNode* camera = smgr->addCameraSceneNodeFPS();
  if (camera)
  {
      //camera->setPosition(vector3df(-20.f, 150.f, -20.f));
      //camera->setTarget(vector3df(200.f, -80.f, 150.f));

      camera->setPosition(vector3df(10,10,0));
      camera->setTarget(vector3df(0,0,0));

      camera->setFarValue(20000.0f);
  }
  */



  /////////
  /*
  //Triangle Selecty-ness on the background

  scene::ITriangleSelector* selector = 0;
  selector = smgr->createOctreeTriangleSelector(mesh->getMesh(0), bg, 128);
  bg->setTriangleSelector(selector);
  selector->drop();
  */

  //add RTS Camera
  RTSCamera* camera = new RTSCamera(device,smgr->getRootSceneNode(),smgr,-1,100.0f,10.0f,10.0f);
  //ICameraSceneNode *camera= smgr->addCameraSceneNodeFPS(0, 50.0f,0.2f,-1,0,0,true,0.0f,false,true);
  camera->setPosition(vector3df(0,9,-14));
  camera->setTranslateSpeed(5);//speed of cam movement
  camera->setRotationSpeed(50);//speed of cam rotation


  //Set collision for camera

  scene::ISceneNodeAnimator *collision = smgr->createCollisionResponseAnimator(
    map.GetSelector(),camera,core::vector3df(20,40,20),
    core::vector3df(0,-2,0),
    core::vector3df(0,0,0),
    0.0f);
  camera->addAnimator(collision);

  collision->drop();

 /////////


  while(device->run())
    {
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
