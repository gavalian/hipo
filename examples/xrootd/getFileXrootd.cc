#include <iostream>
#include <XrdClient/XrdClient.hh>
#include <fstream>
#include <vector>
#include <string>
#include <sys/time.h>
#include <math.h>

#include "XrdClient/XrdClientEnv.hh"
#include "XrdSys/XrdSysHeaders.hh"
#include "reader.h"
#include "writer.h"

kXR_unt16 open_mode = (kXR_ur);
kXR_unt16 open_opts = (1);

//To compile
//g++ mytest.cc -I/usr/include/xrootd -lXrdClient -o mytest
//g++ readFile.cc -I/usr/local/Cellar/xrootd/4.10.1/include/xrootd -L/usr/local/lib -lXrdClient -o readFile
std::string cmd_example = "./getFileXrootd -skip 1000 -n 2000 -f xroot://sci-xrootd.jlab.org//osgpool/hallb/example_large.hipo";

std::string getOption(const char *key, int argc, char **argv){
  for(int i = 0; i < argc; i++){
    std::string item = argv[i];
    if(item.compare(key)==0){
      return std::string(argv[i+1]);
    }
  }
  return std::string("");
}

int main(int argc, char** argv)
{

  std::string address = "xroot://sci-xrootd.jlab.org//osgpool/hallb/example_large.hipo";

  int nSkip   = 0;
  int nEvents = 10000;
  char  inputFile[512];
  char  outputFile[128];

  if(argc<7){
    printf("\n\nPlease provide arguments...\n");
    printf("\nExample : %s\n\n",cmd_example.c_str());
    exit(0);
  } else {
    std::string skipString = getOption("-skip",argc,argv);
    std::string    nString = getOption("-n",argc,argv);
    std::string fileString = getOption("-f",argc,argv);
    printf("skip = %s\n",skipString.c_str());
    printf("n    = %s\n",nString.c_str());
    printf("file   = %s\n",fileString.c_str());
    nSkip   = atoi(skipString.c_str());
    nEvents = atoi(nString.c_str());
    sprintf(inputFile,"%s",fileString.c_str());
    sprintf(outputFile,"%s","xrootd_output.hipo");
  }

  hipo::reader      reader;
  hipo::dictionary  factory;

  reader.open(inputFile);
  reader.readDictionary(factory);
  factory.show();

  hipo::writer  writer;

  writer.getDictionary().addDictionary(factory);
  writer.open(outputFile);

  hipo::event event;

  int counter = 0;

  reader.gotoEvent(nSkip);

  while(reader.next()==true&&counter<nEvents){//&&counter<10000){
      reader.read(event);
      writer.addEvent(event);
      //event.getStructure(particles);
      //event.getStructure(detectors);
      //particles.show();
      //detectors.show();
      counter++;
  }
  writer.close();
  printf("processed events # %d\n", counter );
  return 1;
}

//********************************************************************************
// SAMPLE CODE USED TO DEBUG
// //XrdClient *cli = new XrdClient("xroot://sci-xrootd.jlab.org//osgpool/hallb/text2.txt");
// XrdClient *cli = new XrdClient("xroot://sci-xrootd.jlab.org//osgpool/hallb/example_output.hipo");
//XrdClient *cli = new XrdClient("xroot://sci-xrootd.jlab.org//home/gavalian/test4.txt");
//  cli->Open(open_mode,open_opts);
//  XrdClientStatInfo stats;
//     cli->Stat(&stats);
//     printf("File Size = %lld\n", stats.size);
//  int nread = cli->Read(mybuffer,0,256);
//  printf("number of bytes read = %d\n",nread);
//std::cout << mybuffer ;
//for(int i = 0; i < 14; i++){
//printf("%3d : 0x%08X\n", i,  reinterpret_cast<int *> (mybuffer)[i]);
//}
//return 1;
//********************************************************************************
