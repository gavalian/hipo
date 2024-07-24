/*----------------------------------------------------------------------
 * This is the implementation of fusion class which is used in
 * interfacing HIPO library from python.
 * author: G.Gavalian (June, 2022)
 * ---------------------------------------------------------------------
 */

#include "fusion.h"


namespace hipo {

  void inputSource::open(const char *filename){
   reader.open(filename);
   reader.readDictionary(factory);
  }

  std::string  inputSource::schema(const char *bank){
    return factory.getSchema(bank).getSchemaString();
  }
  
  void inputSource::define(const char *bank){
      banks[std::string(bank)] = hipo::bank(factory.getSchema(bank));
  }

  void   inputSource::describe(const char *bank){
    banks[std::string(bank)].getSchema().show();
  }
  
  bool inputSource::next(){
     bool status = reader.next();
     if(status==true){
       reader.read(event);
       std::map<std::string, hipo::bank>::iterator it;
       for (it = banks.begin(); it != banks.end(); it++)
       {
         event.getStructure(it->second);
            //How do I access each element?  
       }  
     }
     return status;
  }

int inputSource::getSize(const char *bank){
  return banks[bank].getRows();
}


int     inputSource::getInt(   const char *bank, const char *entry, int row){
   return banks[bank].getInt(entry,row);
}

int64_t     inputSource::getLong(   const char *bank, const char *entry, int row){
   return banks[bank].getLong(entry,row);
}
  
  float  inputSource::getFloat( const char *bank, const char *entry, int row){
   return banks[bank].getFloat(entry,row);
}

double  inputSource::getDouble( const char *bank, const char *entry, int row){
   return banks[bank].getDouble(entry,row);
}

  void inputSource::getByteArray(  const char *bank, int entry, int8_t *ptr, int length){
    hipo::bank &b = banks[bank];
    for(int i = 0; i < length; i++) ptr[i] = b.getByte(entry,i);
  }
  void inputSource::getShortArray( const char *bank, int entry, int16_t *ptr, int length){
    hipo::bank &b = banks[bank];
    for(int i = 0; i < length; i++) ptr[i] = b.getShort(entry,i);
  }
  void inputSource::getIntArray(   const char *bank, int entry, int32_t  *ptr, int length){
    hipo::bank &b = banks[bank];
    for(int i = 0; i < length; i++) ptr[i] = b.getInt(entry,i);
  }
  
  void inputSource::getFloatArray( const char *bank, int entry, float   *ptr, int length){
    hipo::bank &b = banks[bank];
    for(int i = 0; i < length; i++) ptr[i] = b.getFloat(entry,i);
  }


   int  fusion::open(const char *filename){
     int number = sources.size() + 1;
     sources.insert(std::make_pair(number, new inputSource(filename)));
     return number;
   }

  void  fusion::define(int fid, const char *bank){
     sources[fid]->define(bank);
  }

  std::string fusion::schema(int fid, const char *bank){
    return sources[fid]->schema(bank);
  }
  void fusion::describe(int fid, const char *bank){
    sources[fid]->describe(bank);
  }
  
  int  fusion::getSize(int fid, const char *bank){
     return sources[fid]->getSize(bank);
  }

bool   fusion::next(int handle){
   return sources[handle]->next();
}

int     fusion::getInt(   int handle, const char *bank, const char *entry, int row){
   return sources[handle]->getInt(bank,entry,row);
}

  int64_t     fusion::getLong(   int handle, const char *bank, const char *entry, int row){
   return sources[handle]->getLong(bank,entry,row);
}

  float  fusion::getFloat( int handle, const char *bank, const char *entry, int row){
   //double result = sources[handle]->getFloat(bank,entry,row);
   //printf("reuslt = %f\n",result);
  return sources[handle]->getFloat(bank,entry,row);
}
  
  double  fusion::getDouble( int handle, const char *bank, const char *entry, int row){
    return sources[handle]->getDouble(bank,entry,row);
  }

int     fusion::getType( int handle, const char *bank, const char *entry){
  hipo::bank b = sources[handle]->get(bank);
  return b.getSchema().getEntryType(b.getSchema().getEntryOrder(entry));
}
}
