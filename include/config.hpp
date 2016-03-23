#ifndef _CONFIG_H
#define _CONFIG_H

#include "config_graphics.hpp"

namespace config {

  // configs contains all of the structures that can be changed in the config files
  struct config {
    config_graphics GraphicsConfig;
  } configs;

}

#endif
