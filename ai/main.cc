#include <iostream>
#include "reader.h"
#include "dcana.h"
#include "utils.h"

void readRaw(clas12::match &m, hipo::bank &hits, int sector, int superlayer){

  int rows = hits.getRows();

//  std::vector<clas12::match> m;
//  m.resize(6);
  for(int i = 0; i < rows; i++){

    int sec   = hits.getInt(0,i);
    int layer = hits.getInt(1,i);
    int comp  = hits.getInt(2,i);

/*    int sec   = hits.getInt("sector",i);
    int layer = hits.getInt("layer",i);
    int comp  = hits.getInt("component",i);
*/
    if(sec==sector){
      int   region = (int) (layer-1)/6;
      if(region==superlayer){
        int sublayer = layer - region*6 -1;
        m.addPoint(comp-1,sublayer,i);
      }
    }
  }
}

int main(int argc, char** argv){

  printf("----->>> start testing program \n");
  char inputFile[256];
  char inputFileComp[256];


  clas12::sector sector;

  //sector.show();
/*
  std::vector<clas12::cluster> clusters;
  clas12::cluster a;

  for(int i = 0; i < 5; i++){
    a.setSector(i);
    a.setRegion(i*4+1);
    clusters.push_back(a);
  }
*/
  //for(int i = 0; i < clusters.size(); i++) clusters[i].print();
  //return 1;
   if(argc>1) {
      sprintf(inputFile,"%s",argv[1]);
      //sprintf(inputFileComp,"%s",argv[2]);
      //sprintf(outputFile,"%s",argv[2]);
   } else {
      std::cout << " *** please provide a file name..." << std::endl;
     exit(0);
   }

  hipo::reader reader;

  hipo::dictionary  factory;
  reader.open(inputFile);
  reader.readDictionary(factory);

  //hipo::bank  dchits(factory.getSchema("TimeBasedTrkg::TBHits"));
  hipo::bank  dctdc(factory.getSchema("DC::tdc"));


  hipo::event      event;

  int counter = 0;

  clas12::dcana  analyzer;
  hipo::benchmark  readBench("read");
  hipo::benchmark  matchBench("match");
  hipo::benchmark  sectorBench("sector");

  while(reader.next()==true&&counter<100000){
     counter++;
     reader.read(event);
     event.getStructure(dctdc);
     int dcSize = dctdc.getRows();
     //dctdc.show();
     //printf(" EVENT # - %d size = %d\n",counter,dcSize);
     clas12::match m;
     readBench.resume();
     readRaw(m,dctdc,1,5);
     readBench.pause();
     clas12::cluster c = clas12::cluster::createMap(m);


     matchBench.resume();
     m.find(2,4);
     m.sort();
     matchBench.pause();
     std::vector<clas12::cluster> cv = clas12::cluster::create(m);
     if(cv.size()>1){
     printf("--------- CLUSTERS SIZE = %d\n", (int) cv.size());
     c.print();
     printf(" - ");
     for(int cs = 0; cs < cv.size(); cs++){
       cv[cs].print();
     }
   }
     //m.show();
     /*sectorBench.resume();
     sector.reset();
     sector.readRaw(dctdc,1);
     sector.makeTracks();
     sectorBench.pause();
     */
     //sector.show();

   }

   readBench.show();
   matchBench.show();
   sectorBench.show();

}
