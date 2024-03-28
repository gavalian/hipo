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

      int          getType() const;
      int          getGroup() const;
      int          getItem() const;
      void         init(const char *buffer, int size);
      void         initNoCopy(const char *buffer, int size);

      const char    *getAddress();
      virtual void   show() const;
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
      
      /// @returns the number of bank rows. This is the number of **all** of the rows, _i.e._, not
      ///          the reduced number if `hipo::bank::rowlist::reduce` was called; for the latter, use
      ///          `hipo::bank::getRowList().size()`.
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

    class bank : public hipo::structure {

    public:

      class rowlist {

      public:
        using list_t = std::vector<int>;

        /// constructor
        /// @param ownerBank if set, associate this `rowlist` with bank `ownerBank`
        rowlist(bank* const ownerBank = nullptr) : m_list({}), m_init(false), m_ownerBank(ownerBank) {}
        ~rowlist() {}

        /// initialize with a full list with specified number of rows
        /// @param numRows if negative, use the owner `bank` to set the number of rows, otherwise use `numRows`
        void reset(int numRows = -1);
        bool const isInitialized() const { return m_init; }

        /// @returns reference to the immutable list
        list_t const& getList() const;
        /// @param list set the list to this list
        void setList(list_t list);

        /// filter the list according to a function
        /// @param func a function which takes a `hipo::bank` reference and an `int` row number and returns a `double`;
        /// if the returned `double` is larger than 0.5, the row is accepted
        void reduce(std::function<double(hipo::bank&, int)> func);
        /// filter the list according to an expression
        /// @param expression the filter expression
        void reduce(char const* expression);

      private:
        bool m_init;
        list_t m_list;
        bank* const m_owner_bank;

        static list_t generate_number_list(list_t::size_type num = 500);
        static list_t copy_number_list(list_t::size_type num);
        static list_t s_number_list;

        bool ownerBankIsUnknown(std::string_view caller = "");

      };

    private:

      hipo::schema bankSchema;
      rowlist      bankRowList;
      int          bankRows{-1};

    public:

        bank() : bankRowList(rowlist(this)) = default;
        // constructor initializes the nodes in the bank
        // and they will be filled automatically by reader.next()
        // method.
        bank(const hipo::schema& __schema) : bankSchema(__schema), bankRowList(rowlist(this)) {}

        bank(const hipo::schema& __schema, int __rows) : bankSchema(__schema), bankRows(__rows), bankRowList(rowlist(this)) {
          int size = bankSchema.getSizeForRows(bankRows);
          initStructureBySize(bankSchema.getGroup(),bankSchema.getItem(), 11, size);
          bankRowList.reset();
        }

        ~bank() = default;

        hipo::schema  &getSchema() { return bankSchema;}
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

        /// @returns an immutable list of available rows for this bank. This list may be a subset of the full
        /// list of rows, if for example the bank was filtered (see `hipo::bank::rowlist::reduce`)
        rowlist::list_t const& getRowList() const { return bankRowList.getList(); }

        /// @returns a reference to the mutable `hipo::bank::rowlist` owned by this bank. For example, use this method to
        /// call `hipo::bank::rowlist::reduce`.
        rowlist& getMutableRowList() { return bankRowList; }

        /// @returns a `hipo::bank::rowlist` for this bank, for rows `r` such that `getInt(column,r) == row`
        /// @param row the value to check
        /// @param column the column to check (must be an integer-type column, _e.g._, that of `"pindex"`)
        rowlist::list_t const getRowListLinked(int const row, int const column) const;

        /// show this bank's contents; only the rows in its current `rowlist` instance are shown
        void show() const override { show(false); }

        /// show this bank's contents
        /// @param showAllRows if `true`, show **all** this bank's rows, otherwise just the rows in its `rowlist` instance,
        /// which may have been reduced
        void show(bool const showAllRows) const;

        /// print a stored value
        /// @param schemaEntry the schema entry number
        /// @param row the row number
        void printValue(int schemaEntry, int row) const;

        void reset();

        void notify() override;

  };

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
