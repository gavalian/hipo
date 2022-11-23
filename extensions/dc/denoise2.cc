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


   std::vector<hipo::event> dataframe;
   std::vector <hipo::bank> databanks;
   const fdeep::model                *model;
   dc::drift chamber;
hipo::datastream stream;

   int  nThreads = 8;
   int   nFrames = 16;

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
  //printf("-- start the thread %d, frames = %d\n", order, nFrames);
  while(isAlive==1){
      stream.pull(events);
      int nNonEmpty = 0;
      for(int k = 0; k < events.size(); k++){
        if(events[k].getSize()>16){
           nNonEmpty++;
           events[k].getStructure(banks[k]);
           chamber.process(*model,banks[k]);
           events[k].remove(banks[k]);
           events[k].addStructure(banks[k]);
        }
      }           
      //printf("non empty = %d\n",nNonEmpty);
     if(nNonEmpty==0) isAlive = 0;
  }
  //printf("-- info : thread %d finished\n",order);
  /*int start = order * nFrames;
  for(int k = 0; k < nFrames; k++){
     chamber.process(*model,databanks[start+k]);
  }*/
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


int main(int argc, char** argv){

std::cout << std::endl;
std::cout << __cplusplus << "--------------------------- "  << __cplusplus << std::endl;
std::cout << __cplusplus << "-- AI denoising program  -- "  << __cplusplus << std::endl;
std::cout << __cplusplus << "--------------------------- "  << __cplusplus << std::endl;
std::cout << std::endl;

   char inputFile[256];

   if(argc>1) {
      sprintf(inputFile,"%s",argv[1]);
      //sprintf(outputFile,"%s",argv[2]);
   } else {
      std::cout << " *** please provide a file name..." << std::endl;
     exit(0);
   }


   if(argc>2) nThreads = atoi(argv[2]);
   if(argc>3)  nFrames = atoi(argv[3]);

    stream.open(inputFile,"output.h5");
    printf("--\n-- opening the neural network file\n--\n");
    const auto modelLocal = fdeep::load_model("network/cnn_autoenc_cpp.json");
    model = &modelLocal;
    
    hipo::benchmark  processBench;

    //hipo::benchmark    totalBench;
    processBench.resume();
    std::thread progress(runstream);
    progress.join();
    processBench.pause();

    stream.close();
    long counter = stream.getProcessed();

  printf("\n\n>>>>> finally events %14lu, time = %9.3f sec, rate = %12.8f sec/event , %9.3f event/sec\n",counter ,processBench.getTimeSec(),
              processBench.getTimeSec()/counter, counter/processBench.getTimeSec() );

}
