#include <iostream>
#include "reader.h"
#include "dcana.h"


int main(int argc, char** argv){

  printf("----->>> start testing program \n");
  char inputFile[256];

   if(argc>1) {
      sprintf(inputFile,"%s",argv[1]);
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
  hipo::bank  dchits(factory.getSchema("HitBasedTrkg::HBHits"));

  hipo::event      event;

  int counter = 0;

  clas12::dcana  analyzer;

  while(reader.next()==true){
    counter++;
     reader.read(event);
     event.getStructure(dchits);
     //dchits.show();
     printf("########### EVENT %6d #############\n",counter);
     analyzer.readClusters(dchits,1);
     analyzer.makeTracks();
     analyzer.showFeatures();
     //analyzer.showTracks();
   }

}
