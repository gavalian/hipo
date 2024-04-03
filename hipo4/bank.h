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
 * File:   bank.h
 * Author: gavalian
 *
 * Created on April 12, 2017, 10:14 AM
 */

#ifndef HIPO_BANK_H
#define HIPO_BANK_H
#include <iostream>
#include <vector>
#include <cstring>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <map>
#include "dictionary.h"
#include "node.h"
#include "functional"

namespace hipo {

  class structure {

    private:
      
      std::vector<char> structureBuffer;
      char *structureAddress{};
      void setAddress(const char *address);

    protected:

      void initStructureBySize(int __group, int __item, int __type, int __size);
      std::vector<char>  &getStructureBuffer(){ return structureBuffer;}
      int                 getStructureBufferSize(){ return 8+getSize();}
      int                 dataOffset = 8;
      //std::vector<char> structureBuffer;
      friend class tuple;

  public:

      structure(){ structureAddress = nullptr;}
      structure(int size){ allocate(size);}
      structure(int __group, int __item, std::string &str);

      virtual     ~structure()= default;
      bool         allocate(int size);
      
      int          getSize() const noexcept{
        int length = *reinterpret_cast<uint32_t *>(structureAddress+4);
	       return length&0x00ffffff;
         //return getHeaderSize()+getDataSize();
      }

      int         getHeaderSize() const noexcept {
         int length = *reinterpret_cast<uint32_t *>(structureAddress+4);
         return (length>>24)&0x000000ff;
      }
      
      int         getDataSize() const noexcept {         
	       return getSize()-getHeaderSize();
      }

      int          getType();
      int          getGroup();
      int          getItem();
      void         init(const char *buffer, int size);
      void         initNoCopy(const char *buffer, int size);

      const char    *getAddress();
      virtual void   show();
      void           setSize(int size);
      void           setHeaderSize(int size);
      void           setDataSize(int size);

      int          getIntAt   ( int index) const noexcept {
        return *reinterpret_cast<int32_t*>(&structureAddress[index+dataOffset]);
      }
    
      int16_t      getShortAt ( int index) const noexcept {
        return *reinterpret_cast<int16_t*>(&structureAddress[index+dataOffset]);
      }
      int8_t       getByteAt  ( int index) const noexcept {
        return *reinterpret_cast<int8_t*>(&structureAddress[index+dataOffset]);
      }
      float        getFloatAt ( int index) const noexcept {
        return *reinterpret_cast<float*>(&structureAddress[index+dataOffset]);
      }
      double       getDoubleAt( int index) const noexcept {
        return *reinterpret_cast<double*>(&structureAddress[index+dataOffset]);
      }
      long         getLongAt  ( int index) const noexcept {
        return *reinterpret_cast<int64_t*>(&structureAddress[index+dataOffset]);
      }

      std::string  getStringAt(int index);

      void         putIntAt(int index, int value){
        *reinterpret_cast<int32_t*>(&structureAddress[index+dataOffset]) = value;
      }

      void         putShortAt(int index, int16_t value){
        *reinterpret_cast<int16_t*>(&structureAddress[index+dataOffset]) = value;
      }

      void         putByteAt(int index, int8_t value){
        *reinterpret_cast<int8_t*>(&structureAddress[index+dataOffset]) = value;
      }

      void         putFloatAt(int index, float value){
        *reinterpret_cast<float*>(&structureAddress[index+dataOffset]) = value;
      }

      void         putDoubleAt(int index, double value){
        *reinterpret_cast<double*>(&structureAddress[index+dataOffset]) = value;
      }

      void         putLongAt(int index, int64_t value){
        *reinterpret_cast<int64_t*>(&structureAddress[index+dataOffset]) = value;
      }

      void         putStringAt(int index, std::string &str);

      virtual void notify(){}
      friend class event;
  };

  class composite : public hipo::node {
    /**
     * @brief This is composite bank with type = 10, who knows why
     * Class is used to store formated data structures without dictionary.
     * Can be passed around in events and parsed on the other side.
     */
    private:

      std::vector<char>  typeChars;
      std::vector<int>   offsets;
      std::vector<int>   types;
      int                rowOffset = 0;

     // void parse(std::string format);
      int  getTypeSize(int type);

    public:

