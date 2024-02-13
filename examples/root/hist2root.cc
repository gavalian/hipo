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
#include <TH2D.h>
#include "TCanvas.h"
#include "TFile.h"
#include "TTreeReader.h"
#include "Compression.h"


void convert(TFile *file, hipo::event &event);
void convert2d(TFile *file, hipo::event &event);

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
     exit(1);
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
   hipo::structure type;

   while(reader.next()==true){
     reader.read(event);
     std::pair<int,int> pos100 = event.getStructurePosition4(100,1);
     std::pair<int,int> pos200 = event.getStructurePosition4(200,1);
    printf("%d , %d, %d %d\n",pos100.first , pos100.second, pos200.first, pos200.second);
     if(pos100.first>8) convert(hf,event);
     if(pos200.first>8) convert2d(hf,event);
   }
   hf->Write();
}


void convert2d(TFile *file, hipo::event &event){
  
  hipo::structure name;
  hipo::structure bins;
  hipo::structure lims;
  hipo::structure data;
  
  event.getStructure4(name,200,3);
  event.getStructure4(bins,200,5);
  event.getStructure4(lims,200,6);
  event.getStructure4(data,200,8);
  
  std::pair<int,int> pos200 = event.getStructurePosition4(200,8);

  std::string path = name.getStringAt(0).c_str();
  int last_slash = path.find_last_of("/");
  if(last_slash>1){
    std::string dir = path.substr(1,last_slash-1);
    std::string hname = path.substr(last_slash+1,path.size()-last_slash-1);
    printf("path [%s] ->  dir [%s], name [%s], 2d SIZE = %d, bins = (%d) %d %d\n",
         path.c_str(),dir.c_str(),hname.c_str(),data.getSize(), 
         bins.getSize(), bins.getIntAt(0),bins.getIntAt(4));
    for(int kk = 0; kk < 4; kk++) printf(" limits = %f \n",lims.getDoubleAt(kk*8));
    file->mkdir(dir.c_str());
    file->cd(dir.c_str());
    TH2D *h = new TH2D(hname.c_str(),"",bins.getIntAt(0),lims.getDoubleAt(0),lims.getDoubleAt(8),
      bins.getIntAt(4),lims.getDoubleAt(16),lims.getDoubleAt(24));
    int nBinsX = bins.getIntAt(0);
    int nBinsY = bins.getIntAt(4);
    int counter = 0;
    for(int x = 0; x < nBinsX; x++){
      for(int y = 0; y < nBinsY; y++){
         h->SetBinContent(x+1,y+1,data.getDoubleAt(counter*8));
         counter++;
      }
      //printf(" bin content %d = %f\n",i+1,data.getDoubleAt(i*8));
    }
    h->Write();
  }
  //printf("length = %d, index = %d\n",dir.size(),last_slash);
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
