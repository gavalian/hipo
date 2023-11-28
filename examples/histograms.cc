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
#include "twig.h"
#include "reaction.h"

/**
 * @brief Example one shows how to do fast physics analysis with HIPO-4 
 * package included utilities, this routine calculates the missing mass
 * of electron, pion+ and pion- from CLAS12 data banks
 * @param file file name to process
 */
void example1(const char *file){

  // open a file, initialize internal beam vector to 10.6 gev.
  // and set event filter to:
  // 11 (electron) at least 1, 211 - (pi+) at least 1,
  // -211 (pi-) at least one
  fizika::reaction r(file,10.6,{{11,1},{211,1},{-211,1}});

  twig::h1d  hmx(120,0.5,2.8); // declare a 1D histogram
  while(r.next()){ // loop through file
    if(r.is_valid()){ // if the condition of 11:1 , 211:1 and -211:1 is met
      // get vector center mass (beam + target, where beam energy is passed through constructor
      // ,and target is considered a proton, can be changed) and subtruct the combined vector
      // of electron (11) , pi+ (211) and pi- (-211).
      // the arguments of {1,211,0,0.13957} are the following:
      // 1 - with positive sign ( use -1 if you want to subtract the vector)
      // 211 - the pid in particle pank (pi+ in this case)
      // 0 - how many pi+ to skip (0 means give me the first pi+ found)
      // 0.13957 - is the mass to assign to the lorentz vector when doing the math
      // the all three vectors passed to the get(...) function are added together
      // with the sign that is the first argument of the request string.
      fizika::lorentz4 vmx = r.cm() - r.get({ {1,11,0,0.0005}, {1,211,0,0.13957},{1,-211,0,0.13957}});
      
      hmx.fill(vmx.m()); // fill the histogram
    }
  } 
  hmx.print(); // print the histogram in the terminal
}

/**
 * @brief Example shows how to use physics libraries included with HIPO-4, 
 * using REC::Particle bank from CLAS12 data, it calculates the invariant 
 * mass of two photons.
 * 
 * @param file data file name to process
 */
void example2(const char *file){

  // open a file, initialize internal beam vector to 10.6 gev.
  // and set event filter to:
  // 11 (electron) at least 1, 22 - (photon) at least 2,
  fizika::reaction r(file,10.6,{{11,1},{22,2}}); 
  twig::h1d  hpi0(120,0.05,0.45); // declare a 1D histogram
  while(r.next()){ // loop through the file
    if(r.is_valid()){ // if the condition 11:1 and 22:2 is met
      // add vectors of first and second photons and return the result
      fizika::lorentz4 vgg =  r.get({ {1,22,0,0.0}, {1,22,1,0.0}});
      hpi0.fill(vgg.m()); //
    }
  } 
  hpi0.print();
}

/**
 * @brief Exampel showing how to read a banks from file and print them on 
 * on the screen.
 * 
 * @param file data file name to process
 */
void example3(const char *file){
   hipo::reader   r(file);
   hipo::banklist list = r.getBanks({"REC::Particle","REC::Event"});
   int counter = 0;
   while( r.next(list)&&counter<350){ counter++; list[0].show(); list[1].show();}
}
/**
 * @brief Example file showing how to read REC::Particle banks and fill
 * a histogram with the vertex z-coordinate for all charged particles 
 * in the event, and print the histogram on the screen.
 * 
 * @param file data file name to process
 */
void example4(const char *file){
   twig::h1d hz(120,-15,5);
   hipo::reader   r(file);
   hipo::banklist list = r.getBanks({"REC::Particle"});
   while( r.next(list)){ 
      for(int r = 0; r < list[0].getRows(); r++){
         if(list[0].getInt("charge",r)!=0) hz.fill(list[0].getFloat("vz",r));
      }
   }
   hz.print();
}

int main(int argc, char** argv) {
  
  std::cout << " reading CLAS12 hipo file and plotting missing mass (ep->epi+pi-X) "  << __cplusplus << std::endl;
  //axis x(120,0.,1.0);
  //printf("bin = %d\n",x.find(0.4));
  
  char inputFile[256];
  
  if(argc>1) {
    sprintf(inputFile,"%s",argv[1]);
    //sprintf(outputFile,"%s",argv[2]);
  } else {
    std::cout << " *** please provide a file name..." << std::endl;
    exit(0);
  }

  //example3(inputFile);

  example1(inputFile);
  example2(inputFile);

  example4(inputFile);
}
//### END OF GENERATED CODE
