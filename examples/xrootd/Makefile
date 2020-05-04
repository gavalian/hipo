#********************************************************
# MAKEFILE for BUILDING EXAMPLES FOR HIPO4 LIBRARY
# AUTHOR: GAVALIAN DATE: 10/24/2018
#********************************************************
HIPOCFLAGS  := -I../../hipo4
HIPOLIBS    := -L../../lib -lhipo4

LZ4LIBS     := -L../../lz4/lib -llz4
LZ4INCLUDES := -I../../lz4/lib

XROOTDINCLUDES := -I/usr/local/Cellar/xrootd/4.10.1/include/xrootd
XROOTDLIBS     := -L/usr/local/lib -lXrdClient

CXX       := g++
CXXFLAGS  += -Wall -fPIC -std=c++11
LD        := g++
LDFLAGS   :=


all:  readFileXrootd

readFileXrootd: readFileXrootd.o
	$(CXX) -o readFileXrootd $<   $(HIPOLIBS) $(LZ4LIBS) $(XROOTDLIBS)


#analysis: analysis.o
#	$(CXX) -o analysis $<   $(HIPOLIBS) $(LZ4LIBS)

#clas12event_example: clas12event_example.o
#	$(CXX) -o clas12event_example $<   $(HIPOLIBS) $(LZ4LIBS)

clean:
	@echo 'Removing all build files'
	@rm -rf *.o readEvents schema *~ builder writeEvents writeFile readFile

%.o: %.cc
	$(CXX) -c $< -O2 $(CXXFLAGS) $(HIPOCFLAGS) $(LZ4INCLUDES) $(XROOTDINCLUDES)
