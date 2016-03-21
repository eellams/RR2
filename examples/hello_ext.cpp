// From the boost python tutorial docs http://www.boost.org/doc/libs/1_58_0/libs/python/doc/tutorial/doc/html/index.html
//  yes, bjam doesn't work
//
// g++ -shared -fPIC -I/usr/include/boost -I/usr/include/python3.4 hello_ext.cpp -o hello_ext.so -lpython3.4m -lboost_python3

#include <boost/python.hpp>

char const* greet()
{
   return "hello, world";
}

BOOST_PYTHON_MODULE(hello_ext)
{
    using namespace boost::python;
    def("greet", greet);
}