      composite(){};
      composite(int size){ allocate(size);};
      composite(int group, int item, int size){ /*initStructureBySize(group, item, 10, size);*/};
      composite(const char *format){}
      composite(int group, int item, const char *format, int capacity);
      
      void       parse(std::string format);
      void       parse(int group, int item, std::string format, int maxrows);
      virtual   ~composite(){}
      
      int      getRows() const noexcept { return dataLength()/rowOffset;}
      int      getEntries() const noexcept { return offsets.size();}
      int      getEntryType(int index) const noexcept { return types[index];}
      void     setRows(int rows);// { setDataLength(rows*rowOffset);}
      
      int      getRowSize() const noexcept { return rowOffset;}

      int      getInt    ( int element, int row) const noexcept;
      int64_t  getLong   ( int element, int row) const noexcept;
      float    getFloat  ( int element, int row) const noexcept;
      void     putInt    ( int element, int row, int value);
      void     putLong   ( int element, int row, int64_t value);
      void     putFloat  ( int element, int row, float value);
      virtual void notify();
      void     print();
      void     reset();
  };
  //typedef std::auto_ptr<hipo::generic_node> node_pointer;

class iterator {
    private:
      std::vector<int> rows;
      decltype(rows)::size_type current_index;
    public:
      iterator(){}
      virtual ~iterator(){}
      void    reset(){ rows.clear();}
      void    add(int index){rows.push_back(index);}
      void    begin(){current_index=0;}
      bool    next(){
        current_index++; if(current_index>rows.size()) { 
          current_index = rows.size(); return false;
        } return true;
      }
      bool   end(){return current_index>=rows.size();}
      int    index(){ return rows[current_index];}
      void   show(){ for(const auto& row : rows) printf("%5d ",row); printf("\n");}
};
    class bank : public hipo::structure {

    private:

      hipo::schema    bankSchema;
      hipo::iterator  bankIterator;
      int           bankRows{-1};

    protected:
        void setBankRows(int rows){ bankRows = rows;}

    public:

        bank();
        // constructor initializes the nodes in the bank
        // and they will be filled automatically by reader.next()
        // method.
        bank(const hipo::schema& __schema){
          bankSchema = __schema;
          bankRows   = -1;
        }

        bank(const hipo::schema& __schema, int __rows){
          bankSchema = __schema;
          bankRows   = __rows;
          int size   = bankSchema.getSizeForRows(__rows);
          initStructureBySize(bankSchema.getGroup(),bankSchema.getItem(), 11, size);
        }

        ~bank() override;
        // display the content of the bank
        //void show();

        hipo::schema  &getSchema() { return bankSchema;}
        hipo::iterator &getIterator(){ return bankIterator;}
        int    getRows()  const noexcept{ return bankRows;}
        void   setRows(   int rows);
        int    getInt(    int item, int index) const noexcept;
        int    getShort(  int item, int index) const noexcept;
        int    getByte(   int item, int index) const noexcept;
        float  getFloat(  int item, int index) const noexcept;
        double getDouble( int item, int index) const noexcept;
        long   getLong(   int item, int index) const noexcept;

        std::vector<int>    getInt(    int item) const noexcept;
        std::vector<float>  getFloat(  int item) const noexcept;
        std::vector<double> getDouble( int item) const noexcept;

        template<typename T = double> T get(int item, int index) const noexcept {
          auto type   = bankSchema.getEntryType(item);
          auto offset = bankSchema.getOffset(item, index, bankRows);
          switch(type) {
            case kByte:   return (int) getByteAt(offset);
            case kShort:  return (int) getShortAt(offset);
            case kInt:    return getIntAt(offset);
            case kFloat:  return getFloatAt(offset);
            case kDouble: return getDoubleAt(offset);
            case kLong:   return getLongAt(offset);
            default:
              printf("---> error(get) : unknown type for [%s] type = %d\n", bankSchema.getEntryName(item).c_str(), type);
          }
          return 0;
        }

        int    getInt(    const char *name, int index) const noexcept;
        
        int    getShort(  const char *name, int index) const noexcept;
        int    getByte(   const char *name, int index) const noexcept;
        float  getFloat(  const char *name, int index) const noexcept;
        double getDouble( const char *name, int index) const noexcept;
        long   getLong(   const char *name, int index) const noexcept;
        
        std::vector<int>    getInt(    const char *name) const noexcept;
        std::vector<float>  getFloat(  const char *name) const noexcept;
        std::vector<double> getDouble( const char *name) const noexcept;

