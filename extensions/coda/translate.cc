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

#define MAXBUF 10000000
unsigned int buf[MAXBUF];


void nextEvent(uint32_t  *buffer) { /* read next event into buffer */};
int  bankLink(uint32_t *buffer, int tag, int leaf, int num, int *length){ /* return the position of the given bank */ return 0;}

/**
*  Main Program ........
*/
int main(int argc, char** argv){

    printf("--->>> example program for data translation\n");


    hipo::composite bank;
    bank.parse(std::string("bssbsl"));
    //bank.show();
    bank.print();

    coda::table table;
    table.read("dc_tt.txt");
    
    coda::decoder decoder;

    nextEvent(buf);
    bank.setSize(0);
    int length;
    for(int board = 41; board < 48; board++){
        int  position = bankLink(buf,board, 0xe166, 0, &length);
        decoder.decode(board,reinterpret_cast<const char*>(&buf[0]), position, length, table, bank);
    }
    bank.print();
}
