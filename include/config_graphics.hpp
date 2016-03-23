#ifndef _CONFIG_GRAPHIC_H
#define _CONFIG_GRAPHIC_H

#include <boost/python.hpp>

using namespace boost::python;

namespace config {
  struct config_graphics {
    long resX;
    long resY;
  };
}

#endif
