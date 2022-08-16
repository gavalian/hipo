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
#include "TLorentzVector.h"
#include <TH1F.h>


void benchmark(const char *file){

  hipo::reader  reader;
   reader.open(file);
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
   int counterEPI = 0;
   int eventcount = 0;
   TLorentzVector vece,vecpi;

   float px,py,pz,vx,vy,vz,vt,beta,chi2pid;
   int pid;
   int16_t status;
   int8_t  charge;
   int pid_index = 0;
      
   readerBenchmark.resume();
   while(reader.next()==true){
     copyBenchmark.resume();
     reader.read(event);
     event.getStructure(particles);
     copyBenchmark.pause();
     operationBenchmark.resume();
     int nrows = particles.getRows();
     for(int row = 0; row < nrows; row++){
       px = particles.getFloat(1,row);
       py = particles.getFloat(2,row);
       pz = particles.getFloat(3,row);
       vx = particles.getFloat(4,row);
       vy = particles.getFloat(5,row);
       vz = particles.getFloat(6,row);
       vt = particles.getFloat(7,row);
       
       double value =  sqrt(px*px + py*py + pz*pz)*sqrt(vx*vx+vy*vy+vz*vz)
	 + vt + particles.getInt(0,row)*particles.getFloat(9,row)*particles.getFloat(10,row)+particles.getInt(11,row) - particles.getInt(8,row);
       h1mag->Fill(value);
     }
   operationBenchmark.pause();
}
readerBenchmark.pause();
printf("processed events = %d (%d, %d) , \n\n\n\t benchmark : time = %10.2f sec, count = %d , \n\t operation : time = %10.2f sec, count = %d\n",
       counter,count_positive,count_negative,
       (readerBenchmark.getTimeSec()),readerBenchmark.getCounter(),
       (operationBenchmark.getTimeSec()),operationBenchmark.getCounter());
printf("   copy structures : time = %10.2f sec, count = %d\n\n\n", copyBenchmark.getTimeSec(), copyBenchmark.getCounter());
printf("total time = %10.2f sec\n\n",
       readerBenchmark.getTimeSec()+operationBenchmark.getTimeSec()+copyBenchmark.getTimeSec());
}
void benchmark_noeventcopy(const char *file){
  hipo::reader  reader;
   hipo::record  record;
  int count_positive = 0;
   int count_negative = 0;
   int ev = 0;
   int counter = 0;
   reader.open(file);
   hipo::dictionary  dict;
   reader.readDictionary(dict);
   hipo::bank  particles(dict.getSchema("REC::Particle"));
   int nrec = reader.getNRecords();
   TH1F *hbench = new TH1F("HBENCH2","",120,0.0,10.0);
  float lpx,lpy,lpz,lvx,lvy,lvz;
float px,py,pz,vx,vy,vz,vt,beta,chi2pid;
   int pid;
   int16_t status;
   int8_t  charge;
   int pid_index = 0;
   hipo::benchmark  readerBenchmark;
   hipo::benchmark  copyBenchmark;
   hipo::benchmark  operationBenchmark;


  readerBenchmark.resume();
  for(int i =0 ;i < nrec; i++){
    reader.loadRecord(record,i);
    int nevt = record.getEventCount();
    //printf("event size = %d\n", nevt);
    for(int r = 0; r < nevt; r++){
      copyBenchmark.resume();
      // the particle column 0 - is PID
      // the call will return the address and the length
      // of the 0-th column from particle bank in the prt data class.
     record.read(particles,r);

   copyBenchmark.pause();

      //int col_size = pid.getDataSize();
      //printf("-> rows %8d \n",col_size);
      operationBenchmark.resume();
 int nrows = particles.getRows();
     for(int row = 0; row < nrows; row++){
       px = particles.getFloat(1,row);
       py = particles.getFloat(2,row);
       pz = particles.getFloat(3,row);
       vx = particles.getFloat(4,row);
       vy = particles.getFloat(5,row);
       vz = particles.getFloat(6,row);
       vt = particles.getFloat(7,row);
       
       double value =  sqrt(px*px + py*py + pz*pz)*sqrt(vx*vx+vy*vy+vz*vz)
	 + vt + particles.getInt(0,row)*particles.getFloat(9,row)*particles.getFloat(10,row)+particles.getInt(11,row) - particles.getInt(8,row);
       hbench->Fill(value);
     }
      operationBenchmark.pause();
    }
  }
   readerBenchmark.pause();
   printf("processed events = %d (%d, %d) , \n\n\n\t benchmark : time = %10.5f sec, count = %d , \n\t operation : time = %10.5f sec, count = %d\n",
       counter,count_positive,count_negative,
       (readerBenchmark.getTimeSec()),readerBenchmark.getCounter(),
       (operationBenchmark.getTimeSec()),operationBenchmark.getCounter());
printf("   copy structures : time = %10.5f sec, count = %d\n\n\n", copyBenchmark.getTimeSec(), copyBenchmark.getCounter());
printf("total time = %10.2f sec\n\n",
       readerBenchmark.getTimeSec()+operationBenchmark.getTimeSec()+copyBenchmark.getTimeSec());
}

