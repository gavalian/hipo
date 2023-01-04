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
 * File:   decoder.cc
 * Author: gavalian
 *
 * Created on April 12, 2017, 10:14 AM
 */


#include "decoder.h"
#include <fstream>

namespace coda {

   void decoder::decode(int crate, const char *buffer, int offset, int length, coda::table &tbl, hipo::composite &bank){
       int   pos = offset;
       bool  doLoop = true;
       int   row = bank.getRows();
       descriptor_t desc;
       desc.crate = crate;
       //printf(">>>>>>>> decoding\n");
       while(doLoop==true){
          uint8_t        slot = *reinterpret_cast<const uint8_t*>( &buffer[pos]);
          //uint32_t    tnumber = *reinterpret_cast<const uint32_t*>(&buffer[pos + 1]);
          uint64_t  timestamp = *reinterpret_cast<const uint64_t*>(&buffer[pos + 5]);
          uint32_t    nrepeat = *reinterpret_cast<const uint32_t*>(&buffer[pos + 13]);
          pos += 17;
          //printf("--- n-repeat : %d\n", nrepeat);
         if(nrepeat>1000) break;
          
          //printf  ("\n >>>> slot = %d, N = %d\n",slot, nrepeat);
          for(int n =  0; n < nrepeat; n++){
             uint8_t  channel =  *reinterpret_cast<const uint8_t*>( &buffer[pos]);
             uint16_t     tdc =  *reinterpret_cast<const uint16_t*>( &buffer[pos+1]);
             pos += 3;
             desc.slot = slot;
             desc.channel = channel;
             //printf("\t\t channel = %d tdc = %d\n",channel,tdc);
             if(tbl.contains(desc)==true){
                tbl.decode(desc);
                bank.putInt(row,0,desc.sector);
                bank.putInt(row,1,desc.layer);
                bank.putInt(row,2,desc.component);
                bank.putInt(row,3,desc.order);
                bank.putInt(row,4,tdc);
                bank.putLong(row,5,timestamp);
                row++;
             } else {
                printf(" error in decoder : "); tbl.print(desc);
             }             
           } 
          if((pos+17 - offset ) < length) doLoop = false;
          
          //if((pos+17 - offset ) < length) doLoop = false;
       }
   }
}
