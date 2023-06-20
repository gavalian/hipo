#=====================================================================
# MAKEFILE for building ROOT CONVERTER
#=====================================================================
ROOTCFLAGS  := $(shell root-config --cflags)
ROOTLDFLAGS := $(shell root-config --ldflags)
ROOTLIBS    := $(shell root-config --libs) -lEG
ROOTGLIBS   := $(shell root-config --glibs)

HIPOCFLAGS  := -I../../hipo4
HIPOLIBS    := -L../../lib -lhipo4

LZ4LIBS     := -L../../lz4/lib -llz4
LZ4INCLUDES := -I../../lz4/lib

F77        = gfortran
CXX       := g++
CXXFLAGS  += -Wall -fPIC $(ROOTCFLAGS)
LD        := g++
LDFLAGS   := $(ROOTLDFLAGS)


all: converter benchmark

hist2root: hist2root.o
	$(CXX) -o hist2root.exe $< $(ROOTCFLAGS) $(ROOTLDFLAGS) $(HIPOLIBS) $(LZ4LIBS) $(ROOTLIBS)

converter: converter.o
	$(CXX) -o converter.exe $< $(ROOTCFLAGS) $(ROOTLDFLAGS) $(HIPOLIBS) $(LZ4LIBS) $(ROOTLIBS)

simulation2hipo: simulation2hipo.o
	$(CXX) -o simulation2hipo.exe $< $(ROOTCFLAGS) $(ROOTLDFLAGS) $(HIPOLIBS) $(LZ4LIBS) $(ROOTLIBS)

#benchmark_root: benchmark_root.o
#	$(CXX) -o benchmark_root.exe $< $(ROOTCFLAGS) $(ROOTLDFLAGS) $(HIPOLIBS) $(LZ4LIBS) $(ROOTLIBS)
#
#benchmark_hipo: benchmark_hipo.o
#	$(CXX) -o benchmark_hipo.exe $< $(ROOTCFLAGS) $(ROOTLDFLAGS) $(HIPOLIBS) $(LZ4LIBS) $(ROOTLIBS)

benchmark: benchmark.o
	$(CXX) -o benchmark.exe $< $(ROOTCFLAGS) $(ROOTLDFLAGS) $(HIPOLIBS) $(LZ4LIBS) $(ROOTLIBS)
#
#benchmark_hipo_ana: benchmark_hipo_ana.o
#	$(CXX) -o benchmark_hipo_ana.exe $< $(ROOTCFLAGS) $(ROOTLDFLAGS) $(HIPOLIBS) $(LZ4LIBS) $(ROOTLIBS)

clean:
	@echo 'Removing all build files'
	@rm -rf *.o converter.exe *~ benchmark*.exe *.class
#=====================================================================
# COMPILE RULES
#=====================================================================
%.o: %.cc
	$(CXX) -c $<  -O3 $(ROOTCFLAGS) $(HIPOCFLAGS) $(LZ4INCLUDES)

%.o:%.F
	$(F77) -c $<
