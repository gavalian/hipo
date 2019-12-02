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

   hipo::reader  reader;
   reader.open(inputFile);
   hipo::dictionary  factory;
   reader.readDictionary(factory);

   factory.show();

   hipo::bank  particles(factory.getSchema("REC::Particle"));

   hipo::event      event;

   int counter = 0;
   hipo::benchmark  readerBenchmark;
   hipo::benchmark  copyBenchmark;
   hipo::benchmark  operationBenchmark;
   //readerBenchmark.resume();
   TVector3 vec,vert;

   int count_positive = 0;
   int count_negative = 0;
   int ev = 0;

   TH1F *h1mag = new TH1F("H1MAG","",120,0.0,10.0);

   //while(reader.next()==true){
     for(int ev = 0 ; ev < 6492832; ev++){
       //bool flag = reader.hasNext();
    //while(1){
    //  if (reader.next()==false) break;
      readerBenchmark.resume();
      reader.next();
      readerBenchmark.pause();
      copyBenchmark.resume();
      reader.read(event);
      event.getStructure(particles);
      copyBenchmark.pause();

      operationBenchmark.resume();
      int nrows = particles.getRows();
      float px,py,pz,vx,vy,vz,vt,beta,chi2pid;
      int pid;
      int16_t status;
      int8_t  charge;
      int pid_index = 0;
      //printf("---------- PARTICLE BANK CONTENT -------\n");
      for(int row = 0; row < nrows; row++){

	pid = particles.getInt(pid_index,row);
  //	int a = particles.getInt((int) 0,row);

  px = particles.getFloat(1,row);
	py = particles.getFloat(2,row);
	pz = particles.getFloat(3,row);
	vx = particles.getFloat(4,row);
	vy = particles.getFloat(5,row);
	vz = particles.getFloat(6,row);
  vt = particles.getFloat(7,row);
  charge = particles.getByte(8,row);

  beta = particles.getFloat(9,row);
  chi2pid = particles.getFloat(10,row);
  status  = particles.getShort(11,row);
  //status  = particles.getInt(11,row);


  double value = sqrt(px*px+py*py+pz*pz) * sqrt(vx*vx+vy*vy+vz*vz)
            + vt + pid*beta*chi2pid+status - charge;

	h1mag->Fill(value);
  //sqrt(px*px+py*py+pz*pz) * sqrt(vx*vx+vy*vy+vz*vz)
 //+ vt + pid*beta*chi2pid+status - charge);
	/*int charge = particles.getByte(8,row);
	  if(vec.Mag()>5.0){
          if(charge>0){
	  count_positive++;
          } else {
	  count_negative++;
          }
	  }*/
      }
      operationBenchmark.pause();
      counter++;
     }
     //readerBenchmark.pause();
     /*while(reader.next()==true){

       counter++;
       }*/
   printf("processed events = %d (%d, %d) , \n\n\n\t benchmark : time = %10.2f sec, count = %d , \n\t operation : time = %10.2f sec, count = %d\n",
      counter,count_positive,count_negative,
       (readerBenchmark.getTimeSec()),readerBenchmark.getCounter(),
     (operationBenchmark.getTimeSec()),operationBenchmark.getCounter());
     printf("   copy structures : time = %10.2f sec, count = %d\n\n\n", copyBenchmark.getTimeSec(), copyBenchmark.getCounter());
     printf("total time = %10.2f sec\n\n",
	    readerBenchmark.getTimeSec()+operationBenchmark.getTimeSec()+copyBenchmark.getTimeSec());
}
//### END OF GENERATED CODE
