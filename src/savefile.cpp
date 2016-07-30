#include "savefile.hpp"
#include "map.hpp"

template<class Archive>
void SaveFile::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_NVP(mMap);
  //ar & boost::serialization::make_nvp("mMap", *pMap);
}

template void SaveFile::serialize<boost::archive::xml_iarchive>(
    boost::archive::xml_iarchive & ar,
    const unsigned int version
);
template void SaveFile::serialize<boost::archive::xml_oarchive>(
    boost::archive::xml_oarchive & ar,
    const unsigned int version
);
