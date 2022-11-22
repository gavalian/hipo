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
#include "evioBankUtil.h"

#define MAXBUF 10000000
unsigned int buf[MAXBUF];


void nextEvent(uint32_t  *buffer) { /* read next event into buffer */};
int  bankLink(uint32_t *buffer, int tag, int leaf, int num, int *length){ /* return the position of the given bank */ return 0;}

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
     exit(0);
   }

    
    hipo::composite bank;
    bank.parse(std::string("bssbsl"));
    //bank.show();
    //bank.print();

    coda::table table;
    table.read("dc_tt.txt");
    
    coda::decoder decoder;

    hipo::reader r;
    hipo::event  e;
    hipo::structure evio;

    r.open(inputFile);
    
    int fragnum = -1; /* always '-1' */
    int banktag = 0xe116; 
    int nbytes, ind_data;
    int ind12; int counter = 0;
    while(r.next()&&counter<10){
        counter++;
        r.read(e);
        e.getStructure(evio,32,1);
        int size = evio.getSize();
        printf(" --- size = %d\n",size);
        const unsigned int *bufptr_c = reinterpret_cast<const unsigned int *>(&evio.getAddress()[8]);
        unsigned int *bufptr = const_cast<unsigned int *>(bufptr_c);
        printf("evio buffer size = %d , in bytes = %d\n",bufptr[0],bufptr[0]*4);
        //unsigned int *bufptr = &bufptr_k;
        for(int fragtag=41; fragtag<=58; fragtag++)
        {
            for(int banknum=0; banknum<40; banknum++) /* loop over 40 bank numbers, it can be any from 0 to 39 */
                {
                    ind12 = evLinkBank(bufptr, fragtag, fragnum, banktag, banknum, &nbytes, &ind_data);
                    if(ind12>0){ 
		      printf(" --- found it");
		      printf("[%5d] %d %d %d %d\n",ind12, fragtag,banknum, nbytes, ind_data*4);
		      decoder.decode(fragtag,reinterpret_cast<const char*>(&bufptr[0]), ind_data*4, nbytes, table, bank);
	
			
                    }
                }

        }
	printf("event # %d : after decoding bank rows = %d\n" ,counter,bank.getRows() );
	//bank.print();
        //nextEvent(buf);
        //bank.setSize(0);
        //int length;
        //for(int board = 41; board < 48; board++){
        //    int  position = bankLink(buf,board, 0xe166, 0, &length);
        //    decoder.decode(board,reinterpret_cast<const char*>(&buf[0]), position, length, table, bank);
        //}
        //bank.print();
    }
}
