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
//*--
//* Author: G.Gavalian
//*

#include <cstdlib>
#include <iostream>
#include <cassert>
#include "reader.h"
#include "twig.h"
#include "reaction.h"

// example using hipo::bank::rowlist to loop through a bank's rows
void example1(const char *file){
  printf("===== EXAMPLE 1 =====\n");
  hipo::reader   r(file);
  hipo::banklist list = r.getBanks({"REC::Particle","REC::Event"});
  auto b_particle     = hipo::getBanklistIndex(list, "REC::Particle");
  auto b_event        = hipo::getBanklistIndex(list, "REC::Event");

  int counter = 0;
  while( r.next(list)&&counter<350){
    counter++;
    int nrows = list[b_particle].getRows();
    printf("\n>>> NEW EVENT <<<\n");
    printf(" rows = %d\n",nrows);

    // first, loop over all the rows
    printf(" loop over all rows:\n");
    for(auto const& row : list[b_particle].getRowList())
      printf("\t pid [%d] = %d\n", row, list[b_particle].getInt(0,row));

    // then, set the rowlist to contain only a few rows, and loop over those
    if(nrows>6){

      // set the list of rows, then loop over those
      printf(" there are more than 6 rows, let's loop over some specific rows: 0, 1, and 4\n");
      list[b_particle].getMutableRowList().setList({0, 1, 4});
      for(auto const& row : list[b_particle].getRowList())
        printf("\t pid [%d] = %d\n", row, list[b_particle].getInt(0,row));
      assert((list[b_particle].getRowList().size() == 3)); // test

      // it's still possible to access the full set of rows
      printf(" we can still loop over all the rows with `getFullRowList()`\n");
      for(auto const& row : list[b_particle].getFullRowList())
        printf("\t pid [%d] = %d\n", row, list[b_particle].getInt(0,row));
      printf(" or similarly with `getRows()`\n");
      for(int row=0; row<list[b_particle].getRows(); row++)
        printf("\t pid [%d] = %d\n", row, list[b_particle].getInt(0,row));
      assert((static_cast<int>(list[b_particle].getFullRowList().size()) == list[b_particle].getRows())); // test

      // you may also reset the list to its original, full state
      printf(" resetting the rowlist restores the full row list\n");
      list[b_particle].getMutableRowList().reset();
      for(auto const& row : list[b_particle].getRowList())
        printf("\t pid [%d] = %d\n", row, list[b_particle].getInt(0,row));
      assert((static_cast<int>(list[b_particle].getRowList().size()) == list[b_particle].getRows())); // test
    }

  }

  assert((b_particle == 0)); // test: validate `hipo::getBanklistIndex` returned the correct indices
  assert((b_event == 1));
}

// example showing how to filter a bank's rowlist with an expression
void example2(const char *file){
  printf("===== EXAMPLE 2 =====\n");
  hipo::reader   r(file);
  hipo::banklist list = r.getBanks({"REC::Particle","REC::Event"});
  auto b_particle = hipo::getBanklistIndex(list, "REC::Particle");
  int counter = 0;
  while( r.next(list)&&counter<350){
    counter++;
    printf("\n>>> NEW EVENT <<<\n");
    printf("=== BEFORE ==================================\n");
    list[b_particle].show();
    list[b_particle].getMutableRowList().filter("charge!=0");
    printf("=== AFTER ===================================\n");
    list[b_particle].show();
    // list[b_particle].show(true); // call `show(true)` if you still need to see the full (not filtered) bank
  }
}

// example showing how to filter a bank's rowlist with an first-order (lambda) function
void example3(const char *file){
  printf("===== EXAMPLE 3 =====\n");

  hipo::reader   r(file);
  hipo::banklist list = r.getBanks({"REC::Particle","REC::Event"});
  auto b_particle = hipo::getBanklistIndex(list, "REC::Particle");
  std::function charged = [](hipo::bank &b, int row) { return b.getInt("charge",row)!=0 ? 1.0 : 0.0;};

  int counter = 0;
  while( r.next(list)&&counter<350){
    counter++;
    printf("\n>>> NEW EVENT <<<\n");
    printf("=== BEFORE ==================================\n");
    list[b_particle].show();
    list[b_particle].getMutableRowList().filter(charged);
    printf("=== AFTER ===================================\n");
    list[b_particle].show();
    // list[b_particle].show(true); // call `show(true)` if you still need to see the full (not filtered) bank
  }
}

// Example showing how rowlists can be used to get indicies of the reference bank.
// The link function will return a hipo::bank::rowlist for the bank "REC::Calorimeter"
// where the column number 1 (which is "pindex") has a value of 0. Then by itarating over the returned
// indicies the total energy can be calculated.
void example4(const char *file){
  printf("===== EXAMPLE 4 =====\n");
  hipo::reader   r(file);
  hipo::banklist list = r.getBanks({"REC::Particle","REC::Calorimeter"});
  auto b_particle = hipo::getBanklistIndex(list, "REC::Particle");
  auto b_cal      = hipo::getBanklistIndex(list, "REC::Calorimeter");
  int const pindex_column = 1; // of REC::Calorimeter
  int const electron_row = 0;
  int counter = 0;

  while( r.next(list)&&counter<350){
    counter++;
    int status = list[b_particle].getInt("status",electron_row);
    if(list[b_particle].getInt("pid",electron_row)==11&&abs(status)>=2000&&abs(status)<3000){
      printf("\n>>> NEW EVENT <<<\n");
      printf("found electron in row %d\n", electron_row);
      double energy = 0.0;
      for(auto const& it : list[b_cal].getRowListLinked(electron_row, pindex_column)){
        auto e = list[b_cal].getFloat("energy",it);
        printf(" => links REC::Calorimeter row=%d, which has energy %f\n", it, e);
        energy += e;
      }
      printf("=========================\ntotal energy = %f\n=========================\n",energy);
      list[b_particle].show();
      list[b_cal].show();
    }
  }
}

int main(int argc, char** argv) {

  std::cout << " reading CLAS12 hipo file and plotting missing mass (ep->epi+pi-X) "  << __cplusplus << std::endl;
  //axis x(120,0.,1.0);
  //printf("bin = %d\n",x.find(0.4));

  char inputFile[256];
  int example_num = -1;

  if(argc>1) {
    snprintf(inputFile,256,"%s",argv[1]);
    //sprintf(outputFile,"%s",argv[2]);
  } else {
    std::cerr << " *** please provide a file name..." << std::endl;
    exit(1);
  }
  if(argc>2)
    example_num = std::stoi(argv[2]);

  switch(example_num) {
    case -1:
      example1(inputFile);
      example2(inputFile);
      example3(inputFile);
      example4(inputFile);
      break;
    case 1: example1(inputFile); break;
    case 2: example2(inputFile); break;
    case 3: example3(inputFile); break;
    case 4: example4(inputFile); break;
    default:
            std::cerr << " *** ERROR: unknown example..." << std::endl;
            exit(1);
  }

}
