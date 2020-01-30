#include <iostream>
#include <map>
#include <vector>
#include <string>
#include "utils.h"
#include <functional>
#include <cmath>

std::vector<long>  hashCodesPower;

void initHash(){
  for(int i = 0; i < 10; i++){
    long pw = pow(31,i+1);
    hashCodesPower.push_back(pw);
  }
}

long hashCode(std::string str){
    long h = 0;
    for(int i = 0; i < str.size(); i++){
      h += hashCodesPower[i]*( (int) str[i] );
    }
    return h;
}

long iterateHash(std::vector<std::string> keys, int count){
  long result = 0;
  for(int i = 0; i < count; i++){
    for(int e = 0; e < keys.size(); e++){
        long pwh = hashCode(keys[e]);
        result += pwh%7;
    }
  }
  return result;
}

std::map<std::string,int> createMap(std::vector<std::string> keys){
  std::map<std::string,int> s_map;
  for(int i = 0; i < keys.size(); i++){
     s_map[keys[i]] = i+1;
  }
  return s_map;
}

std::map<long,int> createMapLong(std::vector<long> keys){
  std::map<long,int> s_map;
  for(int i = 0; i < keys.size(); i++){
     s_map[keys[i]] = i+1;
  }
  return s_map;
}

long  iterate(std::map<std::string,int> &smap, std::vector<std::string> &keys, int count){
   long result = 0;
   for(int i = 0; i < count; i++){
      for(int e = 0; e < keys.size(); e++){
          result += smap[keys[e]];
      }
   }
   return result;
}

long  iterateLong(std::map<long,int> &smap, std::vector<long> &keys, int count){
   long result = 0;
   for(int i = 0; i < count; i++){
      for(int e = 0; e < keys.size(); e++){
          result += smap[keys[e]];
      }
   }
   return result;
}

std::vector<long>  createHash(std::vector<std::string> &keys){
  std::vector<long> hvec;
  for(int i = 0; i < keys.size();i++){
    long hcode = std::hash<std::string>{}(keys[i]);
    printf("%24lu -> %s\n",hcode,keys[i].c_str());
  }
  return hvec;
}

long  benchString(std::map<std::string,int> &smap, std::vector<std::string> &keys, int count, int n){
   long result = 0;
   for(int i = 0 ; i < count; i++){
     result += iterate(smap,keys,n);
   }
   return result;
}

long  benchLong(std::map<long,int> &smap, std::vector<long> &keys, int count, int n){
   long result = 0;
   for(int i = 0 ; i < count; i++){
     result += iterateLong(smap,keys,n);
   }
   return result;
}

long  benchHash(std::vector<std::string> &keys, int count, int n){
   long result = 0;
   for(int i = 0 ; i < count; i++){
     result += iterateHash(keys,n);
   }
   return result;
}
int main(){

  std::vector<std::string> keys;
  keys.push_back("particle");
  keys.push_back("charge");
  keys.push_back("beta");
  keys.push_back("px");
  keys.push_back("py");
  keys.push_back("pz");

  std::vector<long>  ikeys = createHash(keys);

  std::map<std::string,int>  smap = createMap(keys);
  std::map<long,int>         imap = createMapLong(ikeys);

  hipo::benchmark  stringBench("string map");
  hipo::benchmark    longBench("long map");
  hipo::benchmark    hashBench("hash code");

  int niter = 1000;

  stringBench.resume();
  benchString(smap,keys,niter,100000);
  stringBench.pause();
  stringBench.show();

  longBench.resume();
  benchLong(imap,ikeys,niter,100000);
  longBench.pause();

  longBench.show();


  initHash();

  hashBench.resume();
  benchHash(keys,niter,100000);
  hashBench.pause();

  printf("----------------------------------------\n");
  stringBench.show();
  longBench.show();
  hashBench.show();

}
