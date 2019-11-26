/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   dictionary.h
 * Author: gavalian
 *
 * Created on April 27, 2017, 10:01 AM
 */

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <map>
#include <string>
#include <vector>

//#include "reader.h"

namespace hipo {

  typedef struct {
    std::string  name;
    std::string  type;
    int          typeId{};
    int          typeSize{};
    int          offset{};
  } schemaEntry_t;

class schema {
  private:

    std::map<std::string, int>    schemaEntriesMap;
    std::vector<schemaEntry_t>    schemaEntries;

    int         groupid;
    int         itemid;
    int         rowLength{};
    std::string schemaName;


    int  getTypeSize(int id);
    int  getTypeByString(std::string &typeName);

    
  public:

    schema(){ groupid = 0; itemid = 0; rowLength = 0;}
    schema(const char *name, int __group,int __item){
      schemaName = name; groupid = __group; itemid = __item;
    }
    schema(const schema &s) {
      schemaName    = s.schemaName;
      schemaEntries = s.schemaEntries;
      schemaEntriesMap = s.schemaEntriesMap;
      groupid       = s.groupid;
      itemid        = s.itemid;
      }

    virtual ~schema()= default;

    void  parse(const std::string& schString);
    std::string   getName(){ return schemaName;}
    int   getGroup(){ return groupid;}
    int   getItem(){ return itemid;}
    int   getSizeForRows(int rows);
     
    int  getRowLength()  const noexcept{
      const int nentries = schemaEntries.size()-1;
      const auto &sch=schemaEntries[nentries];
      return sch.offset + sch.typeSize;
    }

    int   getEntryOrder(const char *name) const noexcept {
    return schemaEntriesMap.at(name);//at needed for const function
    }
 
    int   getOffset(int item, int order, int rows) const noexcept {
      const auto &sch=schemaEntries[item];
      return  rows*sch.offset + order*sch.typeSize;
    }

    int   getOffset(const char *name, int order, int rows);
    int   getEntryType(int item) const noexcept {
      return schemaEntries[item].typeId;
    }
    std::string getEntryName(int item)  const noexcept { return schemaEntries[item].name;}
    int   getEntries() const noexcept { return schemaEntries.size();}
    void  show();

    std::string  getSchemaString();
    std::string  getSchemaStringJson();
    
    void operator = (const schema &D ) {
         schemaName = D.schemaName;
         groupid    = D.groupid;
         itemid     = D.itemid;
         schemaEntries = D.schemaEntries;
         schemaEntriesMap = D.schemaEntriesMap;
    }
};

  class dictionary {
  private:
    std::map<std::string,schema> factory;
  public:
    dictionary()= default;;
    virtual ~dictionary()= default;;

    std::vector<std::string> getSchemaList();
    void    addSchema(schema sc){ factory[sc.getName()] = sc;}
    bool    hasSchema(const char *name) { return (factory.count(name)!=0);}
    schema &getSchema(const char *name){ return factory[name];}
    bool    parse(const char *schemaString);
    void    show();
  };

}

#endif /* NODE_H */
