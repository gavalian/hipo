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
//* Reads the file and converts the file to ROOT.
//*--
//* Author: G.Gavalian
//*

#include <cstdlib>
#include <iostream>
#include "reader.h"
#include "writer.h"
#include "utils.h"
#include "TFile.h"
#include "TTree.h"
#include "Compression.h"


int main(int argc, char** argv) {

   std::cout << " reading file example program (HIPO) "
   << __cplusplus << std::endl;

   char inputFile[512];
   char outputFile[512];
   char outputFileHipo[512];

   if(argc>1) {
      sprintf(inputFile,"%s",argv[1]);
      sprintf(outputFile,"%s.root",argv[1]);
      sprintf(outputFileHipo,"%s_writer.hipo",argv[1]);
      //sprintf(outputFile,"%s",argv[2]);
   } else {
      std::cout << " *** please provide a file name..." << std::endl;
     exit(0);
   }

   TFile *f = TFile::Open(outputFile,"CREATE");
   //f->SetCompressionAlgorithm(ROOT::kZLIB);
   //f->SetCompressionAlgorithm(ROOT::kLZMA);
   f->SetCompressionAlgorithm(ROOT::kLZ4);
   //f->SetCompressionAlgorithm(ROOT::kOldCompressionAlgo);
   TTree *tree = new TTree("clas12","CLAS12 ROOT Tree");

   std::vector<Int_t>    vec_pid;
   std::vector<Float_t>  vec_px;
   std::vector<Float_t>  vec_py;
   std::vector<Float_t>  vec_pz;
   std::vector<Float_t>  vec_vx;
   std::vector<Float_t>  vec_vy;
   std::vector<Float_t>  vec_vz;
   std::vector<Float_t>  vec_vt;
   std::vector<Char_t>   vec_charge;
   std::vector<Float_t>  vec_beta;
   std::vector<Float_t>  vec_chi2pid;
   std::vector<Short_t>  vec_status;

   tree->Branch("pid",&vec_pid);
   tree->Branch("px",&vec_px);
   tree->Branch("py",&vec_py);
   tree->Branch("pz",&vec_pz);
   tree->Branch("vx",&vec_vx);
   tree->Branch("vy",&vec_vy);
   tree->Branch("vz",&vec_vz);
   tree->Branch("vt",&vec_vt);
   tree->Branch("beta",&vec_beta);
   tree->Branch("chi2pid",&vec_chi2pid);
   tree->Branch("charge",&vec_charge);
   tree->Branch("status",&vec_status);

   hipo::reader  reader;
   reader.open(inputFile);
   hipo::dictionary  factory;
   reader.readDictionary(factory);

   factory.show();

   hipo::writer writer ;
   writer.getDictionary().addSchema(factory.getSchema("REC::Particle"));
   writer.open(outputFileHipo);

   hipo::bank  particles(factory.getSchema("REC::Particle"));

   hipo::event      event;

   int counter = 0;

   hipo::benchmark     writerBenchmark;
   hipo::benchmark     readerBenchmark;
   hipo::benchmark   transferBenchmark;
   hipo::benchmark       restBenchmark;
   hipo::benchmark       writerHipoBenchmark;

   while(reader.next()==true){

      readerBenchmark.resume();
      reader.read(event);
      event.getStructure(particles);
      readerBenchmark.pause();


      restBenchmark.resume();
      //particles.show();
      int nrows = particles.getRows();
      vec_pid.resize(nrows);
      vec_px.resize(nrows);
      vec_py.resize(nrows);
      vec_pz.resize(nrows);
      vec_vx.resize(nrows);
      vec_vy.resize(nrows);
      vec_vz.resize(nrows);
      vec_vt.resize(nrows);
      vec_beta.resize(nrows);
      vec_chi2pid.resize(nrows);
      vec_charge.resize(nrows);
      vec_status.resize(nrows);
      restBenchmark.pause();
      //printf("---------- PARTICLE BANK CONTENT -------\n");
      transferBenchmark.resume();
      for(int row = 0; row < nrows; row++){
         vec_pid[row] = particles.getInt("pid",row);
         vec_px[row]  = particles.getFloat("px",row);
         vec_py[row]  = particles.getFloat("py",row);
         vec_pz[row]  = particles.getFloat("pz",row);
         vec_vx[row]  = particles.getFloat("vx",row);
         vec_vy[row]  = particles.getFloat("vy",row);
         vec_vz[row]  = particles.getFloat("vz",row);
         vec_vt[row]  = particles.getFloat("vt",row);
         vec_beta[row]  = particles.getFloat("beta",row);
         vec_chi2pid[row]  = particles.getFloat("chi2pid",row);
         vec_charge[row] = (int8_t) particles.getByte("charge",row);
         vec_status[row] = (int16_t) particles.getShort("status",row);
         //printf("%6d : %6d %8.4f %8.4f %8.4f\n",row,pid,px,py,pz);
      }
      transferBenchmark.pause();

      writerBenchmark.resume();
      if(vec_pid.size()>0) tree->Fill();
      writerBenchmark.pause();

      writerHipoBenchmark.resume();
      writer.addEvent(event);
      writerHipoBenchmark.pause();
      //printf("---------- END OF PARTICLE BANK -------\n");
      counter++;
   }
   f->Close();
   writer.close();
   printf("processed events = %d, benchmark (WRITE) : time = %10.2f sec , count = %d\n",
      counter,writerBenchmark.getTimeSec(),writerBenchmark.getCounter());
  printf("processed events = %d, hipo      (WRITE) : time = %10.2f sec , count = %d\n",
         counter,writerHipoBenchmark.getTimeSec(),writerHipoBenchmark.getCounter());
   printf("processed events = %d, benchmark (READ)  : time = %10.2f sec , count = %d\n",
      counter,readerBenchmark.getTimeSec(),readerBenchmark.getCounter());
   printf("processed events = %d, benchmark (COPY)  : time = %10.2f sec , count = %d\n",
      counter,transferBenchmark.getTimeSec(),transferBenchmark.getCounter());
      printf("processed events = %d, benchmark (REST)  : time = %10.2f sec , count = %d\n",
         counter,restBenchmark.getTimeSec(),restBenchmark.getCounter());

   double total_time = writerBenchmark.getTimeSec() + readerBenchmark.getTimeSec()
          + transferBenchmark.getTimeSec() + restBenchmark.getTimeSec();
    printf("\n total time = %10.2f\n",total_time);
}
//### END OF GENERATED CODE
