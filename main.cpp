# based on boost python tutorial: http://www.boost.org/doc/libs/1_58_0/libs/python/doc/tutorial/doc/html/python/embedding.html
# usage: g++ -I/usr/include/boost  -I/usr/include/python3.4 main.cpp -o main -lpython3.4m -lboost_python3

#include <boost/python.hpp>
#include <boost/python/object.hpp>

using namespace boost::python;

int main(int argc, char *argv[]) {
  
  try {
        // If you're going to use threads: PyEval_InitThreads();
        Py_Initialize();

        object main_module = import("__main__");
        object main_namespace = main_module.attr("__dict__");

        object ignored = exec("hello = open('hello.txt', 'w')\n"
                              "hello.write('Hello world!')\n"
                              "hello.close()",
                              main_namespace);;
   }
   catch (error_already_set const&) {
        PyErr_Print();
   }

  return 0;
}
