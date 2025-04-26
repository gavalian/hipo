//******************************************************************
//*  ██╗  ██╗██╗██████╗  ██████╗     ██╗  ██╗    ██████╗
//*  ██║  ██║██║██╔══██╗██╔═══██╗    ██║  ██║   ██╔═████╗
//*  ███████║██║██████╔╝██║   ██║    ███████║   ██║██╔██║
//*  ██╔══██║██║██╔═══╝ ██║   ██║    ╚════██║   ████╔╝██║
//*  ██║  ██║██║██║     ╚██████╔╝         ██║██╗╚██████╔╝
//*  ╚═╝  ╚═╝╚═╝╚═╝      ╚═════╝          ╚═╝╚═╝ ╚═════╝
//************************ Jefferson National Lab (2017) ***********
//******************************************************************
//* Example program for writing HIPO-4 Files..
//* Includes defining schemas, opening a file with dictionary
//*--
//* Author: G.Gavalian
//*


#include <iostream>
#include <stdlib.h>
#include "table.h"
#include "utils.h"
#include "bank.h"
#include "decoder.h"
#include "reader.h"
#include "writer.h"
#include "evioBankUtil.h"

#define MAXBUF 10000000
#define ONE_MB 1048576
#define ONE_KB 1024

unsigned int buf[MAXBUF];


void nextEvent(uint32_t  *buffer) { /* read next event into buffer */};
int  bankLink(uint32_t *buffer, int tag, int leaf, int num, int *length){ /* return the position of the given bank */ return 0;}

void processFrame(const char *data);
/**
*  Main Program ........
*/
int main(int argc, char** argv){

    printf("--->>> example program for data translation\n");
    char inputFile[256];

   if(argc>1) {
      sprintf(inputFile,"%s",argv[1]);
      //sprintf(outputFile,"%s",argv[2]);
   } else {
      std::cout << " *** please provide a file name..." << std::endl;
     exit(1);
   }
    
    //bank.show();
    //bank.print();

    coda::table table;
    table.read("dc_tt.txt");
    
    coda::decoder decoder;

    hipo::reader r;

    hipo::event  e(24*ONE_KB);
    hipo::event  eout(24*ONE_KB);

    hipo::structure evio(2*ONE_MB);

    r.open(inputFile);
    
    hipo::writer w;
    w.open("output.h5");


    hipo::dataframe    frame(50,4*ONE_MB); // frame(maximum number of events, maximum number of bytes); 

    int fragnum = -1; /* always '-1' */
    int banktag = 0xe116; 
    int nbytes, ind_data;
    int ind12; int counter = 0;

    hipo::composite bank;//(42,11,std::string("bssbsl"),1024*1024*2);
    bank.parse(42,11,std::string("bssbsl"),2*ONE_MB);
    //bank.parse(std::string("bssbsl"));

    while(r.next()&&counter<450000){

        counter++;
        r.read(e);
        e.getStructure(evio,32,1);        
        //int size = evio.getSize();                
        bank.reset();
        const unsigned int *bufptr_c = reinterpret_cast<const unsigned int *>(&evio.getAddress()[8]);
        unsigned int         *bufptr = const_cast<unsigned int *>(bufptr_c);
        //printf("evio buffer size = %d , in bytes = %d\n",bufptr[0],bufptr[0]*4);
        //unsigned int *bufptr = &bufptr_k;
        for(int fragtag=41; fragtag<=58; fragtag++)
        {
            for(int banknum=0; banknum<40; banknum++) // loop over 40 bank numbers, it can be any from 0 to 39 
                {
                    ind12 = evLinkBank(bufptr, fragtag, fragnum, banktag, banknum, &nbytes, &ind_data);
                    if(ind12>0){ 
		                //printf(" --- found it");
		                //printf("[%5d] %d %d %d %d\n",ind12, fragtag,banknum, nbytes, ind_data*4);
		                decoder.decode(fragtag,reinterpret_cast<const char*>(&bufptr[0]), ind_data*4, nbytes, table, bank);
                    }
                }

        }
	    //printf("event # %d : after decoding bank rows = %d\n" ,counter,bank.getRows() );
	    eout.reset();
        eout.addStructure(bank);
    
        bool success = frame.addEvent(eout);

        if(success==false){
            processFrame(frame.buffer());
            frame.reset();
            frame.addEvent(eout);
        } 
    }
    printf("processed events %d\n",counter);
}

//=========================================================================================
//** The process routine to initiate frame from (char *) buffer, then read events one by 
//** one and read the bank and prin on the screen
//=========================================================================================

void processFrame(const char *data){

    hipo::dataframe frame;
    hipo::event     event(2*ONE_MB);

    frame.init(data);
    int size = frame.size();
    printf("frame size = %d\n",size);

    frame.summary();
    int eventCount = frame.count();
    int pos = 56;
    hipo::composite bank(2*ONE_MB);
    for(int i = 0; i < eventCount; i++){
        pos = frame.getEventAt(pos,event);
        printf("\t event # %d -> size = %d, position = %d\n",i,event.getSize(), pos);
        event.getStructure(bank,42,11);
        bank.show();
        printf(" nrows = %d\n", bank.getRows() );
        bank.print();
    }
}
