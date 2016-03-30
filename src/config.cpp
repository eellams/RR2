#include "config.hpp"

BOOST_PYTHON_MODULE(config)
{
  // The Configs class contains singleton
  class_<config::config_graphics>("Configs")
    .def_readwrite("GraphicsConfig", &config::configs.GraphicsConfig)

    .def_readwrite("ClassConfig", &config::configs.ClassConfig)
  ;

  // Python accessible parts of the graphcis config
  class_<config::config_graphics>("GraphicsConfig")
    .def("setResolution", &config::config_graphics::set_resolution, args("x", "y"), "Set the resolution")
    .def("overrideMe", pure_virtual(&config::config_graphics::override_me))
  ;

  class_<config::BaseWrap, boost::noncopyable>("BaseClass")
    .def("overrideMe", &config::BaseClass::overrideMe, &config::BaseWrap::default_overrideMe)
    ;
}
