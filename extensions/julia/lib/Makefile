SOURCES := $(wildcard *.cc)
OBJECTS := $(SOURCES:.cc=.o)

LIBDIR   = ../lib
SLIBDIR  = ../slib
BINDIR   = ../bin

#LZ4INCLUDES := -I../Lz4/lib/
HIPOINCLUDES := -I../../../hipo4
LZ4INCLUDES := -I../../../lz4/lib/

HIPOLIBS    := -L../../../lib -lhipo4
LZ4LIBS     := -L../../../lz4/lib -llz4

CXXFLAGS = -std=c++17


slibhipo: $(OBJECTS)
	$(CXX) -shared $(OBJECTS) -o ../libjulia.so $(HIPOLIBS) $(LZ4LIBS)

show:
	@echo $(OBJECTS)

clean:
	@echo 'Removing the build files....'
	@rm -rf *~ *.o lib*.so

%.o: %.cc
	$(CXX) -fPIC -c $< -O3  $(CXXFLAGS) $(LZ4INCLUDES) $(HIPOINCLUDES)
