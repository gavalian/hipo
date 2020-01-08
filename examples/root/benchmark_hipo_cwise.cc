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
#include <TH1F.h>

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

   std::vector<hipo::reader>  readers;
   readers.resize(3);

   for(int i = 0; i < 3; i++) readers[i].setTags(i+2);
   for(int i = 0; i < 3; i++) readers[i].open(inputFile);

   hipo::benchmark  readerBenchmark("reader");
   hipo::benchmark  histoBenchmark("histogram");
   hipo::benchmark  operationBenchmark;

   readerBenchmark.resume();
   hipo::structure  pxNode;
   hipo::structure  pyNode;
   hipo::structure  pzNode;

   TH1F *h1mag = new TH1F("H1MAG","",120,0.0,10.0);

   for(int ev = 0 ; ev < 19478498; ev++){
     readers[0].next();
     readers[1].next();
     readers[2].next();

     readers[0].getStructureNoCopy(pxNode,2,1);
     readers[1].getStructureNoCopy(pyNode,2,1);
     readers[2].getStructureNoCopy(pzNode,2,1);

     int size = pxNode.getSize()/4;
     //pxNode.show();
     //printf(" size = %d\n",size);
     for(int i = 0; i < size; i++){
       float px = pxNode.getFloatAt(i*4);
       float py = pyNode.getFloatAt(i*4);
       float pz = pzNode.getFloatAt(i*4);
       double p = sqrt(px*px+py*py+pz+pz);
       //histoBenchmark.resume();
       //h1mag->Fill(p);
       //histoBenchmark.pause();
      //printf(" %8.4f ",px);
    }
     //printf("\n");
    }
    readerBenchmark.pause();
    readerBenchmark.show();
    histoBenchmark.show();
/*
   TVector3 vec,vert;
   int counter = 0;
   int count_positive = 0;
   int count_negative = 0;
   int ev = 0;

   TH1F *h1mag = new TH1F("H1MAG","",120,0.0,10.0);

   //while(reader.next()==true){
   readerBenchmark.resume();
     for(int ev = 0 ; ev < 6492832; ev++){
       //bool flag = reader.hasNext();
    //while(1){
    //  if (reader.next()==false) break;
      //readerBenchmark.resume();
      readers[0].next();



      }
      //operationBenchmark.pause();
      counter++;
     }

     */
    /* readerBenchmark.pause();

   printf("processed events = %d (%d, %d) , \n\n\n\t benchmark : time = %10.2f sec, count = %d , \n\t operation : time = %10.2f sec, count = %d\n",
      counter,count_positive,count_negative,
       (readerBenchmark.getTimeSec()),readerBenchmark.getCounter(),
     (operationBenchmark.getTimeSec()),operationBenchmark.getCounter());
     printf("   copy structures : time = %10.2f sec, count = %d\n\n\n", copyBenchmark.getTimeSec(), copyBenchmark.getCounter());
     printf("total time = %10.2f sec\n\n",
	    readerBenchmark.getTimeSec()+operationBenchmark.getTimeSec()+copyBenchmark.getTimeSec());*/
}
//### END OF GENERATED CODE
