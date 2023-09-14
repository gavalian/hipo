//******************************************************************************
//*       ██╗  ██╗██╗██████╗  ██████╗     ██╗  ██╗    ██████╗                  *
//*       ██║  ██║██║██╔══██╗██╔═══██╗    ██║  ██║   ██╔═████╗                 *
//*       ███████║██║██████╔╝██║   ██║    ███████║   ██║██╔██║                 *
//*       ██╔══██║██║██╔═══╝ ██║   ██║    ╚════██║   ████╔╝██║                 *
//*       ██║  ██║██║██║     ╚██████╔╝         ██║██╗╚██████╔╝                 *
//*       ╚═╝  ╚═╝╚═╝╚═╝      ╚═════╝          ╚═╝╚═╝ ╚═════╝                  *
//************************ Jefferson National Lab (2017) ***********************
/*
 *   Copyright (c) 2017.  Jefferson Lab (JLab). All rights reserved. Permission
 *   to use, copy, modify, and distribute  this software and its documentation
 *   for educational, research, and not-for-profit purposes, without fee and
 *   without a signed licensing agreement.
 *
 *   IN NO EVENT SHALL JLAB BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL
 *   INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS, ARISING
 *   OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF JLAB HAS
 *   BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *   JLAB SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 *   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *   PURPOSE. THE HIPO DATA FORMAT SOFTWARE AND ACCOMPANYING DOCUMENTATION, IF
 *   ANY, PROVIDED HEREUNDER IS PROVIDED "AS IS". JLAB HAS NO OBLIGATION TO
 *   PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 *
 *   This software was developed under the United States Government license.
 *   For more information contact author at gavalian@jlab.org
 *   Department of Experimental Nuclear Physics, Jefferson Lab.
 */
 /*
  * File:   event.h
  * Author: gavalian
  *
  * Created on April 12, 2017, 10:14 AM
  */

#include "event.h"

namespace hipo {


    event::event(){
        #if __cplusplus > 199711L
            //printf("\n*****>>>>> compiled with c++11 support.\n");
        #endif
        // default allocation size for the event is 128 Kb
        dataBuffer.resize(128*1024);
        reset();
    }

    event::event(int size){
        dataBuffer.resize(size);
        reset();
    }

    event::~event()= default;

    void   event::getStructure(hipo::bank &b){
        getStructure(b,b.getSchema().getGroup(),b.getSchema().getItem());
    }

