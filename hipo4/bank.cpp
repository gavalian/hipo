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

#include "bank.h"
#include "parser.h"

namespace hipo {

  //==============================================================
  // Definition of class structure, this will class will be extended
  // to represent different objects that will be appended to the event
  //==============================================================

  structure::structure(int __group, int __item, std::string &str){
    int length = str.length();
    initStructureBySize(__group,__item, 6, length);
    putStringAt(0,str);
  }

  bool structure::allocate(int size){
      if(structureBuffer.size()<size){
        structureBuffer.resize(size+32);
      }
      return true;
  }

  void structure::initStructureBySize(int __group, int __item, int __type, int __size){
      allocate(__size+8);
      structureAddress = &structureBuffer[0];
      *reinterpret_cast<uint16_t *>(structureAddress) = (uint16_t) __group;
      *reinterpret_cast<uint8_t *>(&structureAddress[2]) = (uint8_t) __item;
      *reinterpret_cast<uint8_t *>(&structureAddress[3]) = (uint8_t) __type;
      *reinterpret_cast<uint32_t *>(&structureAddress[4]) = __size;
  }

    void structure::setSize(int size){
      int sizeWord =  *reinterpret_cast<uint32_t *>(structureAddress+4);
      int header   =  sizeWord&0xff000000;
      *reinterpret_cast<uint32_t *>(structureAddress+4) = header|(size&0x00ffffff);
    }

   void  structure::setDataSize(int size){
      int sizeWord  =  *reinterpret_cast<uint32_t *>(structureAddress+4);
      int header    =  sizeWord&0xff000000;
      int headerSize = (sizeWord>>24)&0x000000ff;
      int totalSize = headerSize + size;
      //printf(" header size = %d, size = %d, total size = %d\n", (sizeWord>>24)&0x000000ff,size, totalSize);
      *reinterpret_cast<uint32_t *>(structureAddress+4) = header|(totalSize&0x00ffffff);
    }

    void structure::setHeaderSize(int size){ 
      int sizeWord =  *reinterpret_cast<uint32_t *>(structureAddress+4);
      int dataSize = sizeWord&0x00ffffff;
      *reinterpret_cast<uint32_t *>(structureAddress+4) = ((size<<24)&0xff000000)|dataSize ;
    }

    // return the type of the structure
    int structure::getType() const {
      auto type = (int) (*reinterpret_cast<uint8_t *>(structureAddress+3));
      return type;
    }
    // returns the group number of the object
    int structure::getGroup() const {
      auto group = (int) (*reinterpret_cast<uint16_t *>(structureAddress));
      return group;
    }
    // returns the item number of the structure
    int structure::getItem() const {
      auto item = (int) (*reinterpret_cast<uint8_t *>(structureAddress+2));
      return item;
    }
    void         structure::initNoCopy(const char *buffer, int size){
        structureAddress = const_cast<char*>(buffer);
    }

    void structure::init(const char *buffer, int size){
      allocate(size);
      memcpy(&structureBuffer[0],buffer,size);
      structureAddress = &structureBuffer[0];
    }

    void structure::show() const {
      printf("structure : [%5d,%5d] type = %4d, header = %5d, length = %6d, data size = %5d, offset = %5d, capacity = %5lu\n",
         getGroup(),getItem(),getType(),getHeaderSize(), getSize(), getDataSize(), dataOffset, structureBuffer.size());
    }

    std::string  structure::getStringAt(int index){
        int length = getSize();
        auto *string_ch = (char *) malloc(length+1);
        std::memcpy(string_ch, &structureBuffer[8],length);
        string_ch[length] = '\0';
        std::string result = string_ch;
        free(string_ch);
        return result;
    }

    void         structure::putStringAt(int index, std::string &str){
      int strLen = str.length();
      std::memcpy(&structureBuffer[8],&str[0],strLen);
    }