        template<typename T = double> T get(const char *name, int index) const noexcept {
          return get<T>(bankSchema.getEntryOrder(name), index);
        }

        void    putInt(    const char *name, int index, int32_t value);
        void    putShort(  const char *name, int index, int16_t value);
        void    putByte(   const char *name, int index, int8_t value);
        void    putFloat(  const char *name, int index, float value);
        void    putDouble( const char *name, int index, double value);
        void    putLong(   const char *name, int index, int64_t value);
        template<typename T> void put(const char *name, int index, T value) {
          put(bankSchema.getEntryOrder(name), index, value);
        }

        void    putInt(int item, int index, int32_t value);
        void    putShort(int item, int index, int16_t value);
        void    putByte(int item, int index, int8_t value);
        void    putFloat(int item, int index, float value);
        void    putDouble(int item, int index, double value);
        void    putLong(int item, int index, int64_t value);
        template<typename T> void put(int item, int index, T value) {
          auto type = bankSchema.getEntryType(item);
          switch(type) {
            case kByte:   putByte(item,   index, static_cast<int8_t>(value));  break;
            case kShort:  putShort(item,  index, static_cast<int16_t>(value)); break;
            case kInt:    putInt(item,    index, static_cast<int32_t>(value)); break;
            case kFloat:  putFloat(item,  index, static_cast<float>(value));   break;
            case kDouble: putDouble(item, index, static_cast<double>(value));  break;
            case kLong:   putLong(item,   index, static_cast<int64_t>(value)); break;
            default:
              printf("---> error(put) : unknown type for [%s] type = %d\n", bankSchema.getEntryName(item).c_str(), type);
          }
        }
        void reduce(std::function<double(hipo::bank&, int)> func, bool doReset);
        void reduce(const char *expression, bool doReset);
        void    show() override;
        void    reset();
        //virtual  void notify(){ };

        void notify() override;

  };

/*
  class iterator {
    private:
      hipo::bank  &ib;
      std::vector<int> rows;
      decltype(rows)::size_type current_index;
    public:
      iterator();
      iterator(hipo::bank &b, std::vector<int> index) : ib(b) {
        ib = b; for(auto id : index) rows.push_back(id);
        current_index = 0;
      }
      iterator(bank &b) : ib(b) {ib=b; current_index = 0;}
      virtual ~iterator(){}

      
      void    reset(){ rows.clear();}
      void    add(int index){rows.push_back(index);}
      void    begin(){current_index=0;}
      bool    next(){
        current_index++; if(current_index>rows.size()) { 
          current_index = rows.size(); return false;
        }
        return true;
      }
      bool end(){return current_index>=rows.size();}
      int  index(){ return rows[current_index];}
      void show(){ for(const auto& row : rows) printf("%5d ",row); printf("\n");}
      
      static hipo::iterator link(hipo::bank &b, int row, int column);
      static hipo::iterator reduce(std::function<double(hipo::bank&, int)> func, hipo::bank& bank);
      static hipo::iterator reduce(hipo::bank &bank, const char *expression);
  };
  */
    /////////////////////////////////////
    //inlined getters

    inline float  bank::getFloat(int item, int index) const noexcept{
      if(bankSchema.getEntryType(item)==kFloat){
        int offset = bankSchema.getOffset(item, index, bankRows);
        return getFloatAt(offset);
      }
      return 0.0;
    }

    inline double  bank::getDouble(int item, int index) const noexcept{
      if(bankSchema.getEntryType(item)==kDouble){
        int offset = bankSchema.getOffset(item, index, bankRows);
        return getDoubleAt(offset);
      }
      if(bankSchema.getEntryType(item)==kFloat){
        int offset = bankSchema.getOffset(item, index, bankRows);
        return getFloatAt(offset);
      }
      return 0.0;
    }

    inline long bank::getLong(int item, int index) const noexcept{
      if(bankSchema.getEntryType(item)==kLong){
        int offset = bankSchema.getOffset(item, index, bankRows);
        return getLongAt(offset);
      }
      return 0;
    }

    inline int    bank::getInt(int item, int index) const noexcept{
      int type = bankSchema.getEntryType(item);
      int offset = bankSchema.getOffset(item, index, bankRows);
      switch(type){
      case kByte:  return (int) getByteAt(offset);
      case kShort: return (int) getShortAt(offset);
      case kInt:   return getIntAt(offset);
      default: printf("---> error : requested INT for [%s] type = %d\n",
		      bankSchema.getEntryName(item).c_str(),type); break;
      }
      return 0;
    }

