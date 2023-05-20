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
#include "writer.h"

#define MAXBUF 10000000
#define ONE_MB 1048576
#define ONE_KB 1024
//------------------------------------------------
// function to fill bank with random numbers
void bankFill(hipo::composite &bank, int nrows);

/**
*  Main Program ........
*/
int main(int argc, char** argv){

    printf("--->>> example program to produce a HIPO file.");
    char outputFile[256];
    sprintf(outputFile,"%s","example_composite.hipo");
    //---------------------------------------------------------
    // Define a composite bank
    hipo::composite bank;
    bank.parse(42,11,std::string("bssbsl"),2*ONE_MB);
    //---------------------------------------------------------
    // Open a writer with empty schema list 
    hipo::writer  writer;
    writer.open(outputFile);
    hipo::event outEvent;

    for(int i = 0; i < 250; i++){

        int   nrows = 2 + rand()%10;

        bank.reset();
        bankFill(bank,nrows);
        bank.show();
        bank.print();
        //-------------------------------------------------
        // Create banks with random rows based on schemas
        outEvent.reset();
        outEvent.addStructure(bank);
        writer.addEvent(outEvent);
    }
    writer.close();
}
//********************************************************
// SUBROUTINE FILLS RANDOM VALUES INTO THE bank
// FIRST CHECK THE TYPE OF THE ENTRY THEN GENERATE
// RANDOM NUMBER AND FILL.
//********************************************************
void bankFill(hipo::composite &bank, int nrows){
    int nentries = bank.getEntries();

    for(int row = 0; row < nrows; row++){
        for(int entry = 0; entry < nentries; entry++){
             int type = bank.getEntryType(entry);
             switch(type){
                case 1: bank.putInt(row,entry, row+1); break;
                case 2: bank.putInt(row,entry, row+1); break;
                case 3: bank.putInt(row,entry, row+1); break;
                case 4: bank.putFloat(row,entry, row+1); break;
                //case 5: bank.putDouble(row,entry, row); break;
                case 8: bank.putLong(row,entry, row*10+10); break;
                default: printf("error: unknown entry type : %d\n",type); break;
             }
        }
    }
}