    const char *structure::getAddress(){
      return structureAddress;
    }

//====================================================================
//-=-     END of structure class - starting composite class        -=-
//====================================================================

void     composite::reset(){
    //int length = getHeaderSize();
    //setHeaderSize(length);
    //setSize(length);
    setDataLength(0);
}

composite::composite(int group, int item, const char *format, int capacity){
  parse(group,item,format,capacity);
  //create(group,item,10,capacity);  
}

void  composite::parse(std::string format){
  parse(134,1,format,256);
}

void     composite::setRows(int rows) {
  if(((formatLength()+8)+getRowSize()*(rows+1))<capacity()){
    printf("composite::setRows:: error, the requested row %d exceeds the bank capacity of %d\n",
	   rows,capacity());
  } else {
    setDataLength(rows*rowOffset);
  }
}
  
void  composite::parse(int group, int item, std::string format, int maxrows){
    types.clear(); offsets.clear();
    int length = format.length();
    int offset = 0;
    for(int i = 0; i < length; i++){
       char c = format[i];
       //printf("%5d : %c\n",i,c);

       switch(c){
        case 'b': types.push_back(1); offsets.push_back(offset); offset += getTypeSize(1); break;
        case 's': types.push_back(2); offsets.push_back(offset); offset += getTypeSize(2); break;
        case 'i': types.push_back(3); offsets.push_back(offset); offset += getTypeSize(3); break;
        case 'f': types.push_back(4); offsets.push_back(offset); offset += getTypeSize(4); break;
        case 'd': types.push_back(5); offsets.push_back(offset); offset += getTypeSize(5); break;
        case 'l': types.push_back(8); offsets.push_back(offset); offset += getTypeSize(8); break;
        default: break;
       }
    }
    rowOffset = offset;
    create(group,item,10,rowOffset*maxrows + 8 + length);
    setFormatLength(length);
    memcpy(const_cast<char *>(&pointer()[8]),&format[0],length);
    //initStructureBySize(group,item,10,rowOffset*maxrows + 8 + length);
    //setHeaderSize(length);
    //setSize(length);
    //dataOffset = 8 + length;
    //memcpy(&getStructureBuffer()[8],&format[0],length);
}

int   composite::getTypeSize(int type){
    switch(type){
      case 1: return  1;
      case 2: return  2;
      case 3: return  4;
      case 4: return  4;
      case 5: return  8;
      case 8: return  8;
      default: return 0;
    }
}

int      composite::getInt    ( int element, int row) const noexcept{
   int type = types[element];
   int rows = getRows();
   if(row>=rows) {
     printf(" error : requested row %d out of %d\n", row, rows);
     return -1;
   }
   int offset = getRowSize()*row + offsets[element];
   switch(type){
     case 1: return getByteAt(offset); 
     case 2: return getShortAt(offset); 
     case 3: return getIntAt(offset);
     default: printf(" error : type = %d\n",type); break;
   }
   return -1;
}

int64_t     composite::getLong    ( int element ,int row) const noexcept{
   int type = types[element];
   int rows = getRows();
   if(row>=rows) {
     printf(" error : requested row %d out of %d\n", row, rows);
     return -1;
   }
   int offset = getRowSize()*row + offsets[element];
   return getLongAt(offset);
}
  
float    composite::getFloat  ( int element, int row) const noexcept { 
  int type = types[element];
   int rows = getRows();
   if(row>=rows) {
     printf(" error : requested row %d out of %d\n", row, rows);
     return 0.0;
   }
   int offset = getRowSize()*row + offsets[element];
   return getFloatAt(offset);
}

void     composite::putInt    ( int element, int row, int value){
   int type = types[element];
   int rows = getRows();
   if(row>=rows) setRows(row+1);
   int offset = getRowSize()*row + offsets[element];
   //printf("[putInt] offset = %d\n",offset);
   switch(type){
     case 1: putByteAt(offset, (uint8_t) value); break;
     case 2: putShortAt(offset, (uint16_t) value); break;
     case 3: putIntAt(offset,  value); /*printf(" after put = %d\n",getIntAt(offset));*/ break;
     default: printf("[putInt] error : type = %d\n",type); break;
   }
}

void     composite::putLong    ( int element, int row, int64_t value){
   int type = types[element];
   int rows = getRows();
   if(row>=rows) setRows(row+1);
   int offset = getRowSize()*row + offsets[element];
   putLongAt(offset,  value);
}

  
void     composite::putFloat  ( int element, int row, float value){
   int type = types[element];
   int rows = getRows();
   if(row>=rows) setRows(row+1);
   int offset = getRowSize()*row  + offsets[element];
   if(type==4) putFloatAt(offset, value); 
   else printf("[putFloat] error : type = %d\n",type);
}

void composite::notify(){
    //printf("-----> composite::notify method is called:\n");
    types.clear(); offsets.clear();
    char *destination = const_cast<char*> (&pointer()[4]);
    int sword  =  *reinterpret_cast<int*>(destination);
    //printf("sword = %X ---- %d\n",sword,sword);
    int fsize  = (sword>>24)&(0x000000FF);
    int dsize  = (sword)&0x00FFFFFF;
    //printf("-----> composite::notify method is called: fsize = %d, dsize = %d\n",fsize,dsize);
    int offset = 0;
    for(int i = 0; i < fsize; i++){
       char c = pointer()[8+i];
       //printf("charachter at %d = %c\n",i,c);
       switch(c){
        case 'b': types.push_back(1); offsets.push_back(offset); offset += getTypeSize(1); break;
        case 's': types.push_back(2); offsets.push_back(offset); offset += getTypeSize(2); break;
        case 'i': types.push_back(3); offsets.push_back(offset); offset += getTypeSize(3); break;
        case 'f': types.push_back(4); offsets.push_back(offset); offset += getTypeSize(4); break;
        case 'd': types.push_back(5); offsets.push_back(offset); offset += getTypeSize(5); break;
        case 'l': types.push_back(8); offsets.push_back(offset); offset += getTypeSize(8); break;
        default: break;
       }
    }
    rowOffset  = offset;
    //dataOffset = 8 + fsize;
}

void   composite::print(){
  printf("\n------------- \n");
  printf("[composite] identifiers : [%5d, %5d]\n",group(),item());
  int headerSize = formatLength();
  printf("[composite] format      : [");
  for(int i = 0; i < headerSize; i++) printf("%c",pointer()[8+i]); 
  printf("], row size = %5d , nrows = %5d\n",rowOffset, getRows());
  printf("[composite] entry       : ");
  for(int k = 0; k < offsets.size(); k++) printf("%5d ",k); printf("\n");
  printf("[composite] types       : ");
  for(int k = 0; k < offsets.size(); k++) printf("%5d ",types[k]);
  printf("\n");printf("[composite] offsets     : ");
  for(int k = 0; k < offsets.size(); k++) printf("%5d ",offsets[k]);
  printf("\n------------\n");

  int nEntries = getEntries();
  int nRows  = getRows();  
  for(int e = 0; e < nEntries; e++){
    printf("%5d : ", e);
    for(int r = 0 ; r < nRows; r++){
         int type = getEntryType(e);
         if(type==1||type==2||type==3) printf("%8d ",getInt(e,r));
         if(type==4) printf("%8.5f ",getFloat(e,r));
         if(type==8) printf("%lld ",getLong(e,r));
    }
    printf("\n");
  }
}


//////////////////////////////////////////////////////////////////////////////////
// hipo::bank::rowlist
//////////////////////////////////////////////////////////////////////////////////

void bank::rowlist::reset(int numRows) {
  m_list.clear();
  m_init = false;
  if(numRows >= 0)
    m_list = createFullList(numRows);
  else {
    if(ownerBankIsUnknown("reset"))
      return;
    m_list = createFullList(m_owner_bank->getRows());
  }
  m_init = true;
}

bank::rowlist::list_t const& bank::rowlist::getList() const {
  if(!m_init)
    std::cerr << "WARNING: attempted to get an uninitialized bank row list" << std::endl;
  return m_list;
}

void bank::rowlist::setList(list_t const& list) {
  m_list = list;
  m_init = true;
}

void bank::rowlist::filter(std::function<bool(bank&, int)> func) {
  if(ownerBankIsUnknown("filter"))
    return;
  auto indx = m_list;
  m_list.clear();
  for(auto const& r : indx)
    if(func(*m_owner_bank, r) == true)
      m_list.push_back(r);
}

void bank::rowlist::filter(const char *expression) {
  if(ownerBankIsUnknown("filter"))
    return;
  Parser p(expression); // FIXME: move `Parser p` creation upstream to separate function
  int nitems     = m_owner_bank->getSchema().getEntries();
  schema &schema = m_owner_bank->getSchema();
  auto indx      = m_list;
  m_list.clear();
  for(auto const& r : indx){
    for(int i = 0; i < nitems; i++)
      p[schema.getEntryName(i)] = m_owner_bank->get(i,r);
    if(p.Evaluate() > 0.5)
      m_list.push_back(r);
  }
}

bank::rowlist::list_t bank::rowlist::createFullList(int num) {
  if(num < 0) {
    std::cerr << "ERROR: attempted to call rowlist::getFullRowList with a negative size" << std::endl;
    return {};
  }
  if(num <= s_number_list.size())
    return list_t(s_number_list.begin(), s_number_list.begin() + num); // FIXME: benchmark this vs. `memcpy`
  else {
    auto result = s_number_list;
    for(list_t::size_type i = s_number_list.size(); i < num; i++)
      result.push_back(i);
    return result;
  }
}

bool bank::rowlist::ownerBankIsUnknown(std::string_view caller) {
  if(m_owner_bank == nullptr) {
    std::cerr <<
      "ERROR: attempted to call hipo::bank::rowlist " <<
      (caller=="" ? "method" : caller) <<
      ", but no bank is associated to this rowlist" <<
      std::endl;
    return true;
  }
  return false;
}

bank::rowlist::list_t bank::rowlist::s_number_list_init(list_t::size_type num) {
  list_t result;
  for(list_t::size_type i = 0; i < num; i++)
    result.push_back(i);
  return result;
}

bank::rowlist::list_t bank::rowlist::s_number_list = bank::rowlist::s_number_list_init();


//////////////////////////////////////////////////////////////////////////////////
// hipo::bank
//////////////////////////////////////////////////////////////////////////////////

bank::bank()= default;

bank::~bank()= default;

void    bank::setRows(int rows){
   bankRows = rows;
   int size = bankSchema.getSizeForRows(bankRows);
   initStructureBySize(bankSchema.getGroup(),bankSchema.getItem(), 11, size);
   bankRowList.reset(bankRows);
   //allocate(size+12);
}

void bank::reset(){
   setSize(0);
   bankRows = 0;
   bankRowList.reset(bankRows);
}

void bank::notify(){
  int size = bankSchema.getRowLength();
  bankRows = getSize()/size;
  bankRowList.reset(bankRows);
  //printf("---> bank notify called structure size = %8d (size = %5d)  rows = %d\n",
  //    getSize(),size, bankRows);
}


void    bank::putInt(const char *name, int index, int32_t value){
  int item = bankSchema.getEntryOrder(name);
  int type = bankSchema.getEntryType(item);
  int offset = bankSchema.getOffset(item, index, bankRows);
  putIntAt(offset,value);
}
void    bank::putShort(const char *name, int index, int16_t value){
  int item = bankSchema.getEntryOrder(name);
  int type = bankSchema.getEntryType(item);
  int offset = bankSchema.getOffset(item, index, bankRows);
  putShortAt(offset,value);
}
void    bank::putByte(const char *name, int index, int8_t value){
  int item = bankSchema.getEntryOrder(name);
  int type = bankSchema.getEntryType(item);
  int offset = bankSchema.getOffset(item, index, bankRows);
  putByteAt(offset,value);
}
void    bank::putFloat(const char *name, int index, float value){
  int item = bankSchema.getEntryOrder(name);
  int type = bankSchema.getEntryType(item);
  int offset = bankSchema.getOffset(item, index, bankRows);
  //printf("---- put float %f at position = %d\n",value,offset);
  putFloatAt(offset,value);
}
void    bank::putDouble(const char *name, int index, double value){
  int item = bankSchema.getEntryOrder(name);
  int type = bankSchema.getEntryType(item);
  int offset = bankSchema.getOffset(item, index, bankRows);
  putDoubleAt(offset,value);
}

void    bank::putLong(const char *name, int index, int64_t value){
  int item = bankSchema.getEntryOrder(name);
  int type = bankSchema.getEntryType(item);
  int offset = bankSchema.getOffset(item, index, bankRows);
  putLongAt(offset,value);
}

bank::rowlist::list_t const& bank::getRowList() const {
  return bankRowList.getList();
}

bank::rowlist::list_t const bank::getFullRowList() const {
  return bankRowList.createFullList(getRows());
}

bank::rowlist& bank::getMutableRowList() {
  bankRowList.setOwnerBank(this);
  return bankRowList;
}

bank::rowlist::list_t const bank::getRowListLinked(int const row, int const column) const {
  rowlist::list_t linked_rows;
  for(auto const& r : getRowList()) {
    if(getInt(column,r)==row)
      linked_rows.push_back(r);
  }
  return linked_rows;
}

void bank::show() const {
  show(false);
}

void bank::show(bool const showAllRows) const {

  bool loopAllRows = showAllRows || ! bankRowList.isInitialized() || getRowList().size() == getRows();
  if(loopAllRows)
    printf("BANK :: NAME %24s , ROWS %6d\n", bankSchema.getName().c_str(), getRows());
  else
    printf("BANK :: NAME %24s , ROWS %6ld (FILTERED FROM %d TOTAL)\n", bankSchema.getName().c_str(), getRowList().size(),getRows());

  for(int i = 0; i < bankSchema.getEntries(); i++){
    printf("%18s : ", bankSchema.getEntryName(i).c_str());
    if(loopAllRows)
      for(int k = 0; k < getRows(); k++)
        printValue(i, k);
    else
      for(auto const& k : getRowList())
        printValue(i, k);
    printf("\n");
  }
}

void bank::printValue(int schemaEntry, int row) const {
  switch(bankSchema.getEntryType(schemaEntry)) {
    case kByte:
    case kShort:
    case kInt:
      printf("%8d ", getInt(schemaEntry, row));
      break;
    case kFloat:
      printf("%8.5f ", getFloat(schemaEntry, row));
      break;
    case kDouble:
      printf("%8.5f ", getDouble(schemaEntry, row));
      break;
    case kLong:
      printf("%14ld ", getLong(schemaEntry, row));
      break;
  }
}

}
