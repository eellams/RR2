#include "my-module.hpp"

#include <stdio.h>

void MyModuleDoAction (void)
{
  printf ("You called MyModuleDoAction !\n");
}

void MyClass::SetInt(int value) {
  _i = value;
}

int MyClass::GetInt() const {
  return _i;
}
