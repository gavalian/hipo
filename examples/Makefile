#********************************************************
# MAKEFILE for BUILDING EXAMPLES FOR HIPO4 LIBRARY
# AUTHOR: GAVALIAN DATE: 10/24/2018
#********************************************************
HIPOCFLAGS  := -I../hipo4
HIPOLIBS    := -L../lib -lhipo4

LZ4LIBS     := -L../lz4/lib -llz4
LZ4INCLUDES := -I../lz4/lib

CXX       := g++
CXXFLAGS  += -Wall -fPIC -std=c++11
LD        := g++
LDFLAGS   :=


all:  writeFile readFile readFileTags showFile schema

readEvents: readEvents.o
	$(CXX) -o readEvents $<  $(HIPOLIBS) $(LZ4LIBS)

writeEvents: writeEvents.o
	$(CXX) -o writeEvents $<   $(HIPOLIBS) $(LZ4LIBS)

showFile: showFile.o
	$(CXX) -o showFile $<   $(HIPOLIBS) $(LZ4LIBS)

readFile: readFile.o
	$(CXX) -o readFile $<   $(HIPOLIBS) $(LZ4LIBS)

readFileTags: readFileTags.o
	$(CXX) -o readFileTags $<   $(HIPOLIBS) $(LZ4LIBS)

writeFile: writeFile.o
	$(CXX) -o writeFile $<   $(HIPOLIBS) $(LZ4LIBS)

schema: schema.o
	$(CXX) -o schema $<   $(HIPOLIBS) $(LZ4LIBS)

builder: builder.o
	$(CXX) -o builder $<   $(HIPOLIBS) $(LZ4LIBS)

#analysis: analysis.o
#	$(CXX) -o analysis $<   $(HIPOLIBS) $(LZ4LIBS)

#clas12event_example: clas12event_example.o
#	$(CXX) -o clas12event_example $<   $(HIPOLIBS) $(LZ4LIBS)

clean:
	@echo 'Removing all build files'
	@rm -rf *.o readEvents schema *~ builder writeEvents writeFile readFile

%.o: %.cc
	$(CXX) -c $< -O2 $(CXXFLAGS) $(HIPOCFLAGS) $(LZ4INCLUDES)
