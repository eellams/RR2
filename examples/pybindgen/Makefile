PYTHON_INCLUDE=/usr/include/python2.7
PYBINDGEN_LOCATION=../../

CC = g++

all: MyModule.so

# build my C library
libmymodule.so: my-module.o
	$(CC) -shared -o libmymodule.so my-module.o
my-module.o: my-module.hpp my-module.cpp
	$(CC) -fPIC -c -o my-module.o my-module.cpp

# generate the binding code
my-module-binding.cpp: my-module.hpp my-module.py
	PYTHONPATH=$$PYTHONPATH:$(PYBINDGEN_LOCATION) python my-module.py > my-module-binding.cpp
# build the binding code
my-module-binding.o: my-module-binding.cpp
	$(CC) -fPIC -I$(PYTHON_INCLUDE) -c -o my-module-binding.o my-module-binding.cpp
# build the final python module
MyModule.so: libmymodule.so my-module-binding.o
	$(CC) -shared -o MyModule.so -L. my-module-binding.o -lmymodule

clean:
	rm -f *.o *.so my-module-binding.cpp *~ 2>/dev/null
