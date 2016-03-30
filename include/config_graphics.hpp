#ifndef _CONFIG_GRAPHIC_H
#define _CONFIG_GRAPHIC_H

#include <boost/python.hpp>
#include <iostream>

using namespace boost::python;

namespace config {

  struct config_graphics {
  public:
    // Python accessible function
    void set_resolution(int x, int y) {
      this->resX = x;
      this->resY = y;
    }

    // C++ only python
    void print_resolution() {
      std::cout << this->resX << ", " << this->resY << std::endl;
    }

  // Private stuff, not to be touched by anyone!
  private:
    long _resX;
    long _resY;
  };


}

#endif
