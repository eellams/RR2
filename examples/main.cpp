// based on boost python tutorial: http://www.boost.org/doc/libs/1_58_0/libs/python/doc/tutorial/doc/html/python/embedding.html
// usage: g++ -I/usr/include/boost  -I/usr/include/python3.4 main.cpp -o main -lpython3.4m -lboost_python3

#include <boost/python.hpp>
#include <boost/python/object.hpp>

using namespace boost::python;

int main(int argc, char *argv[]) {

  try {
        // If you're going to use threads: PyEval_InitThreads();
        Py_Initialize();

        object main_module = import("__main__");
        object main_namespace = main_module.attr("__dict__");

        /*object ignored = exec("hello = open('hello.txt', 'w')\n"
                              "hello.write('Hello world!')\n"
                              "hello.close()",
                              main_namespace);;*/
        object test = exec("import sys; import hello;"
                          "sys.path.append('/home/eddie/projects/RR2');", main_namespace);

        object planet = exec("planet = hello.World()", main_namespace);
                          /*"import hello;"
                          "planet = hello.World();"
                          "planet.set('howdy');"
                          "print(planet.greet());", main_namespace);*/

        object test2 = exec("class asdf(hello.World):\n"
                          "\tdef greet(self):\n"
                          "\treturn \"redefined\"\n\n", main_namespace );
   }
   catch (error_already_set const&) {
        PyErr_Print();
   }

  return 0;
}
