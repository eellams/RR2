# Based off of Hiltmon's lovely Makefile
# http://hiltmon.com/blog/2013/07/03/a-simple-c-plus-plus-project-structure/

#
# TODO: Move `libmongoclient.a` to /usr/local/lib so this can work on production servers
#

CC := g++ # This is the main compiler
# CC := clang --analyze # and comment out the linker last line for sanity
SRCDIR := src
BUILDDIR := build
LIBDIR := lib
TARGET := bin/RR2

SRCEXT := cpp
#SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
SOURCES := src/main.cpp
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))

LIBSOURCES := src/config.cpp
#LIBOBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(LIBSOURCES:.$(SRCEXT)=.so))
LIBTARGETS := $(patsubst $(SRCDIR)/%,$(LIBDIR)/%,$(LIBSOURCES:.$(SRCEXT)=.so))

CFLAGS := #-g # -Wall
#LIB := -pthread -lmongoclient -L lib -lboost_thread-mt -lboost_filesystem-mt -lboost_system-mt
LIB := -lpython3.4m -lboost_python-py34 -lboost_filesystem -lboost_system -lboost_timer
INC := -I include -I/usr/include/python3.4

#meh:
#	@echo "$(OBJECTS)";

$(TARGET): $(OBJECTS) $(LIBTARGETS)
	@echo " Linking..."
	@echo " $(CC) $^ -o $(TARGET) $(LIB)"; $(CC) $^ -o $(TARGET) $(LIB)

$(BUILDDIR)/%.o: $(SOURCES) #$(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

$(LIBTARGETS): $(LIBSOURCES)
	@echo " Making libs...";
	@echo " $(CC) -shared -fPIC $(CFLAGS) $(INC) -o $@ $< $(LIB)";
	$(CC) -shared -fPIC $(CFLAGS) $(INC) -o lib/config.so src/config.cpp $(LIB)

clean:
	@echo " Cleaning...";
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)

# Tests
#tester:
#  $(CC) $(CFLAGS) test/tester.cpp $(INC) $(LIB) -o bin/tester

# Spikes
#ticket:
#  $(CC) $(CFLAGS) spikes/ticket.cpp $(INC) $(LIB) -o bin/ticket

.PHONY: clean
.PHONY: mklibs
