//******************************************************************************
//*        ██╗  ██╗██╗██████╗  ██████╗     ███████╗    ██████╗ 
//*        ██║  ██║██║██╔══██╗██╔═══██╗    ██╔════╝   ██╔═████╗
//*        ███████║██║██████╔╝██║   ██║    ███████╗   ██║██╔██║
//*        ██╔══██║██║██╔═══╝ ██║   ██║    ╚════██║   ████╔╝██║
//*        ██║  ██║██║██║     ╚██████╔╝    ███████║██╗╚██████╔╝
//*        ╚═╝  ╚═╝╚═╝╚═╝      ╚═════╝     ╚══════╝╚═╝ ╚═════╝
//************************ Jefferson National Lab (2023) ***********************
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
/*******************************************************************************
 * File:   bank.h
 * Author: gavalian
 *
 * Created on April 12, 2017, 10:14 AM
 */

#ifndef HIPO_NODE_H
#define HIPO_NODE_H
#include <iostream>
#include <vector>
#include <cstring>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <tuple>

namespace hipo {

  class node {

    private:
      
      std::vector<char>  nodeBuffer;
      const char        *nodePointer;

    protected:

      void create(int group, int item, int type, int size);

      //void initStructureBySize(int __group, int __item, int __type, int __size);
      //std::vector<char>  &getStructureBuffer(){ return structureBuffer;}
      //int                 getStructureBufferSize(){ return 8+getSize();}
      //int                 dataOffset = 8;
      //std::vector<char> structureBuffer;
      void init(const char *b, int length){ allocate(length); memcpy(const_cast<char *>(nodePointer),b,length);}
      void initEmpty();

  public:

      node(){ allocate(8); nodePointer = &nodeBuffer[0];}
      node(std::tuple<int,int,int,int> params){
	create(std::get<0>(params),std::get<1>(params), std::get<2>(params), std::get<3>(params));
      }
      node(int size){ allocate(8+size); nodePointer = &nodeBuffer[0];}

      //structure(int __group, int __item, std::string &str);

      virtual     ~node()= default;
      
      void         assign(std::tuple<int,int,int,int> params );

      bool         allocate(int size){
        if(((int) nodeBuffer.size()) <size){
          nodeBuffer.resize(size+8); nodePointer = &nodeBuffer[0];
        } return true;
      }
    
      virtual void         reset(){ setDataLength(0);}

      int          size() const noexcept{
        int length = *reinterpret_cast<const uint32_t *>(nodePointer+4);
	       return length&0x00ffffff;
         //return getHeaderSize()+getDataSize();
      }
      
      int          capacity() const noexcept{
	       return (int) nodeBuffer.size();
         //return getHeaderSize()+getDataSize();
      }

      int         formatLength() const noexcept {
         int length = *reinterpret_cast<const uint32_t *>(nodePointer+4);
         return (length>>24)&0x000000ff;
      }

      void setFormatLength(int length){
          if(length<128){
            char *dest = const_cast<char *>(nodePointer);
            *reinterpret_cast<uint32_t *>(dest+4) = ((length<<24)&0xFF000000)|(length&0x00FFFFFF);
          } else { printf("node::setFormatLength: error >>> the format length can not exceed 128, you tried to set it to %d\n",length);}
      }

      void        setDataLength(int length){
        int word = *reinterpret_cast<const uint32_t *>(nodePointer+4);
        int flength = (word>>24)&0x000000FF;
        setNodeLength(flength+length);
         /*int word = *reinterpret_cast<const uint32_t *>(nodePointer+4);
         int flength = (word>>24)&0x000000FF;
         int totalLength = length + flength;
         char *dest = const_cast<char *>(nodePointer);
         *reinterpret_cast<uint32_t *>(dest+4) = (word&0xFF000000)|(totalLength&0x00FFFFFF);*/
      }

