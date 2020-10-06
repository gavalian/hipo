#********************************************************
# MAKEFILE for BUILDING EXAMPLES FOR HIPO4 LIBRARY
# AUTHOR: GAVALIAN DATE: 10/24/2018
#********************************************************
HIPOCFLAGS  := -I../../hipo4
HIPOLIBS    := -L../../lib -lhipo4

LZ4LIBS     := -L../../lz4/lib -llz4
LZ4INCLUDES := -I../../lz4/lib

CXX       := g++
CXXFLAGS  += -Wall -fPIC -std=c++11
LD        := g++
LDFLAGS   :=


all:  map

map: map_benchmark.o
	$(CXX) -o map_benchmark.exe $<  $(HIPOLIBS) $(LZ4LIBS)

ana: ana_benchmark.o
	$(CXX) -o ana_benchmark.exe $<  $(HIPOLIBS) $(LZ4LIBS)
#analysis: analysis.o
#	$(CXX) -o analysis $<   $(HIPOLIBS) $(LZ4LIBS)

#clas12event_example: clas12event_example.o
#	$(CXX) -o clas12event_example $<   $(HIPOLIBS) $(LZ4LIBS)

clean:
	@echo 'Removing all build files'
	@rm -rf *.o *~ *.exe

%.o: %.cc
	$(CXX) -O2 -c $< $(CXXFLAGS) $(HIPOCFLAGS) $(LZ4INCLUDES)
