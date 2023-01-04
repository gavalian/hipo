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
 * File:   table.cc
 * Author: gavalian
 *
 * Created on April 12, 2017, 10:14 AM
 */


#include "table.h"
#include <fstream>

namespace coda {
    /** 
     * Encode four integer numbers into one LONG value
    */
    long   table::encode(int id1, int id2, int id3, int id4){
        long word = 0;
        word = word|( (((long) id1)<<48)&0xffff000000000000);
        word = word|( (((long) id2)<<32)&0x0000ffff00000000);
        word = word|( (((long) id3)<<8)&0x00000000ffffff00);
        word = word|( (    ((long) id4))&0x00000000000000ff);
        return word;
    }
    /**
     * @brief insert a translation table entry into the translation map
     * @param desc combination of crate/clot/chanel => sector/layer/component/order
     */
    void table::insert(descriptor_t &desc){
         long key   = encode(desc.crate,desc.slot,desc.channel,0);
         long value = encode(desc.sector, desc.layer, desc.component, desc.order);
         translation[key] = value;
    }
    /**
     * @brief check if the translation map contains entry for crate/slot/channel
     * 
     * @param desc crate/slot/channel passed through structure
     * @return true if map contains entry key for given crate/slot/channel
     * @return false otherwise
     */
    bool  table::contains(descriptor_t &desc){
        long key = encode(desc.crate,desc.slot,desc.channel,0);
        return translation.count(key)!=0;
    }
    /**
     * @brief decodes the crate/slot/channel to sector/layer/component/order
     * and overrides the values of the passed structure.
     * 
     * @param desc structure with crate/slot/channel and corresponding sector/layer/component/order
     */
    void   table::decode(descriptor_t &desc){
        long key = encode(desc.crate,desc.slot,desc.channel,0);
        long value = translation[key];
        desc.sector = ((int) (value>>48))&0x0000ffff;
        desc.layer = ((int) (value>>32))&0x0000ffff;
        desc.component = ((int) (value>>8))&0x00ffffff;
        desc.order = ((int) value)&0x000000ff;
    }
    /**
     * @brief read a text file with 7 integers and create a translation map 
     * with key = crate/slot/channel and value = sector/layer/component/order
     * example lines from text file :
     *   41   3   26   1   6    5    0           
     *   41   3   27   1   1    5    0
     * first 3 - hardware address, last 4 software address 
     * @param filename text file name to read.
     */
    void table::read(const char *filename){
        std::ifstream tt (filename);
        descriptor_t desc;
        int counter = 0;
         while (!tt.eof()){
            tt >> desc.crate  >> desc.slot  >> desc.channel 
               >> desc.sector >> desc.layer >> desc.component 
               >> desc.order;
            insert(desc);
            counter++;
         }
         printf("[table] file >> %s, loaded rows = %d, map size = %lu\n",filename, counter, translation.size());
    }
    /**
     * @brief prints the desciptor on the screen in one line for debugging purposes
     * 
     * @param desc data descriptos with hardware and software address
     */
    void   table::print(descriptor_t &desc){
        printf("c/s/c [%4d,%4d,%5d] , s/l/c/o [%4d,%4d,%5d,%4d]\n",
              desc.crate , desc.slot,  desc.channel,
              desc.sector, desc.layer, desc.component, desc.order);
    }
}
