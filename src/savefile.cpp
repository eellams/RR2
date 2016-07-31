#include "savefile.hpp"
#include "map.hpp"

Map* SaveFile::getPMap() {
  //return mMap;
  return pMap;
}

void SaveFile::setPMap(Map *map) {
  delete pMap;
  pMap = new Map();
  std::memcpy(pMap, map, sizeof(Map));
  //*pMap = *map;
}

template<class Archive>
void SaveFile::serialize(Archive & ar, const unsigned int version)
{
  ar & boost::serialization::make_nvp("mMap", pMap);
}

template void SaveFile::serialize<boost::archive::xml_iarchive>(
    boost::archive::xml_iarchive & ar,
    const unsigned int version
);
template void SaveFile::serialize<boost::archive::xml_oarchive>(
    boost::archive::xml_oarchive & ar,
    const unsigned int version
);
