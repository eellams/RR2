#include <iostream>
#include <fstream>
#include <string>
//#include "Serialiser/Example.hpp"

//#include "Serialiser/ConfigFile.hpp"
#include "Map/Map.hpp"
#include "Serialiser/SaveFile.hpp"

#include "GeomObject.hpp"

#include <irrlicht.h>
#include "driverChoice.h"
//Namespaces for the engine
using namespace irr;
using namespace video;
using namespace core;
using namespace scene;
using namespace io;
using namespace gui;

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

  std::ifstream ifs("testIn.xml");
  SaveFile readMap;
  boost::archive::xml_iarchive ia(ifs);
  ia >> BOOST_SERIALIZATION_NVP(readMap);

  std::cout << readMap.mMap.GetName() << std::endl;

  Map& map = readMap.mMap;

  map.Initialise(driver, smgr);

  std::clog << "Finished initialising" << std::endl;

  // irrlict code

  // light is just for nice effects
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

  ICameraSceneNode* camera = smgr->addCameraSceneNodeFPS();
  if (camera)
  {
      //camera->setPosition(vector3df(-20.f, 150.f, -20.f));
      //camera->setTarget(vector3df(200.f, -80.f, 150.f));

      camera->setPosition(vector3df(10,10,0));
      camera->setTarget(vector3df(0,0,0));

      camera->setFarValue(20000.0f);
  }

  while(device->run())
    {
        if(!device->isWindowActive()) {
            device->sleep(100);
            continue;
        }

        /*
        if(receiver.IsKeyDown(irr::KEY_KEY_W))
        {
            //meshnode->setMaterialFlag(video::EMF_WIREFRAME, !meshnode->getMaterial(0).Wireframe);
        }
        else if(receiver.IsKeyDown(irr::KEY_KEY_1))
        {
            //hm.generate(eggbox);
            //mesh.init(hm, 50.f, grey, driver);
        }
        else if(receiver.IsKeyDown(irr::KEY_KEY_2))
        {
            //hm.generate(moresine);
            //mesh.init(hm, 50.f, yellow, driver);
        }
        else if(receiver.IsKeyDown(irr::KEY_KEY_3))
        {
            //hm.generate(justexp);
            //mesh.init(hm, 50.f, yellow, driver);
        }
        */

        driver->beginScene(true, true, SColor(0xff000000));
        smgr->drawAll();
        driver->endScene();
    }

  device->drop();

  std::cout << "Program finished!" << std::endl;

  return 0;
}
