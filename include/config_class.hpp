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
#define WRAPPER_CLASS(CLASS) public CLASS, public boost::python::wrapper<CLASS>

namespace bpy = boost::python;

namespace config {
  class BaseClass {
  protected:
    BaseClass() {}
    ~BaseClass() {}

  public:
    virtual void overrideMe() { std::cout << "Base override" << std::endl; };
  };


  class BaseWrap : WRAPPER_CLASS(BaseClass) {
  public:
    BaseWrap() : BaseClass() {}
    ~BaseWrap() {}

    // Wrappers go here
    WRAPPER(void, BaseWrap, overrideMe)
  };
}

#endif
