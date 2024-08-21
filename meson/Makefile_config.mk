ifndef HIPO
$(error "ERROR: HIPO environment variable not set; please source one of 'libexec/this_hipo.*', found within your installation prefix")
endif

HIPOCFLAGS  := -I${HIPO}/include/hipo4
HIPOLIBS    := -L${HIPO}/lib -lhipo4

LZ4LIBS     := -L$(shell pkg-config liblz4 --variable libdir) -llz4 -lpthread
LZ4INCLUDES := -I$(shell pkg-config liblz4 --variable includedir)
