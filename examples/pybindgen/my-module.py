import pybindgen
import sys

mod = pybindgen.Module('MyModule')
mod.add_include('"my-module.hpp"')
mod.add_function('MyModuleDoAction', None, [])

klass = mod.add_class('MyClass')
klass.add_constructor([])
klass.add_method('SetInt', None, [pybindgen.param('int', 'value')])
klass.add_method('GetInt', pybindgen.retval('int'), [], is_const=True)

mod.generate(sys.stdout)
