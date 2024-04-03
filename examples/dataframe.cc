//******************************************************************
//*  ██╗  ██╗██╗██████╗  ██████╗     ██╗  ██╗    ██████╗
//*  ██║  ██║██║██╔══██╗██╔═══██╗    ██║  ██║   ██╔═████╗
//*  ███████║██║██████╔╝██║   ██║    ███████║   ██║██╔██║
//*  ██╔══██║██║██╔═══╝ ██║   ██║    ╚════██║   ████╔╝██║
//*  ██║  ██║██║██║     ╚██████╔╝         ██║██╗╚██████╔╝
//*  ╚═╝  ╚═╝╚═╝╚═╝      ╚═════╝          ╚═╝╚═╝ ╚═════╝
//************************ Jefferson National Lab (2017) ***********
//******************************************************************
//* Example program for reading HIPO-4 file.
//* shows initial implementation of data frames style columnar data reading.
//* run the example with following command line:
//*  ./dataframe.exe rec_clas_005197.evio.00100-00104.hipo REC::Particle px py pz chi2pid
//*--
//* Author: G.Gavalian
//*

#include <cstdlib>
#include <iostream>
#include "reader.h"
#include "twig.h"
#include "reaction.h"


int main(int argc, char** argv) {
  
  std::cout << " reading CLAS12 hipo file and plotting missing mass (ep->epi+pi-X) "  << __cplusplus << std::endl;
  //axis x(120,0.,1.0);
  //printf("bin = %d\n",x.find(0.4));
  
  char inputFile[256];
  char inputBank[256];
  char variable[256];

  std::vector<std::string> variables;
  
  if(argc>3) {
    snprintf(inputFile,256,"%s",argv[1]);
    snprintf(inputBank,256,"%s",argv[2]);
    for(int k = 3; k < argc; k++){
      snprintf(variable,256,"%s",argv[k]);
      variables.push_back(std::string(variable));
    }
    //sprintf(outputFile,"%s",argv[2]);
  } else {
    std::cout << " *** please provide a file name..." << std::endl;
    exit(1);
  }

  printf("reading file : %s\n",inputFile);
  printf("reading bank : %s\n",inputBank);

  for(int i = 0; i < (int) variables.size(); i++){
    printf("\t%6d : %s \n",i+1,variables[i].c_str());
  }

  hipo::reader r(inputFile);

  for(int k = 0; k < (int) variables.size(); k++){
    r.rewind();
    printf("printing vector [%s]\n",variables[k].c_str());
    std::vector<float> vec = r.getFloat(inputBank,variables[k].c_str(), 100000);
    for(int r = 0 ; r < (int) vec.size(); r++){
      printf(" row %6d = %f\n",r+1,vec[r]);
    }
  }

}
//### END OF GENERATED CODE
