#ifndef _CONFIG_H
#define _CONFIG_H

#include "graphics_config.hpp"

namespace config {

  // configs contains all of the structures that can be changed in the config files
  struct config {
    graphics_config GraphicsConfig;
  } configs;

}

#endif
