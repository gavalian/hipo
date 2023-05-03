//******************************************************************
//*  ██╗  ██╗██╗██████╗  ██████╗     ██╗  ██╗    ██████╗
//*  ██║  ██║██║██╔══██╗██╔═══██╗    ██║  ██║   ██╔═████╗
//*  ███████║██║██████╔╝██║   ██║    ███████║   ██║██╔██║
//*  ██╔══██║██║██╔═══╝ ██║   ██║    ╚════██║   ████╔╝██║
//*  ██║  ██║██║██║     ╚██████╔╝         ██║██╗╚██████╔╝
//*  ╚═╝  ╚═╝╚═╝╚═╝      ╚═════╝          ╚═╝╚═╝ ╚═════╝
//************************ Jefferson National Lab (2017) ***********
//******************************************************************
//* Denoising application for CLAS12 drift chambers
//*--
//* Author: G.Gavalian
//*


#include <fdeep/fdeep.hpp>
#include "drift.h"
#include "writer.h"
#include "utils.h"
#include "datastream.h"
#include <thread>
#include "cmdparser.hpp"



   std::vector<hipo::event> dataframe;
   std::vector <hipo::bank> databanks;
   const fdeep::model                *model;
   dc::drift chamber;
   hipo::datastream stream;

   int  nThreads = 8;
   int   nFrames = 16;

   double threshold = 0.05;

   std::string network_file = "network/cnn_autoenc_0f_112.json";

/**
 * @brief Create a Frame object
 *  Creates an array of events
 * @param dataframe 
 * @param size 
 */
void createFrame(std::vector<hipo::event> &dataframe, int size){
     dataframe.clear();
     for(int i = 0; i < size; i++) dataframe.push_back(hipo::event());
}
/**
 * @brief loads vector of events from a file. If entries expire rest of
 * the events will be empty.
 * @param reader 
 * @param dataframe 
 */
void loadFrame(hipo::reader &reader, std::vector<hipo::event> &dataframe){ 
   for(int loop = 0; loop < dataframe.size(); loop++){
       dataframe[loop].reset();
       if(reader.next()==true) reader.read(dataframe[loop]);
   }
}
/**
 * @brief loads vector of banks from vector of events this is to pass it to
 * drift class to process individual banks.
 * @param dataframe 
 * @param databanks 
 */
void  loadBanks(std::vector<hipo::event> &dataframe, std::vector<hipo::bank> &databanks){ 
    for(int loop = 0; loop < dataframe.size(); loop++)
       dataframe[loop].getStructure(databanks[loop]);
}
/**
 * @brief function to be called for each thread.
 * 
 * @param chamber 
 * @param model 
 * @param bank 
 */
void function(int order){
  
  std::vector<hipo::event> events;
  std::vector<hipo::bank>  banks;
  createFrame(events,nFrames);
  stream.getbank("DC::tdc",banks,nFrames);
 
  int isAlive = 1;
  int totRows = 0;
  int sigRows = 0;
  //printf("-- start the thread %d, frames = %d\n", order, nFrames);
  while(isAlive==1){
      stream.pull(events);
      int nNonEmpty = 0;
      for(int k = 0; k < events.size(); k++){
        if(events[k].getSize()>16){
           nNonEmpty++;
           events[k].getStructure(banks[k]);
           if(banks[k].getRows()>0){
             sigRows += chamber.processRange(*model,banks[k]);
	     // -- old processorsigRows += chamber.process(*model,banks[k]);
	     totRows += banks[k].getRows();
             events[k].replace(banks[k]);
           } else {
	     //printf("bank dc has size=0\n");
           }
        }
      }
      //printf("non empty = %d\n",nNonEmpty);
     if(nNonEmpty==0) isAlive = 0;
  }
  printf(">>> thread #%3d : %12d/%12d , reduction %8.5f\n", order, totRows, sigRows, ((double) sigRows)/totRows);
}