void benchmark_nocopy(const char *file){
   hipo::reader  reader;
   hipo::record  record;
  int count_positive = 0;
   int count_negative = 0;
   int ev = 0;
   int counter = 0;
   reader.open(file);
   hipo::dictionary  dict;
   reader.readDictionary(dict);
   hipo::bank  particle(dict.getSchema("REC::Particle"));
   int nrec = reader.getNRecords();
  std::vector<std::pair<int,int>> events;

  hipo::data pid;
  hipo::data px;
  hipo::data py;
  hipo::data pz;
  hipo::data vx;
  hipo::data vy;
  hipo::data vz; 
  hipo::data vt;  
  hipo::data charge;
  hipo::data beta;
  hipo::data chi2pid;
  hipo::data status;
  TH1F *hbench = new TH1F("HBENCH","",120,0.0,10.0);
  float lpx,lpy,lpz,lvx,lvy,lvz;

   hipo::benchmark  readerBenchmark;
   hipo::benchmark  copyBenchmark;
   hipo::benchmark  operationBenchmark;


  readerBenchmark.resume();
  for(int i =0 ;i < nrec; i++){
    reader.loadRecord(record,i);
    int nevt = record.getEventCount();
    //printf("event size = %d\n", nevt);
    for(int r = 0; r < nevt; r++){
      copyBenchmark.resume();
      // the particle column 0 - is PID
      // the call will return the address and the length
      // of the 0-th column from particle bank in the prt data class.
      record.getColumn(pid,    0, particle, r);
      record.getColumn(px,     1, particle, r);
      record.getColumn(py,     2, particle, r);
      record.getColumn(pz,     3, particle, r);
      record.getColumn(vx,     4, particle, r);
      record.getColumn(vy,     5, particle, r);
      record.getColumn(vz,     6, particle, r);
      record.getColumn(vt,     7, particle, r);
      record.getColumn(charge,  8, particle, r);
      record.getColumn(beta,     9, particle, r);
      record.getColumn(chi2pid,     10, particle, r);
      record.getColumn(status,     11, particle, r);

   copyBenchmark.pause();
      int col_size = pid.getDataSize();
      //printf("-> rows %8d \n",col_size);
      operationBenchmark.resume();
      for(int c = 0; c < col_size; c++){
        lpx = *reinterpret_cast<const float*>(&px.getDataPtr()[8+c*4]);
        lpy = *reinterpret_cast<const float*>(&py.getDataPtr()[8+c*4]);
        lpz = *reinterpret_cast<const float*>(&pz.getDataPtr()[8+c*4]);
        lvx = *reinterpret_cast<const float*>(&vx.getDataPtr()[8+c*4]);
        lvy = *reinterpret_cast<const float*>(&vy.getDataPtr()[8+c*4]);
        lvz = *reinterpret_cast<const float*>(&vz.getDataPtr()[8+c*4]);
      double value = sqrt(lpx*lpx+lpy*lpy+lpz*lpz)*sqrt(lpx*lpx+lpy*lpy+lpz*lpz)
         + (*reinterpret_cast<const float*>(&vt.getDataPtr()[8+c*4])) + 
           (*reinterpret_cast<const int*>(&px.getDataPtr()[8+c*4]))
           * (*reinterpret_cast<const float*>(&beta.getDataPtr()[8+c*4]))
           * (*reinterpret_cast<const float*>(&chi2pid.getDataPtr()[8+c*4]))
           + (*reinterpret_cast<const int16_t*>(&status.getDataPtr()[8+c*2]))
            - (*reinterpret_cast<const int8_t*>(&charge.getDataPtr()[8+c*4]));
          //printf("\t--->>> row %8d , v = %14.8f\n",c,value);
          hbench->Fill(value);
      }
      operationBenchmark.pause();
    }
  }
   readerBenchmark.pause();
   printf("processed events = %d (%d, %d) , \n\n\n\t benchmark : time = %10.5f sec, count = %d , \n\t operation : time = %10.5f sec, count = %d\n",
       counter,count_positive,count_negative,
       (readerBenchmark.getTimeSec()),readerBenchmark.getCounter(),
       (operationBenchmark.getTimeSec()),operationBenchmark.getCounter());
printf("   copy structures : time = %10.5f sec, count = %d\n\n\n", copyBenchmark.getTimeSec(), copyBenchmark.getCounter());
printf("total time = %10.2f sec\n\n",
       readerBenchmark.getTimeSec()+operationBenchmark.getTimeSec()+copyBenchmark.getTimeSec());
}

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


   benchmark(inputFile);
