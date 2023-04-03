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
#include <TH1D.h>
#include "TCanvas.h"
#include "TFile.h"
#include "TTreeReader.h"
#include "Compression.h"


void convert(TFile *file, hipo::event &event);


int main(int argc, char** argv) {

   std::cout << " reading file example program (HIPO) "
   << __cplusplus << std::endl;
   
   char inputFile[512];
   char outputFile[512];
   char outputFileHipo[512];

   if(argc>1) {
      sprintf(inputFile,"%s",argv[1]);
      sprintf(outputFile,"%s.root",argv[1]);
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

   TFile *hf = new TFile(outputFile,"RECREATE");
   
   while(reader.next()==true){
     reader.read(event);
     convert(hf,event);
   }
   hf->Write();
}

void convert(TFile *file, hipo::event &event){
  
  hipo::structure name;
  hipo::structure bins;
  hipo::structure lims;
  hipo::structure data;
  
  event.getStructure(name,100,3);
  event.getStructure(bins,100,5);
  event.getStructure(lims,100,6);
  event.getStructure(data,100,8);

  std::string path = name.getStringAt(0).c_str();
  int last_slash = path.find_last_of("/");
  if(last_slash>1){
    std::string dir = path.substr(1,last_slash-1);
    std::string hname = path.substr(last_slash+1,path.size()-last_slash-1);
    printf("path [%s] ->  dir [%s], name [%s]\n",path.c_str(),dir.c_str(),hname.c_str());
    file->mkdir(dir.c_str());
    file->cd(dir.c_str());
    TH1D *h = new TH1D(hname.c_str(),"",bins.getIntAt(0),lims.getDoubleAt(0),lims.getDoubleAt(8));
    int nBins = bins.getIntAt(0);
    for(int i = 0; i < nBins; i++){
      h->SetBinContent(i+1,data.getDoubleAt(i*8));
      //printf(" bin content %d = %f\n",i+1,data.getDoubleAt(i*8));
    }
    h->Write();
  }
  //printf("length = %d, index = %d\n",dir.size(),last_slash);
}
