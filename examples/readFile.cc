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



int main(int argc, char** argv) {

   std::cout << " reading file example program (HIPO) "  << __cplusplus << std::endl;

   char inputFile[256];

   if(argc>1) {
      sprintf(inputFile,"%s",argv[1]);
      //sprintf(outputFile,"%s",argv[2]);
   } else {
      std::cout << " *** please provide a file name..." << std::endl;
     exit(0);
   }

   hipo::reader  reader;
   reader.open(inputFile);
   hipo::dictionary  factory;
   reader.readDictionary(factory);

   factory.show();

   hipo::bank  particles(factory.getSchema("event::particle"));
   hipo::bank  detectors(factory.getSchema("event::detector"));

   hipo::event      event;

   int counter = 0;

   reader.gotoEvent(561);

   while(reader.next()==true){
      reader.read(event);

      event.getStructure(particles);
      event.getStructure(detectors);

      particles.show();
      detectors.show();

      int nrows = particles.getRows();
      printf("---------- PARTICLE BANK CONTENT -------\n");
      for(int row = 0; row < nrows; row++){
         int   pid = particles.getInt("pid",row);
         float  px = particles.getFloat("px",row);
         float  py = particles.getFloat("py",row);
         float  pz = particles.getFloat("pz",row);
         printf("%6d : %6d %8.4f %8.4f %8.4f\n",row,pid,px,py,pz);
      }
      printf("---------- END OF PARTICLE BANK -------\n");

      counter++;
   }
   printf("processed events = %d\n",counter);
}
//### END OF GENERATED CODE
