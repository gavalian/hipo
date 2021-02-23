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

long hashCode(std::string &str){
    long h = 0;
    for(int i = 0; i < str.size(); i++){
      h += hashCodesPower[i]*( (int) str[i] );
      //h += pow(31,i+1)*( (int) str[i] );
    }
    return h;
}

long hashCodeXOR(std::string &str){
    //long h = 0;
    long *strLong = reinterpret_cast<long *>(&str[0]);
    //int  pwr     = pow(31,4);
    long   hcode = (*strLong)^( hashCodesPower[9]);//std::hash<std::string>{}(keys[i]);
    return hcode;
}

long iterateHash(std::vector<std::string> &keys, int count){
  long result = 0;
  for(int i = 0; i < count; i++){
    for(int e = 0; e < keys.size(); e++){
        long pwh = std::hash<std::string>{}(keys[e].c_str());//hashCode(keys[e]);
        result += pwh%7;
    }
  }
  return result;
}

long iterateHashXOR(std::vector<std::string> &keys, int count){
  long result = 0;
  for(int i = 0; i < count; i++){
    for(int e = 0; e < keys.size(); e++){
        long pwh = hashCodeXOR(keys[e]);
        //result += pwh%7;
        result += pwh;
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
    printf("sdt hash : %24lu -> %s\n",hcode,keys[i].c_str());
  }
  return hvec;
}

std::vector<long>  createHashXOR(std::vector<std::string> &keys){
  std::vector<long> hvec;
  for(int i = 0; i < keys.size();i++){
    //printf("%c\n",keys[i][0]);
    long *strLong = reinterpret_cast<long *>(&keys[i][0]);
    //int  pwr     = pow(31,4);
    long   hcode = (*strLong)^( hashCodesPower[9]);//std::hash<std::string>{}(keys[i]);
    //std::cout << hcode << std::endl;
    //printf("%lu  %lu\n",*strLong, hcode);
    printf("alt hash : %24lu -> %s\n",hcode,keys[i].c_str());
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

long  benchHashXOR(std::vector<std::string> &keys, int count, int n){
   long result = 0;
   for(int i = 0 ; i < count; i++){
     for(int k = 0; k < n; k++){
     for(int e = 0; e < keys.size(); e++){
       /*int len = 0;
       while(keys[e][len]!='\0'){
         len++;
       }*/
        long *strLong = reinterpret_cast<long*> (&keys[e][0]);
        long hash    = hashCodesPower[9]^(*strLong);
        //long hash    = hashCodeXOR(keys[e]);
        //printf(" --- analyzing %5d %5d %8s %24lu\n",i,n,keys[e].c_str(),hash);
        result += hash;//iterateHashXOR(keys,n);
        //result += (*strLong)%7;
     }
   }
   }
   return result;
}

int main(){

  std::vector<std::string> keys;
  keys.push_back("particle");
  keys.push_back("charge  ");
  keys.push_back("beta    ");
  keys.push_back("px      ");
  keys.push_back("py      ");
  keys.push_back("pz      ");

  initHash();

  std::vector<long>  ikeys = createHash(keys);
  std::vector<long>  xkeys = createHashXOR(keys);

  std::map<std::string,int>  smap = createMap(keys);
  std::map<long,int>         imap = createMapLong(ikeys);

  hipo::benchmark  stringBench("string map");
  hipo::benchmark    longBench("long map");
  hipo::benchmark    hashBench("hash code");
  hipo::benchmark    hashBenchXOR("hash code xor");

  int niter = 5000;

  stringBench.resume();
  //benchString(smap,keys,niter,100000);
  stringBench.pause();
  stringBench.show();

  longBench.resume();
  benchLong(imap,ikeys,niter,100000);
  longBench.pause();

  longBench.show();




  hashBench.resume();
  long result = benchHash(keys,niter,100000);
  printf(" result = %lu\n",result);
  hashBench.pause();

  hashBenchXOR.resume();
  result = benchHashXOR(keys,niter,100000);
  hashBenchXOR.pause();
  printf(" result = %lu\n",result);

  printf("----------------------------------------\n");
  stringBench.show();
  longBench.show();
  hashBench.show();
  hashBenchXOR.show();

}
