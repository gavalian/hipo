/*
 * This sowftware was developed at Jefferson National Laboratory.
 * (c) 2017.
 */

#include "reader.h"
#include "hipoexceptions.h"
#include "record.h"

#include <cstdlib>
/**
 * HIPO namespace is used for the classes that read write
 * files and records.
 */
namespace hipo {
 /**
  * The constructor for reader, printWarning routine
  * will printout a warning message if the library
  * was not compiled with compression libraries LZ4 or GZIP
  */
  reader::reader(){
    printWarning();
    hipoutils.printLogo();
  }

  /**
   * Default destructor. Does nothing
   */
  reader::~reader(){
    if(inputStream.is_open()==true){
      inputStream.close();
    }
  }
  /**
   * Open file, if file stream is open, it is closed first.
   * At open time verification of file structure is performed.
   * If the signature does not match EVIO/HIPO template, the
   * file will be closed and warning message is printed.
   */
  void reader::open(const char *filename){

    if(inputStream.is_open()==true){
      inputStream.close();

    }

    inputStream.open(filename, std::ios::binary);
    inputStream.seekg(0,std::ios_base::end);
    inputStreamSize = inputStream.tellg();
    inputStream.seekg(0,std::ios_base::beg);
    if(inputStream.is_open()==false){
      printf("[ERROR] something went wrong with openning file : %s\n",
            filename);
      return;
    }
    readHeader();
    readIndex();
}

/**
 * Reads the file header. The endiannes is determined for bytes
 * swap. The header structure will be filled with file parameters.
 */
 void reader::readHeader(){

    std::vector<char>               headerBuffer;
    headerBuffer.resize(80);
    inputStream.read(&headerBuffer[0],80);

    header.uniqueid     = *(reinterpret_cast<int *>(&headerBuffer[0]));
    header.filenumber   = *(reinterpret_cast<int *>(&headerBuffer[4]));
    header.headerLength = *(reinterpret_cast<int *>(&headerBuffer[8]));
    header.recordCount  = *(reinterpret_cast<int *>(&headerBuffer[12]));

    header.indexArrayLength = *(reinterpret_cast<int *>(&headerBuffer[16]));
    int word_8 = *(reinterpret_cast<int *>(&headerBuffer[20]));

    header.userHeaderLength = *(reinterpret_cast<int *>(&headerBuffer[24]));
    header.magicNumber = *(reinterpret_cast<int *>(&headerBuffer[28]));
    header.userRegister = *(reinterpret_cast<long *>(&headerBuffer[32]));
    header.trailerPosition = *(reinterpret_cast<long *>(&headerBuffer[40]));
    // If magic word is reversed, then the file was written in BIG_ENDIAN
    // format, the bytes have to be swapped
    if(header.magicNumber==0x0001dac0){
       printf(" THIS FILE IS BIG ENDIAN: SWAPPING BYTES\n");
       header.uniqueid  = __builtin_bswap32(header.uniqueid);
       header.filenumber = __builtin_bswap32(header.filenumber);
       header.headerLength = __builtin_bswap32(header.headerLength);
       header.recordCount  = __builtin_bswap32(header.recordCount);
       header.userHeaderLength = __builtin_bswap32(header.userHeaderLength);
       header.indexArrayLength = __builtin_bswap32(header.indexArrayLength);
       word_8 = __builtin_bswap32(word_8);
       header.userRegister = __builtin_bswap64(header.userRegister);
       header.trailerPosition = __builtin_bswap64(header.trailerPosition);
    }

    header.version = word_8&0x000000FF;
    header.bitInfo = (word_8>>8)&0x00FFFFFF;
    header.firstRecordPosition = 4*header.headerLength + header.userHeaderLength;
    printf("----------------------------------------\n");
    printf("**** reader:: header version   : %d \n",header.version);
    printf("**** reader:: header length    : %d \n",header.headerLength*4);
    printf("**** reader:: first record pos : %lu\n",header.firstRecordPosition);
    printf("**** reader:: trailer position : %lu\n",header.trailerPosition);
    printf("**** reader:: file size        : %lu\n",inputStreamSize);
    printf("----------------------------------------\n");
    //int *signature = reinterpret_cast<int *>(&headerBuffer[0]);
    //printf("signature = %X\n",(unsigned int) *signature);
    //std::cout << "signature = " << std::ios::hex << (*signature) << '\n';
}

void  reader::readIndex(){
    inputRecord.readRecord(inputStream,header.trailerPosition,0);
    printf("*** reader:: trailer record event count : %d\n",inputRecord.getEventCount());
    hipo::event event;
    inputRecord.readHipoEvent(event,0);
    event.show();
    hipo::structure base;
    event.getStructure(base,32111,1);
    base.show();
    readerEventIndex.clear();
    int rows = base.getSize()/32;

    printf(" number of rows = %d\n",rows);
    for(int i = 0; i < rows; i++){
       long position = base.getLongAt( i*8);
       int  length   = base.getIntAt ( rows*8  + i*4);
       int  entries  = base.getIntAt ( rows*12 + i*4);
       long uid1     = base.getLongAt( rows*16 + i*8);
       long uid2     = base.getLongAt( rows*24 + i*8);
       //printf("record # %4d POSITION = %12lu , LENGTH = %12d , ENTRIES = %6d , UID = %12lu %12lu\n",
          //i,position,length,entries, uid1,uid2);
       readerEventIndex.addSize(entries);
       readerEventIndex.addPosition(position);
    }
    readerEventIndex.rewind();
}

bool  reader::hasNext(){ return readerEventIndex.canAdvance();}

bool  reader::next(hipo::event &dataevent){
    int recordNumber = readerEventIndex.getRecordNumber();
    readerEventIndex.advance();
    int recordToBeRead = readerEventIndex.getRecordNumber();
    if(recordToBeRead!=recordNumber){
      long position = readerEventIndex.getPosition(recordToBeRead);
      inputRecord.readRecord(inputStream,position,0);
      /*printf(" record changed from %d to %d at event %d total event # %d\n",
        recordNumber, recordToBeRead,readerEventIndex.getEventNumber(),
        readerEventIndex.getMaxEvents());*/
    }
    int eventNumberInRecord = readerEventIndex.getRecordEventNumber();
    inputRecord.readHipoEvent(dataevent,eventNumberInRecord);
    return true;
}

void  reader::read(hipo::event &dataevent){
  int eventNumberInRecord = readerEventIndex.getRecordEventNumber();
  inputRecord.readHipoEvent(dataevent,eventNumberInRecord);
}

void  reader::readDictionary(hipo::dictionary &dict){
  long position = header.headerLength*4;
  hipo::record  dictRecord;
  dictRecord.readRecord(inputStream,position,0);
  int nevents = dictRecord.getEventCount();
  printf(" reading record at position %8lu, number of entries = %5d\n",
      position,dictRecord.getEventCount());
  hipo::structure schemaStructure;
  hipo::event  event;
  for(int i = 0; i < nevents; i++){
    dictRecord.readHipoEvent(event,i);
    event.getStructure(schemaStructure,120,2);
    printf("schema : %s\n",schemaStructure.getStringAt(0).c_str());
    dict.parse(schemaStructure.getStringAt(0).c_str());
  }
}

bool  reader::next(){
    if(readerEventIndex.canAdvance()==false) return false;
    int recordNumber = readerEventIndex.getRecordNumber();
    readerEventIndex.advance();
    int recordToBeRead = readerEventIndex.getRecordNumber();
    if(recordToBeRead!=recordNumber){
      long position = readerEventIndex.getPosition(recordToBeRead);
      inputRecord.readRecord(inputStream,position,0);
      /*printf(" record changed from %d to %d at event %d total event # %d\n",
        recordNumber, recordToBeRead,readerEventIndex.getEventNumber(),
        readerEventIndex.getMaxEvents());*/
    }
    return true;
}

void reader::printWarning(){
    #ifndef __LZ4__
      std::cout << "******************************************************" << std::endl;
      std::cout << "* WARNING:                                           *" << std::endl;
      std::cout << "*   This library war compiled without LZ4 support.   *" << std::endl;
      std::cout << "*   Reading and writing compressed buffers will not  *" << std::endl;
      std::cout << "*   work. However un-compressed file I/O will work.  *" << std::endl;
      std::cout << "******************************************************" << std::endl;
    #endif
  }
}


namespace hipo {

  void readerIndex::addSize(int size){
    if(recordEvents.size()==0){
      recordEvents.push_back(0);
      recordEvents.push_back(size);
    } else {
      int cz = recordEvents[recordEvents.size()-1] + size;
      recordEvents.push_back(cz);
    }
  }

  bool readerIndex::canAdvance(){
    return (currentEvent<getMaxEvents()-1);
  }

  bool readerIndex::advance(){
    if(recordEvents.size()==0) return false;
    if(currentEvent+1<recordEvents[currentRecord+1]){
      currentEvent++;
      currentRecordEvent++;
      return true;
    }

    if(recordEvents.size() < currentRecord + 2 + 1){
      printf("advance(): Warning, reached the limit of events.\n");
      return false;
    }
    currentEvent++;
    currentRecord++;
    currentRecordEvent = 0;
    return true;
  }

  int readerIndex::getMaxEvents(){
    if(recordEvents.size()==0) return 0;
    return recordEvents[recordEvents.size()-1];
  }

}
