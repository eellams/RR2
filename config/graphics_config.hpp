#ifndef _GRAPHICS_CONFIG_H
#define _GRAPHICS_CONFIG_H

#include <boost/python.hpp>

using namespace boost::python;

namespace config {
  struct graphics_config {
    long resX;
    long resY;
  };
}

#endif
