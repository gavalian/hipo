/*
 * File:   track.h
 * Author: gavalian
 *
 * Created on November 19, 2019, 10:14 AM
 */

#ifndef __CLAS12_DCANA__
#define __CLAS12_DCANA__

#include <iostream>
#include <vector>
#include "bank.h"
#include "track.h"

namespace clas12 {

class dcsector {

    std::vector< std::vector<cluster*> > regionclusters;

  public:
    dcsector(){ regionclusters.resize(6);}
    ~dcsector(){};
  /*  std::vector<cluster*> &getRegion(int region){ return regionclusters[region];};
    void add(cluster *cl);
    void print();*/
};

class dcana {
    //std::vector<cluster*> dc_clusters;
    std::vector<dcsector> dcSectors;
    std::vector<track*>    dcTracks;

public:

      dcana(){};
      virtual ~dcana(){};

      void show();
/*
      void readClusters(hipo::bank &hits, int sector);
      void makeTracks();
      void makeTracks(int sector);
      void print();
      void showTracks();

      int   getNtracks();
      void  getFeatures(double* features);
      void  showFeatures();
      std::vector<int> getTrackIndex(int track){
        return dcTracks[track]->getTrackIndexArray();
      }*/
};
}

#endif
