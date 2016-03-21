#include "config.hpp"

BOOST_PYTHON_MODULE(config)
{
  // The Configs class contains singleton
  class_<config::config>("Configs")
    .def_readwrite("GraphicsConfig", &config::configs.GraphicsConfig)
  ;

  // Python accessible parts of the graphcis config
  class_<config::graphics_config>("GraphicsConfig")
    .def_readwrite("resX", &config::graphics_config::resX)
    .def_readwrite("resY", &config::graphics_config::resY)
  ;
}
