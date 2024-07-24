//******************************************************************
//*  ██╗  ██╗██╗██████╗  ██████╗     ██╗  ██╗    ██████╗
//*  ██║  ██║██║██╔══██╗██╔═══██╗    ██║  ██║   ██╔═████╗
//*  ███████║██║██████╔╝██║   ██║    ███████║   ██║██╔██║
//*  ██╔══██║██║██╔═══╝ ██║   ██║    ╚════██║   ████╔╝██║
//*  ██║  ██║██║██║     ╚██████╔╝         ██║██╗╚██████╔╝
//*  ╚═╝  ╚═╝╚═╝╚═╝      ╚═════╝          ╚═╝╚═╝ ╚═════╝
//************************ Jefferson National Lab (2017) ***********
//******************************************************************
//* A simple library to do physics analysis directly from hipo file
//* Author: G.Gavalian
//*   Date: 11/22/2023
//*--


#ifndef __FIZIKA_REACTION__
#define __FIZIKA_REACTION__

#include <cstdlib>
#include <iostream>
#include "reader.h"
#include "fizika.h"

namespace fizika {

class reaction {

   private:
      hipo::reader  reader;
      hipo::dictionary  factory;
      hipo::event      event;

      fizika::lorentz4  lz4_beam,lz4_target, centermass;

      std::vector<hipo::bank>  banks;
      std::vector<std::pair<int,int>> filter;
      bool filter_exclusive = true;

      void initialize(const char* file, std::vector<std::string> blist){
         reader.open(file);
         reader.readDictionary(factory);
         for(decltype(blist)::size_type i = 0; i < blist.size(); i++) {
            hipo::bank b(factory.getSchema("REC::Particle"));
            banks.push_back(b);
         }
      }
      
   public:

  reaction(){}
     reaction(const char* file){ 
       initialize(file,{"REC::Particle"});
     }
     reaction(const char *file, std::initializer_list<std::tuple<int,int> > desc){
        initialize(file,{"REC::Particle"});
        init_filter(desc);
     }
     reaction(const char *file, double benergy, std::initializer_list<std::tuple<int,int> > desc){
        initialize(file,{"REC::Particle"});
        init_filter(desc);
        lz4_beam.setXYZM(0.0,0.0,benergy, 0.0005);
        lz4_target.setXYZM(0.0,0.0,0.0,0.938);
        centermass = lz4_beam + lz4_target;
     }

     reaction(const char *file, double benergy, int *pids, int *count, int length){
       initialize(file,{"REC::Particle"});
       lz4_beam.setXYZM(0.0,0.0,benergy, 0.0005);
       lz4_target.setXYZM(0.0,0.0,0.0,0.938);
       centermass = lz4_beam + lz4_target;
       printf(">>>> initializing the reaction\n");
       for(int k = 0; k < length; k++){
	 filter.push_back(std::make_pair(pids[k],count[k]));
	 printf("push back -> %6d %6d\n",pids[k],count[k]);
       }
     }
	   
   virtual ~reaction(){}


   fizika::lorentz4 &cm(){return centermass;}

   void init_filter(std::initializer_list<std::tuple<int,int> > desc){
         std::initializer_list< std::tuple<int,int>>::iterator it;  // same as: const int* it
         for ( it=desc.begin(); it!=desc.end(); ++it){
            printf("PID %d, count %d\n", std::get<0>(*it),std::get<1>(*it));
            filter.push_back(std::make_pair( std::get<0>(*it),std::get<1>(*it)));
           //banks[counter].parse(std::get<0>(*it),std::get<1>(*it),std::get<2>(*it),std::get<3>(*it));
            //counter++;
        }
      }

   fizika::lorentz4  get(std::initializer_list<std::tuple<int,int,int, double> > desc){
      fizika::lorentz4 vec(0.0,0.0,0.0,0.0), temp;
      std::initializer_list< std::tuple<int,int,int ,double>>::iterator it;
      for ( it=desc.begin(); it!=desc.end(); ++it){
         int ind = index(std::get<1>(*it), std::get<2>(*it));
         if(ind>=0) {
            get_vector(temp,std::get<3>(*it),banks[0],ind, 1,2,3);
            if(std::get<0>(*it)<0) vec -= temp; else vec += temp;
         }
      }
      return vec;
   }

  fizika::lorentz4 get(int *signs, int *pids, int *skips, double *masses, int length){
    fizika::lorentz4 vec(0.0,0.0,0.0,0.0), temp;
    //printf("analyzing # %d counts \n", length);
    for(int k = 0; k < length; k++){
      int ind = index(pids[k],skips[k]);
      if(ind>=0){
	get_vector(temp,masses[k],banks[0],ind,1,2,3);
	if(signs[k]<0) vec -= temp; else vec += temp;
      }
    }
    //printf("final vector %f %f %f %f\n",vec.x(),vec.y(),vec.z(),vec.e());
    return vec;
  }
  
   bool next(){
      bool status = reader.next();
      if(status==false) return status;
      reader.read(event);
      for(decltype(banks)::size_type r = 0; r < banks.size(); r++) event.read(banks[r]);
      return true;
   }
   
   void get_vector(fizika::lorentz4 &vec, double mass, hipo::bank &b, int order, int ind_px, int ind_py, int ind_pz){
      vec.setXYZM(b.getFloat(ind_px, order), b.getFloat(ind_py, order),b.getFloat(ind_pz, order),mass);
   }

   bool is_valid(){
    for(decltype(filter)::size_type f = 0; f < filter.size(); f++){
         int count = countpid(filter[f].first);
         //printf(" pid = %d, count = %d\n",filter[f].first, count);
         if(filter_exclusive==true){
            if(count!=filter[f].second) return false;
         } else { if(count<filter[f].second) return false; }
      }
      return true;
   }

   int countpid(int pid){
      int counter = 0;
      for(int r = 0; r < banks[0].getRows(); r++){
         int upid = banks[0].getInt(0,r);
         int stat = banks[0].getInt("status",r);
         if(upid == pid)
            if(abs(stat)>2000&&abs(stat)<3000) counter++;
      }
      return counter;
   }

   int index(int pid, int skip){
      int skipped = 0;
      for(int r = 0; r < banks[0].getRows(); r++){
         int upid = banks[0].getInt(0,r);
         int stat = banks[0].getInt("status",r);
         if(upid == pid)
            if(abs(stat)>2000&&abs(stat)<3000){
               if(skip==skipped) { return r; } else { skipped++;}
            } 
      }
      return -1;
   }

   std::vector<hipo::bank>  &getBanks(){return banks;}
  fizika::lorentz4   &beam(){    return lz4_beam; }
  fizika::lorentz4 &target(){  return lz4_target; }
};
}
#endif
