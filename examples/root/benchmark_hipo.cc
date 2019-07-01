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
#include <TVector3.h>

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

   hipo::bank  particles(factory.getSchema("REC::Particle"));

   hipo::event      event;

   int counter = 0;
   hipo::benchmark  readerBenchmark;
   hipo::benchmark  operationBenchmark;
   readerBenchmark.resume();
   TVector3 vec;

   int count_positive = 0;
   int count_negative = 0;

   while(reader.next()==true){
      reader.read(event);
      event.getStructure(particles);
      operationBenchmark.resume();
      int nrows = particles.getRows();
      //printf("---------- PARTICLE BANK CONTENT -------\n");
      for(int row = 0; row < nrows; row++){
         vec.SetXYZ(particles.getFloat(1,row),particles.getFloat(2,row),particles.getFloat(3,row));
        int charge = particles.getByte(7,row);
        if(vec.Mag()>5.0){
          if(charge>0){
            count_positive++;
          } else {
            count_negative++;
          }
        }
      }
      operationBenchmark.pause();
      counter++;
   }
   readerBenchmark.pause();
   /*while(reader.next()==true){

      counter++;
   }*/
   printf("processed events = %d (%d, %d) , benchmark : time = %ld , count = %d , operation : time = %ld , count = %d\n",
      counter,count_positive,count_negative,
      (long) (readerBenchmark.getTime()/1e+9),readerBenchmark.getCounter(),
    (long) (operationBenchmark.getTime()/1e+9),operationBenchmark.getCounter());
}
//### END OF GENERATED CODE
