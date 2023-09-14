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
/*******************************************************************************
 * File:   bank.cc
 * Author: gavalian
 *
 * Created on April 12, 2017, 10:14 AM
 */

#include "node.h"
#include "utils.h"

namespace hipo {

  //==============================================================
  // Definition of class structure, this will class will be extended
  // to represent different objects that will be appended to the event
  //==============================================================

    void node::create(int group, int item, int type, int size){
        allocate(size);
        char *__a = const_cast<char*> (nodePointer);
        *reinterpret_cast<uint16_t *>(__a) = (uint16_t) group;
        *reinterpret_cast<uint8_t *>(__a+2) = (uint8_t) item;
        *reinterpret_cast<uint8_t *>(__a+3) = (uint8_t) type;
        *reinterpret_cast<uint32_t *>(__a+4) = 0x00000000;
    }
  
  void    node::setSize(int size){
    char *__a = const_cast<char*> (nodePointer);
    *reinterpret_cast<uint32_t *>(__a+4) = size;
  }
  
  void     node::assign(std::tuple<int,int,int,int> params ){
    int  size = std::get<3>(params);
    int group = std::get<0>(params);
    int  item = std::get<1>(params);
    int  type = std::get<2>(params);
    //std::get<0>(params),std::get<1>(params), std::get<2>(params), std::get<3>(params)
    allocate(size);
    char *__a = const_cast<char*> (nodePointer);
    *reinterpret_cast<uint16_t *>(__a) = (uint16_t) group;
    *reinterpret_cast<uint8_t *>(__a+2) = (uint8_t) item;
    *reinterpret_cast<uint8_t *>(__a+3) = (uint8_t) type;
    *reinterpret_cast<uint32_t *>(__a+4) = 0x00000000;
    
  }
  void node::initEmpty(){
      char *__a = const_cast<char*> (nodePointer);
        *reinterpret_cast<uint16_t *>(__a) = (uint16_t) 0;
        *reinterpret_cast<uint8_t *>(__a+2) = (uint8_t) 0;
        *reinterpret_cast<uint8_t *>(__a+3) = (uint8_t) 0;
        *reinterpret_cast<uint8_t *>(__a+4) = 0x00000000;
    }

    void node::show(){
      printf("hipo::node : [%5d,%5d] type = %4d, node length = %5d, format length = %6d, data length = %8d, offset = %5d, capacity = %8d\n",
         group(),item(),type(), size(), formatLength(), dataLength(), dataOffset(), (int) capacity());
    }

//====================================================================
//-=-     END of hipo::node class - starting composite class        -=-
//====================================================================

}
