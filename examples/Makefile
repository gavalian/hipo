#********************************************************
# MAKEFILE for BUILDING EXAMPLES FOR HIPO4 LIBRARY
# AUTHOR: GAVALIAN DATE: 10/24/2018
#********************************************************
HIPOCFLAGS  := -I../hipo4 -I../hipo4/chart
HIPOLIBS    := -L../lib -lhipo4

LZ4LIBS     := -L../lz4/lib -llz4
LZ4INCLUDES := -I../lz4/lib

CXX       := g++
CXXFLAGS  += -Wall -fPIC -std=c++17
LD        := g++
LDFLAGS   :=


all:  writeFile readFile readFileTags showFile schema writeUserHeader tupleFile readJson dataframe histograms bankRowList

histograms: histograms.o
	$(CXX) -o histograms.exe $<  $(HIPOLIBS) $(LZ4LIBS)

bankRowList: bankRowList.o
	$(CXX) -o bankRowList.exe $<  $(HIPOLIBS) $(LZ4LIBS)

readEvents: readEvents.o
	$(CXX) -o readEvents.exe $<  $(HIPOLIBS) $(LZ4LIBS)

node: node.o
	$(CXX) -o node.exe $<  $(HIPOLIBS) $(LZ4LIBS)

dataframe: dataframe.o
	$(CXX) -o dataframe.exe $<  $(HIPOLIBS) $(LZ4LIBS)

readHist: readHist.o
	$(CXX) -o readHist.exe $<  $(HIPOLIBS) $(LZ4LIBS)

writeEvents: writeEvents.o
	$(CXX) -o writeEvents.exe $<   $(HIPOLIBS) $(LZ4LIBS)

writeUserHeader: writeUserHeader.o
	$(CXX) -o writeUserHeader.exe $<   $(HIPOLIBS) $(LZ4LIBS)

showFile: showFile.o
	$(CXX) -o showFile.exe $<   $(HIPOLIBS) $(LZ4LIBS)

readFileDebug: readFileDebug.o
	$(CXX) -o readFileDebug.exe $<   $(HIPOLIBS) $(LZ4LIBS)

readJson: readJson.o
	$(CXX) -o readJson.exe $<   $(HIPOLIBS) $(LZ4LIBS)

readFile: readFile.o
	$(CXX) -o readFile.exe $<   $(HIPOLIBS) $(LZ4LIBS)

readFusion: readFusion.o
	$(CXX) -o readFusion.exe $<   $(HIPOLIBS) $(LZ4LIBS)

readFileTags: readFileTags.o
	$(CXX) -o readFileTags.exe $<   $(HIPOLIBS) $(LZ4LIBS)

tupleFile: tupleFile.o
	$(CXX) -o tupleFile.exe $<   $(HIPOLIBS) $(LZ4LIBS)

writeFile: writeFile.o
	$(CXX) -o writeFile.exe $<   $(HIPOLIBS) $(LZ4LIBS)

writeFileComposite: writeFileComposite.o
	$(CXX) -o writeFileComposite.exe $<   $(HIPOLIBS) $(LZ4LIBS)

schema: schema.o
	$(CXX) -o schema.exe $<   $(HIPOLIBS) $(LZ4LIBS)

builder: builder.o
	$(CXX) -o builder.exe $<   $(HIPOLIBS) $(LZ4LIBS)

#analysis: analysis.o
#	$(CXX) -o analysis $<   $(HIPOLIBS) $(LZ4LIBS)

#clas12event_example: clas12event_example.o
#	$(CXX) -o clas12event_example $<   $(HIPOLIBS) $(LZ4LIBS)

clean:
	@echo 'Removing all build files'
	@rm -rf *.o *~ *.exe example*hipo 

%.o: %.cc
	$(CXX) -c $< -O2 $(CXXFLAGS) $(HIPOCFLAGS) $(LZ4INCLUDES)
