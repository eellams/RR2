#g++ -shared -fPIC -I/usr/include/boost -I/usr/include/python3.4 hello.cpp -o hello.so -lpython3.4m -lboost_python3

CC      = g++
CFLAGS  = -shared -fPIC -I/usr/include/boost -I/usr/include/python3.4
LDFLAGS = -lpython3.4m -lboost_python3

#OUTPUTDIR = ./bin/
#BUILDDIR = ./build/
#MKDIR = mkdir -p $(OUTPUTDIR) && mkdir -p $(BUILDDIR)

# All objects are c++ (with .cxx extension)
#OBJ = main.o
OBJ = config.so

all: $(OBJ)

#config: $(OBJ)
#	$(CC)  -o $@ $^ $(LDFLAGS)

%.so: %.cpp
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -r *.so

#cleanest: clean
	#rm -r $(OUTPUTDIR)
