#ifndef _CONFIG_CLASS_H
#define _CONFIG_CLASS_H

#include <boost/python.hpp>
#include <iostream>

namespace bpy = boost::python;

namespace config {
  class BaseClass {
  protected:
    BaseClass() {}

  public:
    virtual void overrideMe() { std::cout << "Asdf-asdf-asdf" << std::endl; };
  };

  class BaseWrap : public BaseClass, public bpy::wrapper<BaseClass> {
  public:
    BaseWrap() : BaseClass() {}

    // I can't seem to ever call this function?
    //  however, it is needed?
    void overrideMe() {
      if (override overrideMe = this->get_override("overrideMe"))
        overrideMe(); // *note*
      return BaseClass::overrideMe();
    }

    // What is done by default?
    void default_overrideMe() { return this->BaseClass::overrideMe(); }

  };
}

#endif
