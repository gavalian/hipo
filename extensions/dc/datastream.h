
#ifndef __DATASTREAM__
#define __DATASTREAM__

#include "reader.h"
#include "writer.h"
#include "utils.h"

namespace hipo {

class datastream {
    private:


      
      hipo::reader hr;
      hipo::writer hw;
      hipo::dictionary  factory;
      std::mutex obj;
      long  nProcessed = 0;
      hipo::benchmark  bench;
      long  nDataLimit = -1;
  
   public:

  //static int eof_printout;
  
  datastream(){ /*datastream::eof_printout = 0;*/}
    virtual ~datastream(){} 
    void open(const char *input, const char *output){        
           hr.open(input);           
           hr.readDictionary(factory);
           hw.addDictionary(factory);
           hw.open(output);
    }
    
    void getbank(const char *name, std::vector<hipo::bank> &banks, int count){
        banks.clear();
        for(int i = 0; i < count; i++) banks.push_back(hipo::bank(factory.getSchema(name))); 
    }

  void setLimit(long limit){
    nDataLimit = limit;
  }
  
  void pull(std::vector<hipo::event> &events){
        //printf("pull events %lu %d\n", events.size(), hr.hasNext());
        std::unique_lock<std::mutex> lock(obj);
	bool finished = false;
	if(nDataLimit>0){ if(nProcessed>nDataLimit) finished = true;}
	
	if(hr.hasNext()==false){//&&datastream::eof_printout==0){
	  //datastream::eof_printout = 1;
	  printf("\n");
	}
	
	for(int n = 0; n < events.size(); n++){
            // write the event in the output if it's not empty
            if(events[n].getSize()>16){ hw.addEvent(events[n]);}
            // reset event and read next in the file if any left
            events[n].reset();
            if(hr.next()==true&&finished==false){ 
                hr.read(events[n]); nProcessed++;
                if(nProcessed%250==0) { printf("."); fflush(stdout);}
		if(nProcessed%10000==0) printf(" : %9lu \n",nProcessed);               
                //printf("read event %d , size = %d\n",n,events[n].getSize());
                //events[n].show();
            }
        }
        //bench.pause();
        //printf("\n\n>>>>> finally events %14lu, rate = %12.8f sec/event , %9.3f event/sec\n",nProcessed ,
        //      bench.getTimeSec()/nProcessed, nProcessed/bench.getTimeSec() );
        //bench.resume();
    }
    long getProcessed(){return nProcessed; }
    void close(){hw.close();}
   };
}
#endif
