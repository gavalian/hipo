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
#include <vector>
#include "reader.h"
#include "record.h"


void debug1(const char *file){

  hipo::reader  reader;
  reader.open(file);

  hipo::dictionary  dict;
  reader.readDictionary(dict);
  
  auto run_config_sch =   dict.getSchema("RUN::config");
  run_config_sch.show();
  hipo::bank  runinfo(dict.getSchema("RUN::config"));
  auto rec_particle_sch = dict.getSchema("REC::Particle");
  rec_particle_sch.show();
  hipo::bank  particles(dict.getSchema("REC::Particle"));
  hipo::bank  rasteradc(dict.getSchema("RASTER::adc"));
  hipo::bank  raster(dict.getSchema("RASTER::position"));
  hipo::event event;
  
  int counter = 0;
  //int particle_counter = 0;
  int current_event = 2520;

  reader.gotoEvent(current_event);
  
  while( reader.gotoEvent(current_event)==true){  //
    reader.read(event);
    current_event++;
    counter++;
    event.getStructure(runinfo);
    printf("Evt: %6d  = run: %5d  event: %6d \n", current_event, runinfo.getInt("run", 0), runinfo.getInt("event", 0));
  }
  printf("processed events = %d\n",counter);
}



void debug2(const char *file){
  hipo::reader  reader;
  hipo::record  record;
  reader.open(file);
  hipo::dictionary  dict;
  reader.readDictionary(dict);

  hipo::bank  runinfo(dict.getSchema("RUN::config"));
  int nrec = reader.getNRecords();
  std::vector<std::pair<int,int>> events;

  for(int i =0 ;i < nrec; i++){
    reader.loadRecord(record,i);
    //printf("record # %8d events = %d\n",i,record.getEventCount());
    record.getEventsMap(events);
    for(int r = 0; r < events.size(); r++){
      // printf("\t event # %8d, start -> %8d , end -> %8d (size = %8d)\n",
       //r,events[r].first,events[r].second, events[r].second-events[r].first);
       record.read(runinfo,r);
       //runinfo.show();
    }
  }
}

int main(int argc, char** argv) {

   std::cout << " reading file example program (HIPO) "  << __cplusplus << std::endl;

   char inputFile[256];

   if(argc>1) {
      sprintf(inputFile,"%s",argv[1]);
      //sprintf(outputFile,"%s",argv[2]);
   } else {
      std::cout << " *** please provide a file name..." << std::endl;
     exit(0);
   }


//   debug1(inputFile);
    debug2(inputFile);

}
//### END OF GENERATED CODE
