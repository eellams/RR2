#include <boost/python.hpp>
#include <boost/python/object.hpp>

using namespace boost::python;

int main(int argc, char *argv[]) {

  try {
        // If you're going to use threads: PyEval_InitThreads();
        Py_Initialize();

        object main_module = import("__main__");
        object main_namespace = main_module.attr("__dict__");
        object ignore = exec("print(\"Hi!\")\n", main_namespace);

   }
   catch (error_already_set const&) {
        PyErr_Print();
   }

  return 0;
}
