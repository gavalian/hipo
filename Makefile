#***********************************************************
# MAKEFILE for BUILDING EXAMPLES FOR HIPO4 LIBRARY         *
# AUTHOR: GAVALIAN DATE: 10/24/2018                        *
#***********************************************************
#   ██╗  ██╗██╗██████╗  ██████╗     ██╗  ██╗    ██████╗    *
#   ██║  ██║██║██╔══██╗██╔═══██╗    ██║  ██║   ██╔═████╗   *
#   ███████║██║██████╔╝██║   ██║    ███████║   ██║██╔██║   *
#   ██╔══██║██║██╔═══╝ ██║   ██║    ╚════██║   ████╔╝██║   *
#   ██║  ██║██║██║     ╚██████╔╝         ██║██╗╚██████╔╝   *
#   ╚═╝  ╚═╝╚═╝╚═╝      ╚═════╝          ╚═╝╚═╝ ╚═════╝    *
#   === Jefferson National Lab (2017)                      *
#***********************************************************
#
SHAREDEXT=so

ifeq ($(OSTYPE),darwin)
  SHAREDEXT=dylib
endif

all: 
	@test -d lib || mkdir -p lib
	@cd lz4 ; make lz4
	@cp lz4/lib/liblz4.a lib/.
	@cd hipo4; make
shlib:
	@echo compiling shared library
	@test -d slib || mkdir -p slib
	@cd lz4 ; make
	@test -f lz4/lib/liblz4.$(SHAREDEXT) && cp lz4/lib/liblz4.$(SHAREDEXT) slib/.
#	@test -f lz4/lib/liblz4.so && cp lz4/lib/liblz4.so lib/.
	@cd hipo4 ; make slibhipo

clean:
	@cd lz4; make clean
	@cd hipo4; make clean
	@cd examples; make clean
	@cd examples/root ; make clean
	@echo "Cleaning lib directory"
	@rm -rf lib/* slib lib
