#ifndef _CONFIG_CLASS_H
#define _CONFIG_CLASS_H

#include <boost/python.hpp>
#include <iostream>

#define STRINGIFY(x) #x
#define WRAPPER(TYPE, CLASS, FUNCTION, ARGS...) TYPE FUNCTION(ARGS) { \
    if (override FUNCTION = this->get_override( STRINGIFY(FUNCTION) )) FUNCTION(ARGS); \
    return CLASS::FUNCTION(ARGS); \
  } \
  TYPE default_##NAME(ARGS) { return this->CLASS::FUNCTION(ARGS); }

namespace bpy = boost::python;

namespace config {
  class BaseClass {
  protected:
    BaseClass() {}

  public:
    virtual void overrideMe() { std::cout << "Base override" << std::endl; };
  };


  class BaseWrap : public BaseClass, public bpy::wrapper<BaseClass> {
  public:
    BaseWrap() : BaseClass() {}
    WRAPPER(void, BaseWrap, overrideMe)
  };
}

#endif
