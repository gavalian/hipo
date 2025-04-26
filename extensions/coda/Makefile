#********************************************************
# MAKEFILE for BUILDING EXAMPLES FOR HIPO4 LIBRARY
# AUTHOR: GAVALIAN DATE: 10/24/2018
#********************************************************

include ../../meson/Makefile_config.mk

EVIOCFLAGS  := -Ievio

CXX       := g++
CC        := gcc
CXXFLAGS  += -Wall -fPIC -std=c++11
LD        := g++
LDFLAGS   :=

OBJECTS = translate.o table.o decoder.o evioBankUtil.o

all:  translate

translate: $(OBJECTS)
	$(CXX) -o translate.exe $(OBJECTS) $(HIPOLIBS) $(LZ4LIBS)

#analysis: analysis.o
#	$(CXX) -o analysis $<   $(HIPOLIBS) $(LZ4LIBS)

#clas12event_example: clas12event_example.o
#	$(CXX) -o clas12event_example $<   $(HIPOLIBS) $(LZ4LIBS)

clean:
	@echo 'Removing all build files'
	@rm -rf *.o *~ *.exe example*hipo 

%.o: %.cc
	$(CXX) -c $< -O3 $(CXXFLAGS) $(HIPOCFLAGS) $(EVIOCFLAGS) $(LZ4INCLUDES)
%.o: %.c
	$(CC) -c $< -O3 $(HIPOCFLAGS) $(EVIOCFLAGS) $(LZ4INCLUDES)