  inline std::vector<int>    bank::getInt(int item) const noexcept{
      int type = bankSchema.getEntryType(item);
      
      std::vector<int> row;
      int nrows = getRows();

      for(int j = 0; j < nrows; j++){
        int offset = bankSchema.getOffset(item, j, bankRows);
      switch(type){
          case kByte:  row.push_back((int) getByteAt(offset)); break;
          case kShort: row.push_back((int) getShortAt(offset)); break; 
          case kInt:   row.push_back((int) getIntAt(offset)); break;
      default: printf("---> error : requested INT for [%s] type = %d\n",
		      bankSchema.getEntryName(item).c_str(),type); break;
      }
      }
      return row;
    }

    inline int    bank::getShort(int item, int index) const noexcept{
      int type = bankSchema.getEntryType(item);
      int offset = bankSchema.getOffset(item, index, bankRows);
      switch(type){
      case kByte:  return (int) getByteAt(offset);
      case kShort: return (int) getShortAt(offset);
      default: printf("---> error : requested SHORT for [%s] type = %d\n",
		      bankSchema.getEntryName(item).c_str(),type); break;
      }
      return 0;
    }

    inline int    bank::getByte(int item, int index) const noexcept{
      int type = bankSchema.getEntryType(item);
      int offset = bankSchema.getOffset(item, index, bankRows);
      switch(type){
      case kByte: return (int) getByteAt(offset);
      default: printf("---> error : requested BYTE for [%s] type = %d\n",
		      bankSchema.getEntryName(item).c_str(),type); break;
      }
      return 0;
    }
    inline int    bank::getInt(const char *name, int index) const noexcept{
      int item = bankSchema.getEntryOrder(name);
      int type = bankSchema.getEntryType(item);
      int offset = bankSchema.getOffset(item, index, bankRows);
      switch(type){
      case kByte:  return (int) getByteAt(offset);
      case kShort: return (int) getShortAt(offset);
      case kInt:   return getIntAt(offset);
      default: printf("---> error : requested INT for [%s] type = %d\n",name,type); break;
      }
      return 0;
    }

    inline std::vector<int>    bank::getInt(const char *name) const noexcept{
      int item = bankSchema.getEntryOrder(name);
      int type = bankSchema.getEntryType(item);
      std::vector<int> row;

      int nrows = getRows();
      for(int j = 0; j < nrows; j++){
          int offset = bankSchema.getOffset(item, j, bankRows);
         switch(type){
             case kByte:  row.push_back((int) getByteAt(offset)); break;
             case kShort: row.push_back((int) getShortAt(offset)); break; 
             case kInt:   row.push_back((int) getIntAt(offset)); break;
             default: printf("---> error : requested INT for [%s] type = %d\n",name,type); break;
         }
      }
      return row;
    }

    inline int    bank::getShort(const char *name, int index) const noexcept{
      int item = bankSchema.getEntryOrder(name);
      int type = bankSchema.getEntryType(item);
      int offset = bankSchema.getOffset(item, index, bankRows);
      switch(type){
      case kByte:  return (int) getByteAt(offset);
      case kShort: return (int) getShortAt(offset);
      default: printf("---> error : requested SHORT for [%s] type = %d\n",
		      bankSchema.getEntryName(item).c_str(),type); break;
      }
      return 0;
    }
    inline int    bank::getByte(const char *name, int index) const noexcept{
      int item = bankSchema.getEntryOrder(name);
      int type = bankSchema.getEntryType(item);
      int offset = bankSchema.getOffset(item, index, bankRows);
      switch(type){
      case kByte: return (int) getByteAt(offset);
      default: printf("---> error : requested BYTE for [%s] type = %d\n",
		      bankSchema.getEntryName(item).c_str(),type); break;
      }
      return 0;
    }

    inline float  bank::getFloat(const char *name, int index) const noexcept{
      int item = bankSchema.getEntryOrder(name);
      if(bankSchema.getEntryType(item)==kFloat){
        int offset = bankSchema.getOffset(item, index, bankRows);
        return getFloatAt(offset);
      }
      return 0.0;
    }

