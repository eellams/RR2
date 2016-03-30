#include "config.hpp"

BOOST_PYTHON_MODULE(config)
{
  // The Configs class contains singleton
  class_<config::config_graphics>("Configs")
    .def_readwrite("GraphicsConfig", &config::configs.GraphicsConfig)
  ;

  // Python accessible parts of the graphcis config
  class_<config::config_graphics>("GraphicsConfig")
    //.def_readwrite("resX", &config::config_graphics::resX)
    //.def_readwrite("resY", &config::config_graphics::resY)
    .def("setResolution", &config::config_graphics::set_resolution, args("x", "y"), "Set the resolution")
  ;
}
