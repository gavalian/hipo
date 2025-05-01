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
//* Reads the file using iterator
//*--
//* Author: M.Ouillon
//*

#include "hipoeventiterator.h"

int main(int argc, char** argv) {

    std::cout << " reading file example program (HIPO) "  << __cplusplus << std::endl;
 
    char inputFile[256];
 
    if(argc>1) {
      snprintf(inputFile,256,"%s",argv[1]);
       //sprintf(outputFile,"%s",argv[2]);
    } else {
       std::cout << " *** please provide a file name..." << std::endl;
      exit(1);
    }

    int counter = 0;

    hipo::hipoeventfile events(inputFile);
    
    for (auto event : events) {

        hipo::bank particles = event.get_bank("event::particle");

        int nrows = particles.getRows();
        printf("---------- PARTICLE BANK CONTENT -------\n");
        for(int row = 0; row < nrows; row++){
           int   pid = particles.getInt("pid",row);
           float  px = particles.getFloat("px",row);
           float  py = particles.getFloat("py",row);
           float  pz = particles.getFloat("pz",row);
           printf("%6d : %6d %8.4f %8.4f %8.4f\n",row,pid,px,py,pz);
        }
        printf("---------- END OF PARTICLE BANK -------\n");

        counter++;
    }
    printf("processed events = %d\n",counter);

} 