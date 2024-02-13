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
#include "TCanvas.h"
#include "TFile.h"
#include "TH1D.h"
#include "TSystem.h"
#include "TTree.h"

double benchmark_hipo(const char *file);
double benchmark_root(const char *file);
double benchmark_root_partial(const char *file);

double average(std::vector<double> data);

int main(int argc, char** argv) {

   std::cout << " reading file example program (HIPO) "  << __cplusplus << std::endl;

   char inputFile[256];

   if(argc>1) {
      sprintf(inputFile,"%s",argv[1]);
      //sprintf(outputFile,"%s",argv[2]);
   } else {
      std::cout << " *** please provide a file name..." << std::endl;
     exit(1);
   }


   std::vector<double> times;
   benchmark_hipo(inputFile);
   benchmark_hipo(inputFile);
   for(int i = 0; i < 4; i++)
   {
     double time = benchmark_hipo(inputFile);
     times.push_back(time);
   }

  double timeHipo = average(times);
  double timeRoot = 0.0;

  if(argc>2){
    char inputFileRoot[256];
    sprintf(inputFileRoot,"%s",argv[2]);
    benchmark_root_partial(inputFileRoot);
    benchmark_root_partial(inputFileRoot);
    std::vector<double> timesroot;
    for(int i = 0; i < 4; i++)
    {
     double time = benchmark_root_partial(inputFileRoot);
     timesroot.push_back(time);     
    }
    timeRoot = average(timesroot);
  }

  printf("\n\n************\n");
  printf("[FIN] (C++ ) hipo read  : %10.4f sec\n",timeHipo);
  printf("[FIN] (C++ ) root read  : %10.4f sec\n",timeRoot);
}

double benchmark_root(const char *pathInput){

  auto file = TFile::Open(pathInput);
   auto tree = file->Get<TTree>("clas12");

  int count;
   int   pid[1000];
   float px[1000];
   float py[1000];
   float pz[1000];
   float vx[1000];
   float vy[1000];
   float vz[1000];
   float vt[1000];
   float beta[1000];
   float chi2pid[1000];
   int16_t status[1000];
   Char_t  charge[1000];

   TBranch *br_count = nullptr;
   TBranch *br_px = nullptr;
   TBranch *br_py = nullptr;
   TBranch *br_pz = nullptr;

   TBranch *br_vx = nullptr;
   TBranch *br_vy = nullptr;
   TBranch *br_vz = nullptr;
   TBranch *br_vt = nullptr;

   TBranch *br_pid = nullptr;
   TBranch *br_beta = nullptr;
   TBranch *br_chi2pid = nullptr;
   TBranch *br_status = nullptr;
   TBranch *br_charge = nullptr;

   tree->SetBranchAddress("count", &count, &br_count);
   tree->SetBranchAddress("px", px, &br_px);
   tree->SetBranchAddress("py", py, &br_py);
   tree->SetBranchAddress("pz", pz, &br_pz);
   tree->SetBranchAddress("vx", vx, &br_vx);
   tree->SetBranchAddress("vy", vy, &br_vy);
   tree->SetBranchAddress("vz", vz, &br_vz);
   tree->SetBranchAddress("vt", vt, &br_vt);

   tree->SetBranchAddress("pid", pid, &br_pid);
   tree->SetBranchAddress("beta", beta, &br_beta);
   tree->SetBranchAddress("charge", charge, &br_charge);

   tree->SetBranchAddress("chi2pid", chi2pid, &br_chi2pid);
   tree->SetBranchAddress("status", status, &br_status);

   auto h = new TH1D("", "", 120, 0, 10);
   long runtime = 0;
   auto nEntries = tree->GetEntries();
   //printf("drawing optimized, entries = %ld\n",nEntries);
   hipo::benchmark  readerBenchmark;
   hipo::benchmark  copyBenchmark;
   hipo::benchmark  operationBenchmark;
  int counter = 0;
   readerBenchmark.resume();
   for (decltype(nEntries) entryId = 0; entryId < nEntries; ++entryId) {
      tree->LoadTree(entryId);
      auto tsStart = std::chrono::steady_clock::now();
      br_count->GetEntry(entryId);
      br_px->GetEntry(entryId);
      br_py->GetEntry(entryId);
      br_pz->GetEntry(entryId);
      br_vx->GetEntry(entryId);
      br_vy->GetEntry(entryId);
      br_vz->GetEntry(entryId);
      br_vt->GetEntry(entryId);
      br_pid->GetEntry(entryId);
      br_beta->GetEntry(entryId);
      br_chi2pid->GetEntry(entryId);
      br_status->GetEntry(entryId);

      counter++;
      for (int i = 0; i < count; ++i) {	
	      h->Fill(
		      sqrt(px[i]*px[i] + py[i]*py[i] + pz[i]*pz[i])*sqrt(vx[i]*vx[i]+vy[i]*vy[i]+vz[i]*vz[i])
		      + vt[i] + pid[i]*beta[i]*chi2pid[i]+status[i] - charge[i]);
      }
      //auto tsStop = std::chrono::steady_clock::now();
      //runtime += std::chrono::duration_cast<std::chrono::microseconds>(tsStop - tsStart).count();
   }
   readerBenchmark.pause();
   printf("processed events %d, \n\t benchmark : time = %10.5f sec, \n",
       counter,
       (readerBenchmark.getTimeSec()));
   //std::cout << "Runtime algo: " << runtime << " us" << std::endl;
  return readerBenchmark.getTimeSec();
}

