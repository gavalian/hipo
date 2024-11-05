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
#include "tuple.h"

//------------------------------------------------
// function to fill bank with random numbers
void writeTuple(){

  hipo::tuple nt;
  nt.setVerbose(2);
  nt.initBranches(12);
  nt.open("ntuple.hipo");

  float *array = new float[12];

   for(int k = 0; k < 500000; k++){
       for(int r = 0; r < 12; r++){
          array[r] = ((float) rand()) / (float) RAND_MAX;
       }
       nt.fill(array);
   }
   nt.close();
   delete[] array;
}

/**
*  Main Program ........
*/
int main(int argc, char** argv){
   writeTuple();
}