    void   event::read(hipo::bank &b){
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

    void event::getStructure4(hipo::structure &str, int group, int item){
       std::pair<int,int> index = getStructurePosition4(group,item);
       //printf("\n\n>>> GET STRUCTURE %d %d -> %d %d \n",group,item, index.first, index.second);
       if(index.first>0){
         str.init(&dataBuffer[index.first], index.second + 8);
         str.notify();
       } else {
         str.initStructureBySize(group,item,1,0);
         str.notify();
         //printf("*** error *** : structure (%5d,%5d) does not exist\n", group,item);
       }
    }

    void     event::getStructureNoCopy(const char *buffer, hipo::structure &str, int group, int item){
       std::pair<int,int> index = getStructurePosition(buffer,group,item);
       if(index.first>0){
         str.initNoCopy(&buffer[index.first], index.second + 8);
         str.notify();
       } else {
         str.initStructureBySize(group,item,1,0);
         str.notify();
         //printf("*** error *** : structure (%5d,%5d) does not exist\n", group,item);
       }
    }
    void     event::getStructure(const char *buffer, hipo::structure &str, int group, int item){
       std::pair<int,int> index = getStructurePosition(buffer,group,item);
       if(index.first>0){
         str.init(&buffer[index.first], index.second + 8);
         str.notify();
       } else {
         str.initStructureBySize(group,item,1,0);
         str.notify();
         //printf("*** error *** : structure (%5d,%5d) does not exist\n", group,item);
       }
    }
    void   event::override(hipo::structure &str){

    }

    void   event::replace(hipo::bank &bank){
      std::pair<int,int> index = getStructurePosition(&dataBuffer[0],
            bank.getSchema().getGroup(),bank.getSchema().getItem());
      if(index.first>0){
          int oSize =   index.second + 8;
          int sSize = bank.getSize() + 8;
          if(oSize==sSize){
             std::memcpy(&dataBuffer[index.first],&(bank.getStructureBuffer()[0]),sSize);
          } else {
            printf("[event::replace] error in replacing the bank %s\n",bank.getSchema().getName().c_str());
          }
      }       
    }
    void   event::remove(hipo::bank &str){
       remove(str.getSchema().getGroup(),str.getSchema().getItem());
    }

    void   event::remove(int group, int item){
       //int str_size = str.getStructureBufferSize();
       //int data_size = str.getSize();
       std::pair<int,int> index = getStructurePosition(&dataBuffer[0],group,item);
       printf("-removing %d, %d\n",index.first,index.second);
       if(index.first>0){
	        int eventSize = getSize();
	        int    toCopy = eventSize - index.first - (index.second + 8);
	        int   newSize = eventSize - (index.second + 8);
	        std::memcpy(&dataBuffer[index.first],&dataBuffer[index.first+8+index.second],toCopy);
          printf(" first = %d, second = %d\n",index.first,index.second);
          printf(" \t memcopy = %d, %d, ncopy %d , size = %d , new size = %d\n", 
          index.first, index.first+8+index.second, toCopy, eventSize, newSize
          );
	          *(reinterpret_cast<uint32_t*>(&dataBuffer[4])) = newSize;
       }
       //printf("size = %d, new size = %d, structure removed size = %d, position = %d, to copy = %d\n",
       //   eventSize, newSize, index.second +8, index.first, toCopy );
    }

    void    event::addStructure(hipo::structure &str){

        int str_size = str.getStructureBufferSize();
        int data_size = str.getSize();
        int evt_size = getSize();
	      int evt_capacity = dataBuffer.size();

        //if(dataBuffer.size()<= () ){
        //  dataBuffer.resize(size+1024);
        //}
        if(data_size>0){
          //printf(" writing structure %d/%d - size %d, data size %d\n",str.getGroup(),str.getItem(),str_size,data_size);
	        if((evt_size + str_size)<evt_capacity){
	           memcpy(&dataBuffer[evt_size], &str.getStructureBuffer()[0],str_size);
	            //*(reinterpret_cast<uint32_t*>(&dataBuffer[4])) = (evt_size + str_size + 24);
              *(reinterpret_cast<uint32_t*>(&dataBuffer[4])) = (evt_size + str_size);
	          } else {
	              printf("event::add : error adding structure with size = %5d (capacity = %5d, size = %5d)\n",
		              str_size,evt_capacity, evt_size);
	          }
        }
    }

  void   event::add(hipo::node &_n){

      int _n_data_length = _n.dataLength();
      if(_n_data_length==0) return;

      int ev_size = getSize();
      int _n_size = _n.size() + 8;
      int ev_capacity = dataBuffer.size();
      if((ev_size + _n_size)<ev_capacity){
        memcpy(&dataBuffer[ev_size], _n.pointer(),_n_size);
        *(reinterpret_cast<uint32_t*>(&dataBuffer[4])) = (ev_size + _n_size);
      } else {
        printf("event::add : error adding structure with size = %5d (capacity = %5d, size = %5d)\n",
          _n_size,ev_capacity, ev_size);
      }
  }
  void   event::write(hipo::node &node){
      add(node);
  }

  void   event::read(hipo::node  &node, int group, int item){

  }

void event::get(hipo::node &_n, int group, int item){
       std::pair<int,int> index = getStructurePosition(group,item);
       printf(" found the structure : %d/%d - properties = %d, %d\n",group,item,index.first,index.second);
       if(index.first>0){
         _n.init(&dataBuffer[index.first], index.second + 8);
         printf("calling notify.....\n");
         _n.notify();
       } else {
         _n.initEmpty();
         _n.notify();
         //printf("*** error *** : structure (%5d,%5d) does not exist\n", group,item);
       }
    }

    int  event::getTag(){
      int eventTag = *(reinterpret_cast<const uint32_t*>(&dataBuffer[8]));
      return eventTag;
    }

    void  event::setTag(int tag){
      *(reinterpret_cast<uint32_t*>(&dataBuffer[8])) = tag;
    }

    void event::init(std::vector<char> &buffer){
        dataBuffer.resize(buffer.size());
        std::memcpy(&dataBuffer[0],&buffer[0],buffer.size());
    }

    std::pair<int,int>  event::getStructurePosition(const char *buffer, int group, int item){
      int position = 16;
      int eventSize = *(reinterpret_cast<const uint32_t*>(&buffer[4]));
      while(position+8<eventSize){
          uint16_t   gid = *(reinterpret_cast<const uint16_t*> (&buffer[position  ]));
          uint8_t    iid = *(reinterpret_cast<const uint8_t*>  (&buffer[position+2]));
          uint8_t   type = *(reinterpret_cast<const uint8_t*>  (&buffer[position+3]));
          int     length = *(reinterpret_cast<const int*>      (&buffer[position+4]));
          //printf("group = %4d , item = %4d\n",(unsigned int) gid, (unsigned int) iid);
          if(gid==group&&iid==item) return std::make_pair(position,length);
          position += (length + 8);
      }
      return std::make_pair(-1,0);
    }

    std::pair<int,int>  event::getStructurePosition(int group, int item){
      int position = 16;
      int eventSize = *(reinterpret_cast<uint32_t*>(&dataBuffer[4]));
      while(position+8<eventSize){
          uint16_t   gid = *(reinterpret_cast<uint16_t*>(&dataBuffer[position]));
          uint8_t    iid = *(reinterpret_cast<uint8_t*>(&dataBuffer[position+2]));
          uint8_t   type = *(reinterpret_cast<uint8_t*>(&dataBuffer[position+3]));
          int       word = *(reinterpret_cast<int*>(&dataBuffer[position+4]));
          //printf("group = %4d , item = %4d\n",(unsigned int) gid, (unsigned int) iid);
          int length = word&0x00FFFFFF;
          if(gid==group&&iid==item) return std::make_pair(position,length);
          position += (length + 8);
      }
      return std::make_pair(-1,0);
    }

    std::pair<int,int>  event::getStructurePosition4(int group, int item){
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
/*
    std::pair<int,int>  event::getStructurePosition(const char *buffer, int group, int item){
      int position = 16;
      int eventSize = *(reinterpret_cast<uint32_t*>(&buffer[4]));
      while(position+8<eventSize){
          uint16_t   gid = *(reinterpret_cast<uint16_t*>(&buffer[position]));
          uint8_t    iid = *(reinterpret_cast<uint8_t*>(&buffer[position+2]));
          uint8_t   type = *(reinterpret_cast<uint8_t*>(&buffer[position+3]));
          int     length = *(reinterpret_cast<int*>(&buffer[position+4]));
          //printf("group = %4d , item = %4d\n",(unsigned int) gid, (unsigned int) iid);
          if(gid==group&&iid==item) return std::make_pair(position,length);
          position += (length + 8);
      }
      return std::make_pair(-1,0);
    }
*/
    void event::init(const char *buffer, int size){
       if(dataBuffer.size()<=size){
         dataBuffer.resize(size+1024);
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
	    int     sizeWord = *(reinterpret_cast<int*>(&dataBuffer[position+4]));
            int     length = sizeWord&0x00FFFFFF;
	    int     format = (sizeWord>>24)&0x000000FF;
            //printf("group = %4d , item = %4d\n",(unsigned int) gid, (unsigned int) iid);
            //if(gid==group&&iid==item) return std::make_pair(position,length);
            printf("%12s node [%9d %4d] type = %12d, fotmat size = %3d , length = %12d\n"," ",gid,iid,type,format,length);
            position += (length + 8);
        }
    }



    void     event::get(const char *buffer, hipo::node &_n, int group, int item){
       std::pair<int,int> index = getStructurePosition(buffer,group,item);
       if(index.first>0){
         _n.init(&buffer[index.first], index.second + 8);
         //_n.notify();
       } else {
         _n.initEmpty();
         _n.notify();
         //printf("*** error *** : structure (%5d,%5d) does not exist\n", group,item);
       }
    }
}
