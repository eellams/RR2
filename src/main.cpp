#include <iostream>
#include <fstream>
#include <string>
#include "Serialiser/Example.hpp"

#include "Serialiser/ConfigFile.hpp"
#include "Serialiser/MapFile.hpp"

int main(int argc, char *argv[]) {
  std::cout << "Program startup!" << std::endl;

  /*
  // create and open a character archive for output
  {
      std::string outFileName = "testOut.xml";
      std::ofstream ofs(outFileName.c_str());

      // create class instance
      const ToSerialise g(35, 59, 24.567f);

      // save data to archive
      {
          boost::archive::xml_oarchive oa(ofs);
          // write class instance to archive
          oa << BOOST_SERIALIZATION_NVP(g);
      }
  }

  // read
  {
      std::string inFileName = "testOut.xml";
      std::ifstream ifs(inFileName.c_str());

      {
          boost::archive::xml_iarchive ia(ifs);
          // write class instance to archive
          ToSerialise g;
          ia >> BOOST_SERIALIZATION_NVP(g);
      }
  }
  */

  std::ofstream ofs("testOut.xml");
  //const ConfigFile UserConfig(1,2);
  const MapFile Map;
  boost::archive::xml_oarchive oa(ofs);
  oa << BOOST_SERIALIZATION_NVP(Map);


  std::ifstream ifs("testIn.xml");
  MapFile readMap;
  boost::archive::xml_iarchive ia(ifs);
  ia >> BOOST_SERIALIZATION_NVP(readMap);

  


  std::cout << "Program finished!" << std::endl;

  return 0;
}
