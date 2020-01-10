
SOURCES := $(wildcard *.cpp)
OBJECTS := $(SOURCES:.cpp=.o)

LIBDIR   = ../lib
SLIBDIR  = ../slib
BINDIR   = ../bin

#LZ4INCLUDES := -I../Lz4/lib/
LZ4INCLUDES := -I../lz4/lib/ -I../hipo4

CXXFLAGS = -std=c++11

libtrack: checkdirs $(OBJECTS)
	ar cr ../lib/libtrack.a $(OBJECTS)

slibtrack: checkdirs $(OBJECTS)
	$(CXX) -shared $(OBJECTS) -L ../lib -llz4 -o ../slib/libhipo4.so

exe: main.o
	$(CXX) -o main.exe main.o -L../lib -llz4 -lhipo4 -ltrack

ana: analyze.o
	$(CXX) -o analyze.exe analyze.o -L../lib -llz4 -lhipo4 -ltrack

show:
	@echo $(OBJECTS)

checkdirs: $(LIBDIR) $(SLIBDIR)

$(LIBDIR):
	@mkdir -p $(LIBDIR)

$(SLIBDIR):
	@mkdir -p $(SLIBDIR)
clean:
	@echo 'Removing the build files....'
	@rm -rf *~ *.o *.exe

%.o: %.cpp
	$(CXX) -fPIC -c $< -O2 -D__LZ4__ $(CXXFLAGS) $(LZ4INCLUDES)
%.o: %.cc
	$(CXX) -fPIC -c $< -O2 -D__LZ4__ $(CXXFLAGS) $(LZ4INCLUDES)
