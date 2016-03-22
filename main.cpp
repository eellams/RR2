// based on boost python tutorial: http://www.boost.org/doc/libs/1_58_0/libs/python/doc/tutorial/doc/html/python/embedding.html
// usage: g++ -I/usr/include/boost  -I/usr/include/python3.4 main.cpp -o main -lpython3.4m -lboost_python3

#include <string>
#include <boost/python.hpp>
#include <boost/python/object.hpp>
#include <boost/filesystem.hpp>
#include <iostream>

using namespace boost::python;

int main(int argc, char *argv[]) {
  std::cout << "Program startup!" << std::endl;
  
  boost::filesystem::path basedir = boost::filesystem::system_complete(argv[0]).parent_path();

  // TODO this should be a path to a binary directory
  boost::filesystem::path configpath = basedir / boost::filesystem::path("config");

  try {
        // If you're going to use threads: PyEval_InitThreads();
        Py_Initialize();

        object main_module = import("__main__");
        object main_namespace = main_module.attr("__dict__");

        object sys = import("sys");
        object os = import("os");

        // Set up the environment variables (path and environment)
        sys.attr("path").attr("append")(configpath.string());
        os.attr("environ").attr("__setitem__")("LD_LIBRARY_PATH", configpath.string());

        // Now path has been set, we can import our own modules (yay!)
        object config = import("config");

        std::cout << "Program finished!" << std::endl;
   }
   catch (error_already_set const&) {
        PyErr_Print();
   }

  return 0;
}