void runstream(){//},std::vector<hipo::bank> *banks){//}, fdeep::model &model, std::vector<hipo::bank> &banks){
    std::vector<std::thread*> threads;
    for(int i = 0; i < nThreads; i++){
      //printf("-- creating thread , order = %4d\n",i);
        threads.push_back(new std::thread(function,i));
    //   std::thread thread(function,chamber,model,banks[i]);
    //   thread.join();
    }
    printf("-- created denoiser with %lu threads\n", threads.size());
    for(int k = 0; k < threads.size(); k++) threads[k]->join();
    for(int k = 0; k < threads.size(); k++) delete threads[k];
    //threads.clear();
    //printf("---- done with %ld threads \n", databanks.size());
}
/**
 * @brief configure command line parser with input arguments
 */
void configure_parser(cli::Parser& parser){
  parser.set_required<std::string>("i", "input", "input file name");
  parser.set_optional<std::string>("o", "output", "output.h5", "output denoised file name");
  parser.set_optional<std::string>("n","network","network/cnn_autoenc_0f_112.json", "neural network file name");
  
  parser.set_optional<int>("t", "threads",  8,"number of threads to run");
  parser.set_optional<int>("f",  "frames", 16,"number of events in each frame");
  parser.set_optional<double>("l", "level", 0.05,"cut off level for background hits");
  parser.set_optional<int>("e", "events", -1,"maximum number of events to process");
}

/*
===============================================================================================
==  MAIN program to run de-noising on CLAS 12 Drift Chamber Data..
================================================================================================
*/
int main(int argc, char** argv){

std::cout << std::endl;
std::cout << __cplusplus << "--------------------------- "  << __cplusplus << std::endl;
std::cout << __cplusplus << "-- AI denoising program  -- "  << __cplusplus << std::endl;
std::cout << __cplusplus << "--------------------------- "  << __cplusplus << std::endl;
std::cout << std::endl;

   cli::Parser parser(argc, argv);
   configure_parser(parser);
   parser.run_and_exit_if_error();
 
   /*char inputFile[256];
   if(argc>1) { sprintf(inputFile,"%s",argv[1]);} else {
      std::cout << " *** please provide a file name..." << std::endl;exit(0);
      }*/

   chamber.setRows(112);

   std::string  inputFile = parser.get<std::string>("i");
   std::string outputFile = parser.get<std::string>("o");
   std::string network_file = parser.get<std::string>("n");
   
   double threshold = parser.get<double>("l");
   nThreads = parser.get<int>("t");
   nFrames  = parser.get<int>("f");
   chamber.setThreshold(threshold);
   
   printf("\n--\n");
   printf("\n*********************************************************\n");
   printf("::   input file : %s \n",inputFile.c_str());
   printf("::  output file : %s \n",outputFile.c_str());
   printf(":: network file : %s \n",network_file.c_str());
   printf("::  num threads : %d \n",nThreads);
   printf("::   frame size : %d \n",nFrames);
   printf("::    threshold : %.9f \n",threshold);
   printf("*********************************************************\n");
   printf("\n--\n");
   
   /*if(argc>2) nThreads = atoi(argv[2]);
   if(argc>3) nFrames = atoi(argv[3]);
   if(argc>4) chamber.setThreshold(atof(argv[4]));
   */
   
   stream.open(inputFile.c_str(),outputFile.c_str());
   stream.setLimit(parser.get<int>("e"));
   
   printf("--\n-- opening the neural network file\n--\n");
   //const auto modelLocal = fdeep::load_model("network/cnn_autoenc_0f_112.json");
   const auto modelLocal = fdeep::load_model(network_file.c_str());
   //const auto modelLocal = fdeep::load_model("network/cnn_autoenc_cpp.json");
   model = &modelLocal;
   
   hipo::benchmark  processBench;
   
   //hipo::benchmark    totalBench;
   processBench.resume();
   std::thread progress(runstream);
   progress.join();
   processBench.pause();
   
   stream.close();
   long counter = stream.getProcessed();
   
   printf("\n\n>>>>> finally events finally events %d , %d - %14lu, time = %9.3f sec, rate = %12.8f sec/event , %9.3f event/sec\n",
	  nThreads, nFrames,counter ,processBench.getTimeSec(),
	  processBench.getTimeSec()/counter, counter/processBench.getTimeSec() );
   
}
