#####################################################################
## EXAMPLE MAKEFILE FOR COMPILING FORTRAN INTERFACE
#####################################################################
HIPOLIBS    := -L../../lib -lhipo4
LZ4LIBS     := -L../../lz4/lib -llz4

F77        = gfortran
CXX       := g++
CXXFLAGS  += -Wall -fPIC 
LD        := g++
LDFLAGS   := 

#CERNINCLUDES = -I$(CERN_ROOT)/include/cfortran
#CERNLIBS     = -L$(CERN_ROOT)/lib -lpacklib -lgfortran
#CERNFLAGS    = -w -Df2cFortran

all: freader

freader: freader.o
	$(F77) -o freader.exe freader.o -lgcc -lstdc++ $(HIPOLIBS) $(LZ4LIBS)

clean:
	@echo 'Removing all build files....'
	@rm -rf *.o *~ freader.exe
#=====================================================================
# COMPILE RULES
#=====================================================================
%.o: %.cc
	$(CXX) -c $<  -O2 $(ROOTCFLAGS) $(HIPOCFLAGS) $(LZ4INCLUDES)

%.o:%.F
	$(F77) -c $<
