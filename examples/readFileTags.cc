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
#include "writer.h"

void dataFill(hipo::bank &bank){
    int    nrows = bank.getRows();
    int nentries = bank.getSchema().getEntries();
    for(int row = 0; row < nrows; row++){
       for(int e = 0 ; e < nentries; e++){
           int type = bank.getSchema().getEntryType(e);
           if(type==1||type==2||type==3){
              int inum = rand()%20;
              bank.putInt(bank.getSchema().getEntryName(e).c_str(),row,inum);
           }
           if(type==4){
             float ifloat = ((float) rand()) / (float) RAND_MAX;
             bank.putFloat(bank.getSchema().getEntryName(e).c_str(),row,ifloat);
           }
       }
    }
}

void writeFileWithTags(){
   
   hipo::schema  sch_part("event::particle",100,1);   
   std::string a("{\"type\":\"gcard\", \"version\": 1.0}");
   std::string b("{\"detector\":\"pcal\", \"threshold\": 0.423}");

   hipo::structure desc1(1000,1,a);
   hipo::structure desc2(1000,1,b);
   
   sch_part.parse("pid/S,px/F,py/F,pz/F");

   hipo::writer  writer;
   writer.getDictionary().addSchema(sch_part);

   writer.open("example_file_tags.hipo");

   hipo::event outEvent;
   for(int i = 0; i < 5; i++){
       int  nparts = 2 + rand()%10;
       hipo::bank partBank(sch_part,nparts);
       dataFill(partBank);

       outEvent.reset();
       outEvent.setTag(0);
       outEvent.addStructure(partBank);
       writer.addEvent(outEvent);
   }

   outEvent.reset();
   outEvent.setTag(2);
   outEvent.addStructure(desc1);
   writer.addEvent(outEvent);

   outEvent.reset();
   outEvent.setTag(2);
   outEvent.addStructure(desc2);
   writer.addEvent(outEvent);

   writer.close();
}

void readFileWithTags(){
   hipo::reader  reader;
   reader.setTags(2);
   reader.open("example_file_tags.hipo");

   hipo::event      event;

   hipo::structure description;

   while(reader.next()==true){
      reader.read(event);
      event.getStructure(description,1000,1);
      printf("string : %s\n",description.getStringAt(0).c_str());
   }
}

int main(int argc, char** argv) {
   writeFileWithTags();
   printf("\n\nnow read the strings from tag = 2;\n\n");
   readFileWithTags();
}

/*
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

   hipo::reader  reader;
   reader.setTags(1);
   reader.open(inputFile);
   hipo::dictionary  factory;
   reader.readDictionary(factory);

   factory.show();

   hipo::bank  helicity(factory.getSchema("HEL::flip"));

   hipo::event      event;

   int counter = 0;

   while(reader.next()==true){
      reader.read(event);

      event.getStructure(helicity);
      helicity.show();

      counter++;
   }
   printf("processed events = %d\n",counter);
}*/
//### END OF GENERATED CODE
