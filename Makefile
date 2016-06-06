# Based off of Hiltmon's lovely Makefile
# http://hiltmon.com/blog/2013/07/03/a-simple-c-plus-plus-project-structure/

#
# TODO: Move `libmongoclient.a` to /usr/local/lib so this can work on production servers
#

CC := g++ # This is the main compiler
# CC := clang --analyze # and comment out the linker last line for sanity
SRCDIR := src
BUILDDIR := build
BINDIR := bin
LIBDIR := lib
TARGET := RR2

SRCEXT := cpp
#SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
SOURCEMAIN := src/main.cpp
SOURCES2 := $(filter-out $(SOURCEMAIN), $(SOURCES))
SOURCEDIRS := $(shell find $(SRCDIR) -type d)
OBJECTS := $(patsubst $(SRCDIR)/%, $(BUILDDIR)/%, $(SOURCES:.$(SRCEXT)=.o))

BUILDDIRS := $(SOURCEDIRS:$(SRCDIR)%=$(BUILDDIR)%)

CFLAGS := -std=c++11
LIB := -pedantic -pthread -lboost_serialization -lboost_system -lIrrlicht -lGL -lXxf86vm -lXext -lX11 -lXcursor

INC := -Iinclude

$(TARGET): $(OBJECTS) $(LIBTARGETS)
	@echo " Linking..."
	@mkdir -p $(BINDIR)
	@touch $(BINDIR)/$(TARGET)
	@echo " $(CC) $(CFLAGS) $(INC) $^ $(SOURCES) -o $(BINDIR)/$(TARGET) $(LIB)"; $(CC) $(CFLAGS) $(INC) $^ $(SOURCES2) -o $(BINDIR)/$(TARGET) $(LIB)

$(BUILDDIR)/%.o: $(SOURCES) #$(SRCDIR)/%.$(SRCEXT)
	@echo " Building..."
	@mkdir -p $(BUILDDIRS)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	@echo " Cleaning...";
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(BINDIR)/$(TARGET)

# Tests
#tester:
#  $(CC) $(CFLAGS) test/tester.cpp $(INC) $(LIB) -o bin/tester

# Spikes
#ticket:
#  $(CC) $(CFLAGS) spikes/ticket.cpp $(INC) $(LIB) -o bin/ticket

.PHONY: clean
.PHONY: mklibs