benchmark_nocopy(inputFile);

benchmark_noeventcopy(inputFile);

   /*
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
   int counterEPI = 0;
   int eventcount = 0;
   TLorentzVector vece,vecpi;

   readerBenchmark.resume();
   while(reader.next()==true){
  //   for(int ev = 0 ; ev < 6492832; ev++){
       //bool flag = reader.hasNext();
    //while(1){
    //  if (reader.next()==false) break;

      //reader.next();

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
      int index_e = -1, index_pi = -1;
      for(int row = 0; row < nrows; row++){
	//double value =  sqrt(px[i]*px[i] + py[i]*py[i] + pz[i]*pz[i])*sqrt(vx[i]*vx[i]+vy[i]*vy[i]+vz[i]*vz[i])
        //        + vt[i] + pid[i]*beta[i]*chi2pid[i]+status[i] - charge[i]);
      // pid = particles.getInt(pid_index,row);
      //  if(pid==11) index_e = row;
      //   if(pid==211) index_pi = row;
      //	int a = particles.getInt((int) 0,row);
      }
   
      eventcount++;
      if(index_e>=0&&index_pi>=0){
        vece.SetXYZM(
            particles.getFloat(1,index_e),
            particles.getFloat(2,index_e),
            particles.getFloat(3,index_e),
            0.005
          );
          vecpi.SetXYZM(
              particles.getFloat(1,index_pi),
              particles.getFloat(2,index_pi),
              particles.getFloat(3,index_pi),
              0.138
            );
            TLorentzVector invMass = vece+vecpi;
            h1mag->Fill(invMass.M());
            counterEPI++;
	    }
      
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
	int charge = particles.getByte(8,row);
	  if(vec.Mag()>5.0){
          if(charge>0){
	  count_positive++;
          } else {
	  count_negative++;
          }
	  }
      //}
      operationBenchmark.pause();
      counter++;
     }

    readerBenchmark.pause();
     //readerBenchmark.pause();
     while(reader.next()==true){

       counter++;
       }
   printf("processed events = %d (%d, %d) , \n\n\n\t benchmark : time = %10.2f sec, count = %d , \n\t operation : time = %10.2f sec, count = %d\n",
      counter,count_positive,count_negative,
       (readerBenchmark.getTimeSec()),readerBenchmark.getCounter(),
     (operationBenchmark.getTimeSec()),operationBenchmark.getCounter());
     printf("   copy structures : time = %10.2f sec, count = %d\n\n\n", copyBenchmark.getTimeSec(), copyBenchmark.getCounter());
     printf("total time = %10.2f sec\n\n",
	    readerBenchmark.getTimeSec()+operationBenchmark.getTimeSec()+copyBenchmark.getTimeSec());

	    printf("analyzed events = %8d / %8d\n",counterEPI,eventcount);*/
}
//### END OF GENERATED CODE
