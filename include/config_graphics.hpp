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
      this->_resX = x;
      this->_resY = y;
    }

    // C++ only python
    void print_resolution() {
      std::cout << this->_resX << ", " << this->_resY << std::endl;
    }

    virtual void override_me() {};

  // Private stuff, not to be touched by anyone!
  private:
    long _resX;
    long _resY;
  };


}

#endif