double benchmark_root_partial(const char *pathInput){

  auto file = TFile::Open(pathInput);
   auto tree = file->Get<TTree>("clas12");

  int count;
   int   pid[1000];
   float px[1000];
   float py[1000];
   float pz[1000];
   float vx[1000];
   float vy[1000];
   float vz[1000];
   float vt[1000];
   float beta[1000];
   float chi2pid[1000];
   int16_t status[1000];
   Char_t  charge[1000];

   TBranch *br_count = nullptr;
   TBranch *br_px = nullptr;
   TBranch *br_py = nullptr;
   TBranch *br_pz = nullptr;

   TBranch *br_vx = nullptr;
   TBranch *br_vy = nullptr;
   TBranch *br_vz = nullptr;
   TBranch *br_vt = nullptr;

   TBranch *br_pid = nullptr;
   TBranch *br_beta = nullptr;
   TBranch *br_chi2pid = nullptr;
   TBranch *br_status = nullptr;
   TBranch *br_charge = nullptr;

   tree->SetBranchAddress("count", &count, &br_count);
   tree->SetBranchAddress("px", px, &br_px);
   tree->SetBranchAddress("py", py, &br_py);
   tree->SetBranchAddress("pz", pz, &br_pz);
/*   tree->SetBranchAddress("vx", vx, &br_vx);
   tree->SetBranchAddress("vy", vy, &br_vy);
   tree->SetBranchAddress("vz", vz, &br_vz);
   tree->SetBranchAddress("vt", vt, &br_vt);

   tree->SetBranchAddress("pid", pid, &br_pid);
   tree->SetBranchAddress("beta", beta, &br_beta);
   tree->SetBranchAddress("charge", charge, &br_charge);

   tree->SetBranchAddress("chi2pid", chi2pid, &br_chi2pid);
   tree->SetBranchAddress("status", status, &br_status);
*/
   auto h = new TH1D("", "", 120, 0, 10);
   long runtime = 0;
   auto nEntries = tree->GetEntries();
   //printf("drawing optimized, entries = %ld\n",nEntries);
   hipo::benchmark  readerBenchmark;
   hipo::benchmark  copyBenchmark;
   hipo::benchmark  operationBenchmark;
  int counter = 0;
   readerBenchmark.resume();
   for (decltype(nEntries) entryId = 0; entryId < nEntries; ++entryId) {
      tree->LoadTree(entryId);
      auto tsStart = std::chrono::steady_clock::now();
      br_count->GetEntry(entryId);
      br_px->GetEntry(entryId);
      br_py->GetEntry(entryId);
      br_pz->GetEntry(entryId);
  /*    br_vx->GetEntry(entryId);
      br_vy->GetEntry(entryId);
      br_vz->GetEntry(entryId);
      br_vt->GetEntry(entryId);
      br_pid->GetEntry(entryId);
      br_beta->GetEntry(entryId);
      br_chi2pid->GetEntry(entryId);
      br_status->GetEntry(entryId);
*/
      counter++;
      for (int i = 0; i < count; ++i) {	
	      h->Fill(
		      sqrt(px[i]*px[i] + py[i]*py[i] + pz[i]*pz[i]));
      }
      //auto tsStop = std::chrono::steady_clock::now();
      //runtime += std::chrono::duration_cast<std::chrono::microseconds>(tsStop - tsStart).count();
   }
   readerBenchmark.pause();
   printf("processed events %d, \n\t benchmark : time = %10.5f sec, \n",
       counter,
       (readerBenchmark.getTimeSec()));
   //std::cout << "Runtime algo: " << runtime << " us" << std::endl;
  return readerBenchmark.getTimeSec();
}
double benchmark_hipo(const char *file){
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
      //copyBenchmark.resume();
      // the particle column 0 - is PID
      // the call will return the address and the length
      // of the 0-th column from particle bank in the prt data class.
     record.read(particles,r);

     //copyBenchmark.pause();

      //int col_size = pid.getDataSize();
      //printf("-> rows %8d \n",col_size);
      //operationBenchmark.resume();
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
	                     + vt + particles.getInt(0,row)*particles.getFloat(9,row)
                       *particles.getFloat(10,row)+particles.getInt(11,row) 
                       - particles.getInt(8,row);
       hbench->Fill(value);
     }
     //operationBenchmark.pause();
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
    return readerBenchmark.getTimeSec();
}

double average(std::vector<double> data){
  double summ = 0.0;
  for(int i =0; i < data.size(); i++)
     summ += data[i];
  return summ/data.size();
}


//### END OF GENERATED CODE
