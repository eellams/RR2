CC      = g++
CFLAGS  = -I/usr/include/boost -I/usr/include/python3.4 -Wno-unknown-pragmas
LDFLAGS = -lpython3.4m -lboost_python-py34 -lboost_filesystem -lboost_system

# All objects are c++ (with .cxx extension)
OBJ = main

all: config $(OBJ)

config:
	cd config && make && cd ../

#%.o: %.cpp
#	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

main: main.cpp
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

cleanconfig:
	cd config && make clean && cd ../

clean: cleanconfig
	rm -r -f $(OBJ)
