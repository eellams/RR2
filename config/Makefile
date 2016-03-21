CC      = g++
CFLAGS  = -shared -fPIC -I/usr/include/boost -I/usr/include/python3.4 -Wno-unknown-pragmas
LDFLAGS = -lpython3.4m -lboost_python3

# All objects are c++ (with .cxx extension)
OBJ = config.so

all: $(OBJ)

%.so: %.cpp
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -r -f *.so