  inline std::vector<float>  bank::getFloat(const char *name) const noexcept{
      int item = bankSchema.getEntryOrder(name);
      std::vector<float> row;
      int nrows = getRows();
      if(bankSchema.getEntryType(item)==kFloat){
        for(int j = 0; j < nrows; j++){
          int offset = bankSchema.getOffset(item, j, bankRows);
          row.push_back( getFloatAt(offset));
        }
      }
      return row;
    }
    
    inline std::vector<float>  bank::getFloat(int item) const noexcept{
      std::vector<float> row;
      int nrows = getRows();
      if(bankSchema.getEntryType(item)==kFloat){
        for(int j = 0; j < nrows; j++){
          int offset = bankSchema.getOffset(item, j, bankRows);
          row.push_back( getFloatAt(offset));
        }
      }
      return row;
    }
    inline double  bank::getDouble(const char *name, int index) const noexcept{
      int item = bankSchema.getEntryOrder(name);
      if(bankSchema.getEntryType(item)==kDouble){
        int offset = bankSchema.getOffset(item, index, bankRows);
        return getDoubleAt(offset);
      }
      if(bankSchema.getEntryType(item)==kFloat){
        int offset = bankSchema.getOffset(item, index, bankRows);
        return (double) getFloatAt(offset);
      }
      return 0.0;
    }

    inline std::vector<double>  bank::getDouble(int item) const noexcept{
      std::vector<double> row;
      int nrows = getRows();      

      if(bankSchema.getEntryType(item)==kDouble){
        for(int j = 0; j < nrows; j++){
          int offset = bankSchema.getOffset(item, j, bankRows);
          row.push_back(getDoubleAt(offset));
        }
      }
      if(bankSchema.getEntryType(item)==kFloat){
        for(int j = 0; j < nrows; j++){
          int offset = bankSchema.getOffset(item, j, bankRows);
          row.push_back((double) getFloatAt(offset));
        }
      }
      return row;
    }

    inline std::vector<double>  bank::getDouble(const char *name) const noexcept{
      std::vector<double> row;
      int nrows = getRows();
      int item = bankSchema.getEntryOrder(name);

      if(bankSchema.getEntryType(item)==kDouble){
        for(int j = 0; j < nrows; j++){
          int offset = bankSchema.getOffset(item, j, bankRows);
          row.push_back(getDoubleAt(offset));
        }
      }
      if(bankSchema.getEntryType(item)==kFloat){
        for(int j = 0; j < nrows; j++){
          int offset = bankSchema.getOffset(item, j, bankRows);
          row.push_back((double) getFloatAt(offset));
        }
      }
      return row;
    }
    
    inline long bank::getLong(const char *name, int index) const noexcept{
      int item = bankSchema.getEntryOrder(name);
      if(bankSchema.getEntryType(item)==kLong){
        int offset = bankSchema.getOffset(item, index, bankRows);
        return getLongAt(offset);
      }
      return 0;
    }
  inline void    bank::putInt(int item, int index, int32_t value){
    //int type = bankSchema.getEntryType(item);
      int offset = bankSchema.getOffset(item, index, bankRows);
      putIntAt(offset,value);
    }
    inline void    bank::putShort(int item, int index, int16_t value){
      //int type = bankSchema.getEntryType(item);
      int offset = bankSchema.getOffset(item, index, bankRows);
      putShortAt(offset,value);
    }
    inline void    bank::putByte(int item, int index, int8_t value){
      //int type = bankSchema.getEntryType(item);
      int offset = bankSchema.getOffset(item, index, bankRows);
      putByteAt(offset,value);
    }
    inline  void    bank::putFloat(int item, int index, float value){
      //int type = bankSchema.getEntryType(item);
      int offset = bankSchema.getOffset(item, index, bankRows);
      //printf("---- put float %f at position = %d\n",value,offset);
      putFloatAt(offset,value);
    }
    inline void    bank::putDouble(int item, int index, double value){
      //int type = bankSchema.getEntryType(item);
      int offset = bankSchema.getOffset(item, index, bankRows);
      putDoubleAt(offset,value);
    }
    inline void    bank::putLong(int item, int index, int64_t value){
      //int type = bankSchema.getEntryType(item);
      int offset = bankSchema.getOffset(item, index, bankRows);
      putLongAt(offset,value);
    }

    using banklist=std::vector<bank>;
}
#endif /* BANK_H */
