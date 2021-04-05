SOURCES := $(wildcard *.cpp)
OBJECTS := $(SOURCES:.cpp=.o)

LIBDIR   = ../lib
SLIBDIR  = ../slib
BINDIR   = ../bin

#LZ4INCLUDES := -I../Lz4/lib/
LZ4INCLUDES := -I../lz4/lib/
LZ4INCLUDES := -I../lz4/lib/

CXXFLAGS = -std=c++11

libhipo: checkdirs $(OBJECTS)
	ar cr ../lib/libhipo4.a $(OBJECTS)
#	$(CXX) -shared -fPIC  -o ../lib/libhipo.so $(OBJECTS) -llz4

slibhipo: checkdirs $(OBJECTS)
	$(CXX) -shared $(OBJECTS) -L ../slib -llz4 -o ../slib/libhipo4.so

show:
	@echo $(OBJECTS)

checkdirs: $(LIBDIR) $(SLIBDIR)

$(LIBDIR):
	@mkdir -p $(LIBDIR)

$(SLIBDIR):
	@mkdir -p $(SLIBDIR)
clean:
	@echo 'Removing the build files....'
	@rm -rf *~ *.o

%.o: %.cpp
	$(CXX) -fPIC -c $< -O2 -D__LZ4__ $(CXXFLAGS) $(LZ4INCLUDES)
