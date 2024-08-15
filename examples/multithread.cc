//******************************************************************
//*  ██╗  ██╗██╗██████╗  ██████╗     ██╗  ██╗    ██████╗
//*  ██║  ██║██║██╔══██╗██╔═══██╗    ██║  ██║   ██╔═████╗
//*  ███████║██║██████╔╝██║   ██║    ███████║   ██║██╔██║
//*  ██╔══██║██║██╔═══╝ ██║   ██║    ╚════██║   ████╔╝██║
//*  ██║  ██║██║██║     ╚██████╔╝         ██║██╗╚██████╔╝
//*  ╚═╝  ╚═╝╚═╝╚═╝      ╚═════╝          ╚═╝╚═╝ ╚═════╝
//************************ Jefferson National Lab (2017) ***********
//******************************************************************
//* Example program for reading HIPO-4 Files..
//* Reads the file created by writeFile program
//*--
//* Author: G.Gavalian
//*

#include <cstdlib>
#include <iostream>
#include "reader.h"
#include <unistd.h>
#include "twig.h"

hipo::readerstream stream;
std::vector<twig::h1d>   histo = twig::h1d::declare(4,120,-20.0,10.0);

/**
 * @brief Create a Frame object
 *  Creates an array of events
 * @param dataframe
 * @param size
 */
void createFrame(std::vector<hipo::event> &dataframe, int size){
     dataframe.clear(); for(int i = 0; i < size; i++) dataframe.push_back(hipo::event());
}
/**
* @brief This function will be called for all threads
 */
int function(int order){

   std::vector<hipo::event> events;
   createFrame(events,128); // 64 events in one frame,
   //hipo::banklist list = stream.reader().getBanks({"REC::Particle","REC::Event"});
   hipo::bank part (stream.dictionary().getSchema("REC::Particle"),48);

   int isAlive    = 1;
   int nProcessed = 0;
   int nElectrons = 0;

   while(isAlive==1){
      stream.pull(events);
      int nNonEmpty = 0;
      for(int e = 0; e < (int) events.size(); e++){
            if(events[e].getSize()>16) { nNonEmpty++; nProcessed ++;}
            events[e].read(part);
            if(part.getRows()>0){
	      if(part.getInt("pid",0)==11) {
		double vz = part.getFloat("vz",0);
		histo[order].fill(vz);
		nElectrons++;
	      }
            }
      }
      usleep(50000); // This is neccessary to benchmark, has to be removed in real program
      if(nNonEmpty==0) isAlive = 0;
   }
   printf(">>> thread #%3d : finished , processed %8d, electrons = %8d\n",order,nProcessed, nElectrons);
   return nProcessed;
}

int main(int argc, char** argv) {

   std::cout << " reading file example program (HIPO) "  << __cplusplus << std::endl;

   char inputFile[256];

   if(argc>1) {
     snprintf(inputFile,256,"%s",argv[1]);
      //sprintf(outputFile,"%s",argv[2]);
   } else {
      std::cout << " *** please provide a file name..." << std::endl;
     exit(1);
   }


   stream.open(inputFile);
   stream.run(function,4);

   twig::h1d h = twig::h1d::accumulate(histo);

   h.print();
   histo[0].print();

   //h.show();
   //histo[0].show();
}
//### END OF GENERATED CODE
