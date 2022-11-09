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
#include "jsonutil.h"



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

   //factory.show();

   hipo::bank  epics(factory.getSchema("RAW::epics"));

   hipo::event      event;

   int counter = 0;
   hipo::jsonutil  util;

   std::string var("hallb_IPM2C21A_YPOS");
   
   while(reader.next()==true){
     reader.read(event);
     event.read(epics);
     if(epics.getRows()>0){
       std::string json = epics.getStringAt(0);
       util.set(json.c_str());
       if(util.hasObject(var.c_str())==true){
	 float ypos_C21A = util.getFloat(var.c_str());
	 printf("event # %d : %s = %f\n",counter,var.c_str(),ypos_C21A);
       }
       //---------------------------------------------
       // uncomment the following line to prinout on
       // the screen the entire json epics string
       //printf("%d : %s\n",counter,json.c_str());
     }
     counter++;
     
   }
   printf("processed events = %d\n",counter);
}
//### END OF GENERATED CODE
