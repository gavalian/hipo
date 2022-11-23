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
  printf("-- start the thread %d, frames = %d\n", order, nFrames);
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
  printf("-- info : thread %d finished\n",order);
  /*int start = order * nFrames;
  for(int k = 0; k < nFrames; k++){
     chamber.process(*model,databanks[start+k]);
  }*/
}

void runstream(){//},std::vector<hipo::bank> *banks){//}, fdeep::model &model, std::vector<hipo::bank> &banks){
    std::vector<std::thread*> threads;
    for(int i = 0; i < nThreads; i++){
      printf("-- creating thread , order = %4d\n",i);
        threads.push_back(new std::thread(function,i));
    //   std::thread thread(function,chamber,model,banks[i]);
    //   thread.join();
    }
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

  printf("\n\n>>>>> finally events %14lu, rate = %12.8f sec/event , %9.3f event/sec\n",counter ,
              processBench.getTimeSec()/counter, counter/processBench.getTimeSec() );
/*


   std::vector<float> data;
   
   //for(int i = 0; i < 36*112; i++) data.push_back(1.0);
   const auto modelLocal = fdeep::load_model("network/cnn_autoenc_cpp.json");
   model = &modelLocal;
   //const auto model = fdeep::load_model("cnn_autoenc_produced.json");     
   
   hipo::reader  reader;
   reader.open(inputFile);
   
   hipo::dictionary  factory;
   reader.readDictionary(factory);
   
   //factory.show();
   
   hipo::bank  dcb(factory.getSchema("DC::tdc"));
   
   hipo::event      event;
   
   hipo::benchmark  processBench;
   hipo::benchmark    totalBench;
   
   int counter = 0;
   int totalEvents = 0;
   //std::vector<hipo::event> dataframe;
   //std::vector <hipo::bank> databanks;
   
   createFrame(dataframe,nThreads*nFrames);
   
   for(int loop = 0 ; loop < nThreads*nFrames; loop++) 
     databanks.push_back(hipo::bank(factory.getSchema("DC::tdc")));

   printf("\n\n--\n");
   printf("-- starting denoising process with %lu frames\n",dataframe.size());
   printf("-- n threads = %4d, events per thread = %d\n",nThreads, nFrames);
   printf("--\n--\n");
   hipo::writer  writer;
   writer.addDictionary(factory);
   writer.open("denoiser_output.h5");
   
   totalBench.resume();
   
   while(reader.hasNext()==true){

    counter++; totalEvents += databanks.size();
    if((counter)%50==0){
      totalBench.pause();
      printf(">>>>> processed events %14d, rate = %12.8f sec/event , %9.3f evt/second\n",totalEvents ,
              totalBench.getTimeSec()/totalEvents, totalEvents/totalBench.getTimeSec() );
      totalBench.resume();
    }
    //  reader.read(event);
    //   event.getStructure(dcb);
      
    //  chamber.process(model,dcb);
    loadFrame(reader,dataframe);
    loadBanks(dataframe,databanks);

    //printf("--- lunching threads %ld\n",dataframe.size());
    
    //std::thread progress(runframe,chamber,model,databanks);
    std::thread progress(runframe);
    progress.join();
      

    for(int loop = 0; loop < dataframe.size(); loop++){
      dataframe[loop].remove(databanks[loop]);
      dataframe[loop].addStructure(databanks[loop]);
      writer.addEvent(dataframe[loop]);
    }
      //event.remove(dcb);
      //event.addStructure(dcb);
      //writer.addEvent(event);
      //dcb.show();
   }

   writer.close();

   totalBench.pause();

  printf("\n\n>>>>> finally events %14d, rate = %12.8f sec/event , %9.3f event/sec\n",counter ,
              totalBench.getTimeSec()/totalEvents, totalEvents/totalBench.getTimeSec() );

/*
      std::vector<int>  index;
      for(int s = 1; s <= 6; s++){
        chamber.initvec(dcmap);
        chamber.create(dcmap,dcb,s);
        fdeep::tensors result = model.predict(
				        {fdeep::tensor(
					  	     fdeep::tensor_shape(static_cast<std::size_t>(36),
						  			 static_cast<std::size_t>(114),
							  		 static_cast<std::size_t>(1)),
						         dcmap)}
				        );
        auto output = result[0].as_vector();
        std::vector<float>  buffer;
        for(int jj=0; jj< output->size(); jj++) buffer.push_back( (*output)[jj]);
        chamber.map(index,buffer,dcb,s);
        
        //printf("[sec=%d] dc size = %d, index size = %ld\n", s, dcb.getRows(), index.size());
      }
      //printf("[sec=%d] dc size = %d, index size = %ld\n", 1, dcb.getRows(), index.size());
      //printf("-------------------\n");
      for(int nn = 0; nn < dcb.getRows(); nn++) dcb.putByte("order",nn,(int8_t) 10);
      int nindex = index.size();      
      for(int nn = 0; nn < nindex; nn++){
         dcb.putByte("order",index[nn],(int8_t) 0);
      }
*/
/*

  for(int k = 0; k < 1000; k++){
    //const auto result = model.predict(
    fdeep::tensors result = model.predict(
				      {fdeep::tensor(
						     fdeep::tensor_shape(static_cast<std::size_t>(36),
									 static_cast<std::size_t>(112),
									 static_cast<std::size_t>(1)),
						     //		     std::vector<float>{1, 2, 3, 4})}
						     data)}
				      );
     auto output = result[0].as_vector();
     printf("size = %ld\n",output->size());
     for(int j = 0; j < output->size(); j++){
       printf("elemend %d = %f\n",j,(*output)[j]);
     }
  }
  */
  
  //std::cout << fdeep::show_tensors(result) << std::endl;
}
