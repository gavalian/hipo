#include <iostream>
#include "reader.h"
#include "dcana.h"


int main(int argc, char** argv){

  printf("----->>> start testing program \n");
  char inputFile[256];
  char inputFileComp[256];


  clas12::sector sector;
  clas12::sector sector_c;
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
      sprintf(inputFileComp,"%s",argv[2]);
      //sprintf(outputFile,"%s",argv[2]);
   } else {
      std::cout << " *** please provide a file name..." << std::endl;
     exit(0);
   }

  hipo::reader reader;
  hipo::reader reader_c;

  hipo::dictionary  factory;
  reader.open(inputFile);
  reader_c.open(inputFileComp);

  reader.readDictionary(factory);

  //hipo::bank  dchits(factory.getSchema("TimeBasedTrkg::TBHits"));
  /*
  hipo::bank  dchits(factory.getSchema("HitBasedTrkg::HBHits"));
  hipo::bank  dctracks(factory.getSchema("HitBasedTrkg::HBTracks"));
  hipo::bank  dchits_c(factory.getSchema("HitBasedTrkg::HBHits"));
  hipo::bank  dctracks_c(factory.getSchema("HitBasedTrkg::HBTracks"));
*/
hipo::bank  dchits(factory.getSchema("TimeBasedTrkg::TBHits"));
hipo::bank  dctracks(factory.getSchema("TimeBasedTrkg::TBTracks"));
hipo::bank  dchits_c(factory.getSchema("TimeBasedTrkg::TBHits"));
hipo::bank  dctracks_c(factory.getSchema("TimeBasedTrkg::TBTracks"));
  hipo::event      event;
  hipo::event      event_c;

  int counter = 0;

  clas12::dcana  analyzer;

  while(reader.next()==true){
    counter++;
     reader.read(event);
     event.getStructure(dchits);
     event.getStructure(dctracks);

     reader_c.next();
     reader_c.read(event_c);
     event_c.getStructure(dchits_c);
     event_c.getStructure(dctracks_c);
     sector_c.reset();
     sector_c.read(dchits_c,1);
     sector_c.readTrackInfo(dctracks_c);
     sector_c.makeTracks();
     //dchits.show();
     //printf("########### EVENT %6d #############\n",counter);
     sector.reset();
     sector.read(dchits,1);
     sector.readTrackInfo(dctracks);
     sector.makeTracks();
     //printf(" TRACK COUNT = %5d\n", sector.getTrackCount());
     if(sector.getTrackInfoCount(-1,1)<2){
       std::string dataALGO = sector.getTrackInfoString(-1,1);
       std::string dataNNAI = sector_c.getTrackInfoString(-1,1);
       printf("%5d %s %s\n",sector.getTrackCount(),dataALGO.c_str(),dataNNAI.c_str());
     }
     //sector.showTrackInfo();
     //sector_c.showTrackInfo();
     //sector.show();

//     analyzer.readClusters(dchits,1);
//     analyzer.makeTracks();
//     analyzer.showFeatures();
     //analyzer.showTracks();
   }

}
