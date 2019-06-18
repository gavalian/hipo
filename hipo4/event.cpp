/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "event.h"

namespace hipo {


    event::event(){
        #if __cplusplus > 199711L
            printf("\n*****>>>>> compiled with c++11 support.\n");
        #endif
        // default allocation size for the event is 20 Kb
        //
        dataBuffer.resize(20*1024);
        reset();
        //printf("creating event class.....\n");
        //hipo::node<int> *type = new hipo::node<int>();
        //nodes.push_back(type);
    }

    event::event(int size){
        dataBuffer.resize(size);
        reset();
    }

    event::~event(){

    }

    void   event::getStructure(hipo::bank &b){
        getStructure(b,b.getSchema().getGroup(),b.getSchema().getItem());
    }

    void event::getStructure(hipo::structure &str, int group, int item){
       std::pair<int,int> index = getStructurePosition(group,item);
       if(index.first>0){
         str.init(&dataBuffer[index.first], index.second + 8);
         str.notify();
       } else {
         str.initStructureBySize(group,item,1,0);
         str.notify();
         //printf("*** error *** : structure (%5d,%5d) does not exist\n", group,item);
       }
    }

    void    event::addStructure(hipo::structure &str){
        int str_size = str.getStructureBufferSize();
        int evt_size = getSize();
        memcpy(&dataBuffer[evt_size], &str.getStructureBuffer()[0],str_size);
        *(reinterpret_cast<uint32_t*>(&dataBuffer[4])) = (evt_size + str_size);
    }

    void event::init(std::vector<char> &buffer){
        dataBuffer.resize(buffer.size());
        std::memcpy(&dataBuffer[0],&buffer[0],buffer.size());
    }

    std::pair<int,int>  event::getStructurePosition(int group, int item){
      int position = 16;
      int eventSize = *(reinterpret_cast<uint32_t*>(&dataBuffer[4]));
      while(position+8<eventSize){
          uint16_t   gid = *(reinterpret_cast<uint16_t*>(&dataBuffer[position]));
          uint8_t    iid = *(reinterpret_cast<uint8_t*>(&dataBuffer[position+2]));
          uint8_t   type = *(reinterpret_cast<uint8_t*>(&dataBuffer[position+3]));
          int     length = *(reinterpret_cast<int*>(&dataBuffer[position+4]));
          //printf("group = %4d , item = %4d\n",(unsigned int) gid, (unsigned int) iid);
          if(gid==group&&iid==item) return std::make_pair(position,length);
          position += (length + 8);
      }
      return std::make_pair(-1,0);
    }

    void event::init(const char *buffer, int size){
        if(dataBuffer.size()<=size){
         dataBuffer.resize(size);
       }
       std::memcpy(&dataBuffer[0],buffer,size);
       *(reinterpret_cast<uint32_t*>(&dataBuffer[4])) = size;
    }

    int event::getSize(){
      return *(reinterpret_cast<uint32_t*>(&dataBuffer[4]));
    }
    void event::reset(){
        dataBuffer[0] = 'E'; dataBuffer[1] = 'V';
        dataBuffer[2] = 'N'; dataBuffer[3] = 'T';
        *(reinterpret_cast<uint32_t*>(&dataBuffer[ 4])) = 16;
        *(reinterpret_cast<uint32_t*>(&dataBuffer[ 8])) = 0;
        *(reinterpret_cast<uint32_t*>(&dataBuffer[12])) = 0;
    }
    std::vector<char> &event::getEventBuffer(){ return dataBuffer;}
    /*
    template<class T>   node<T> event::getNode(){
        node<T> en;
        en.setLength(4);
        en.setAddress(NULL);
    } */
    void event::show(){
        printf(" EVENT  SIZE = %d\n",getSize());
        int position = 16;
        int eventSize = *(reinterpret_cast<uint32_t*>(&dataBuffer[4]));
        while(position+8<eventSize){
            uint16_t   gid = *(reinterpret_cast<uint16_t*>(&dataBuffer[position]));
            uint8_t    iid = *(reinterpret_cast<uint8_t*>(&dataBuffer[position+2]));
            uint8_t   type = *(reinterpret_cast<uint8_t*>(&dataBuffer[position+3]));
            int     length = *(reinterpret_cast<int*>(&dataBuffer[position+4]));
            //printf("group = %4d , item = %4d\n",(unsigned int) gid, (unsigned int) iid);
            //if(gid==group&&iid==item) return std::make_pair(position,length);
            printf("%12s %9d %4d %12d %12d\n"," ",gid,iid,type,length);
            position += (length + 8);
        }
    }
}
