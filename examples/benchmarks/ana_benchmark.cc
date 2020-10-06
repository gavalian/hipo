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
#include <map>
#include <vector>
#include "utils.h"
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

   hipo::bank  particles(factory.getSchema("REC::Particle"));
   hipo::bank  detectors(factory.getSchema("REC::Calorimeter"));

   hipo::event      event;

   int counter = 0;
   hipo::benchmark    hashBench("hash code");
   //reader.gotoEvent(561);
   //std::map<int,std::vector<int> > responses;
   std::map<int,std::vector<int> * > responses;
   int counterSf_gt_One = 0;
   int counterSf_lt_One = 0;
   int  c1 = 0;
   int  c2 = 0;
   int  c3 = 0;
   while(reader.next()==true){
      reader.read(event);

      event.getStructure(particles);
      event.getStructure(detectors);

      responses.clear();
      hashBench.resume();
      int nrows_d = detectors.getRows();
      for(int d = 0; d < nrows_d; d++){
        int pindex = detectors.getInt(1,d);
        if(responses.count(pindex)==0){
          responses[pindex] = new std::vector<int>();
        }
        responses[pindex]->push_back(d);
      }

      int nrows = particles.getRows();
      //printf("---------- PARTICLE BANK CONTENT -------\n");
      for(int row = 0; row < nrows; row++){
        c1++;
         int    charge = particles.getInt(8,row);
         if(charge==0){
           c2++;
           float  px = particles.getFloat(1,row);
           float  py = particles.getFloat(2,row);
           float  pz = particles.getFloat(3,row);
           double p = sqrt(px*px+py*py+pz*pz);
           double energy = 0.0;
           if(responses.count(row)>0){
             c3++;
              std::vector<int> &ref = *responses[row];
               for(int r = 0 ; r < ref.size(); r++){
                   energy += detectors.getFloat(5,ref[r]);
               }
               if(p/energy>1.0){
                  counterSf_gt_One++;
               } else {
                  counterSf_lt_One++;
               }
           }
         }

         //printf("%6d : %6d %8.4f %8.4f %8.4f\n",row,pid,px,py,pz);
      }
      hashBench.pause();
      //printf("---------- END OF PARTICLE BANK -------\n");

      counter++;
   }
   printf("processed events = %d = %d / %d \n",counter,counterSf_gt_One,counterSf_lt_One);
   printf("c1 / c2 / c3 =  %d / %d / %d\n",c1,c2,c3);
   hashBench.show();
}
//### END OF GENERATED CODE
