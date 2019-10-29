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
#include "TFile.h"
#include "TTree.h"
#include "TVector3.h"

int main(int argc, char** argv) {

   std::cout << " reading file example program (HIPO) "  << __cplusplus << std::endl;

   char inputFile[512];
   char outputFile[512];

   if(argc>1) {
      sprintf(inputFile,"%s",argv[1]);
      //sprintf(outputFile,"%s",argv[2]);
   } else {
      std::cout << " *** please provide a file name..." << std::endl;
     exit(0);
   }

   TFile *f = TFile::Open(inputFile);

   TTree *tree;
   f->GetObject("clas12",tree);

   std::vector<Int_t>    *vec_pid;
   std::vector<Float_t>  *vec_px;
   std::vector<Float_t>  *vec_py;
   std::vector<Float_t>  *vec_pz;
   std::vector<Float_t>  *vec_vx;
   std::vector<Float_t>  *vec_vy;
   std::vector<Float_t>  *vec_vz;
   std::vector<Char_t>   *vec_charge;
   std::vector<Float_t>  *vec_beta;
   std::vector<Float_t>  *vec_chi2pid;
   std::vector<Short_t>  *vec_status;

   TBranch        *b_pid;   //!
     TBranch        *b_px;   //!
     TBranch        *b_py;   //!
     TBranch        *b_pz;   //!
     TBranch        *b_vx;   //!
     TBranch        *b_vy;   //!
     TBranch        *b_vz;   //!
     TBranch        *b_beta;   //!
     TBranch        *b_chi2pid;   //!
     TBranch        *b_charge;   //!
     TBranch        *b_status;   //!

   tree->SetBranchAddress("pid",&vec_pid,&b_pid);
   tree->SetBranchAddress("px",&vec_px,&b_px);
   tree->SetBranchAddress("py",&vec_py,&b_py);
   tree->SetBranchAddress("pz",&vec_pz,&b_pz);
   tree->SetBranchAddress("vx",&vec_vx,&b_vx);
   tree->SetBranchAddress("vy",&vec_vy,&b_vy);
   tree->SetBranchAddress("vz",&vec_vz,&b_vz);
   tree->SetBranchAddress("beta",&vec_beta,&b_beta);
   tree->SetBranchAddress("chi2pid",&vec_chi2pid,&b_chi2pid);
   tree->SetBranchAddress("charge",&vec_charge,&b_charge);
   tree->SetBranchAddress("status",&vec_status,&b_status);

   Long64_t nentries = tree->GetEntriesFast();
   printf("---> tree entries = %lld\n",nentries);
   int counter = 0;
   hipo::benchmark  readerBenchmark;
   hipo::benchmark  operBenchmark;

   TVector3 vec;

   int count_positive = 0;
   int count_negative = 0;

   for(Long64_t jentry = 0; jentry<nentries;jentry++){
       readerBenchmark.resume();
       //Long64_t ientry = tree->LoadTree(jentry);
       int nb = tree->GetEntry(jentry);
       readerBenchmark.pause();
       operBenchmark.resume();
       int size = vec_pid->size();
       for(int i = 0; i < size ; i++){
         vec.SetXYZ((*vec_px)[i],(*vec_py)[i],(*vec_pz)[i]);
         int charge = (*vec_charge)[i];
         if(vec.Mag()>5.0){
           if(charge>0){
             count_positive++;
           } else {
             count_negative++;
           }
         }
       }
       counter++;
       operBenchmark.pause();
   }
   /*while(reader.next()==true){

      counter++;
   }*/
   f->Close();
   printf("processed events = %d (%d, %d) , benchmark : red time = %10.2f sec , oper time = %10.2f sec , count = %d\n",
      counter,count_positive,count_negative,
      readerBenchmark.getTimeSec(), operBenchmark.getTimeSec(),
      readerBenchmark.getCounter());

}
//### END OF GENERATED CODE
