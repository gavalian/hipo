//******************************************************************
//*  ██╗  ██╗██╗██████╗  ██████╗     ██╗  ██╗    ██████╗
//*  ██║  ██║██║██╔══██╗██╔═══██╗    ██║  ██║   ██╔═████╗
//*  ███████║██║██████╔╝██║   ██║    ███████║   ██║██╔██║
//*  ██╔══██║██║██╔═══╝ ██║   ██║    ╚════██║   ████╔╝██║
//*  ██║  ██║██║██║     ╚██████╔╝         ██║██╗╚██████╔╝
//*  ╚═╝  ╚═╝╚═╝╚═╝      ╚═════╝          ╚═╝╚═╝ ╚═════╝
//************************ Jefferson National Lab (2017) ***********
//******************************************************************
//* Example program for reading histogram from HIPO-4 Files..
//* Reads the file crated by clas12mon and prints on the screen
//* histograms.
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
   hipo::event   event;
   
   reader.open(inputFile);   

   int counter = 0;
   
   hipo::structure name;
   hipo::structure bins;
   hipo::structure lims;
   hipo::structure data;
   
   while(reader.next()==true){
      reader.read(event);
      event.getStructure(name,100,3);
      event.getStructure(bins,100,5);
      event.getStructure(lims,100,6);
      event.getStructure(data,100,8);
      printf("name: %s\n", name.getStringAt(0).c_str());
      printf("bins: %d\n", bins.getIntAt(0));
      printf("min/max: %f/%f\n", lims.getDoubleAt(0),lims.getDoubleAt(8));
      int nBins = bins.getIntAt(0);
      for(int k = 0; k < nBins; k++){
	printf("bin %4d : content = %12.6f\n",k,data.getDoubleAt(k*8));
      }
      counter++;
   }
   printf("processed events = %d\n",counter);
}
//### END OF GENERATED CODE