      int         dataLength() const noexcept {         
        int  size = (*reinterpret_cast<const uint32_t *>(nodePointer+4))&0x00FFFFFF;
        int fsize = ((*reinterpret_cast<const uint32_t *>(nodePointer+4))>>24)&0x000000FF;
	      return size-fsize;
      }

      int          nodeLength(){          
          int  size = (*reinterpret_cast<const uint32_t *>(nodePointer+4))&0x00FFFFFF;
          return size;
      }

      void        setNodeLength(int size){
          if(size<16777215){
             uint32_t    word = *reinterpret_cast<const uint32_t *>(nodePointer+4);
             uint32_t     nodeSize = (word&0xFF000000)|(size&0x00FFFFFF);
             char *dest = const_cast<char *>(nodePointer);
             *reinterpret_cast<uint32_t *>(dest+4) = nodeSize;
          } else { printf("node::setNodeLength:: error: the total size of the node exceeds 16777215\n");}
      }

      int          dataOffset() const noexcept;

      int          group(){return (int) (*reinterpret_cast<const uint16_t *>(nodePointer));}
      int          item(){return (int) (*reinterpret_cast<const uint8_t *>(nodePointer+2));}
      int          type(){return (int) (*reinterpret_cast<const uint8_t *>(nodePointer+3));}

      //void         init(const char *buffer, int size);
      //void         initNoCopy(const char *buffer, int size);

      const char    *pointer(){ return nodePointer;};

      virtual void   show();
      
      void           setSize(int size);
      //void           setHeaderSize(int size);
      //void           setDataSize(int size);

      int          getIntAt   ( int index) const noexcept {
        return *reinterpret_cast<const int32_t*>(nodePointer + index + dataOffset());
      }
    
      int16_t      getShortAt ( int index) const noexcept {
        return *reinterpret_cast<const int16_t*>(nodePointer + index + dataOffset());
      }
      int8_t       getByteAt  ( int index) const noexcept {
        return *reinterpret_cast<const int8_t*>(nodePointer + index + dataOffset());
      }
      float        getFloatAt ( int index) const noexcept {
        return *reinterpret_cast<const float*>(nodePointer + index + dataOffset());
      }
      double       getDoubleAt( int index) const noexcept {
        return *reinterpret_cast<const double*>(nodePointer + index + dataOffset());
      }
      long         getLongAt  ( int index) const noexcept {
        return *reinterpret_cast<const int64_t*>(nodePointer + index + dataOffset());
      }

      //std::string  getStringAt(int index);

      void         putIntAt(int index, int value){
        char *a = const_cast<char*>(nodePointer);
        *reinterpret_cast<int32_t*>(a + index + dataOffset()) = value;
      }

      void         putShortAt(int index, int16_t value){
        char *a = const_cast<char*>(nodePointer);
        *reinterpret_cast<int16_t*>(a + index + dataOffset()) = value;
      }

      void         putByteAt(int index, int8_t value){
        char *a = const_cast<char*>(nodePointer);
        *reinterpret_cast<int8_t*>(a + index + dataOffset()) = value;
      }

      void         putFloatAt(int index, float value){
        char *a = const_cast<char*>(nodePointer);
        *reinterpret_cast<float*>(a + index + dataOffset()) = value;
      }

      void         putDoubleAt(int index, double value){
        char *a = const_cast<char*>(nodePointer);
        *reinterpret_cast<double*>(a + index + dataOffset()) = value;
      }

      void         putLongAt(int index, int64_t value){
        char *a = const_cast<char*>(nodePointer);
        *reinterpret_cast<int64_t*>(a + index + dataOffset()) = value;
      }

      //void         putStringAt(int index, std::string &str);

      virtual void notify(){}

      friend class tuple;
      friend class event;
  };

  inline int node::dataOffset() const noexcept { 
    int fsize = ((*reinterpret_cast<const uint32_t *>(nodePointer+4))>>24)&0x000000FF;
    return 8+fsize;
  }
}
#endif /* NODE_H */
