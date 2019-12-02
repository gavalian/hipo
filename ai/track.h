/*
 * File:   track.h
 * Author: gavalian
 *
 * Created on November 19, 2019, 10:14 AM
 */

#ifndef __CLAS12_TRACK__
#define __CLAS12_TRACK__

#include <iostream>
#include <vector>
#include "bank.h"

namespace clas12 {


class cluster {
    //int wires[6][112];
    std::vector<int> wires;
    int region;
    int sector;
    int trackid;

public:

    cluster();
    cluster(const cluster &c);
    virtual ~cluster(){}

    void reset();
    void setWire(int layer, int wire, int value){ wires[layer*112+wire] = value;}
    int  getWire(int layer, int wire) const { return wires[layer*112+wire];}

    int    getRegion(){ return region;}
    int    getSector(){ return sector;}
    int    getTrackId(){ return trackid;}
    void   setRegion(int r) { region = r;}
    void   setSector(int s) { sector = s;}
    void   setTrackId(int id){ trackid = id;}


    double getLayerCenterX(int layer);
    double getCenterX();
    double getCenterY();
    void   print();
    void   copy(cluster &c);
};

class track {

   //std::vector<cluster *> clusters;
   std::vector<int>     clusters;
   std::vector<double>  means;
   double               weight;
   int                  status;

 public:

   track();
   virtual ~track();
   void    setWeight(double w) { weight = w;}
   double  getWeight(){ return weight;}
   void    setStatus(int s) { status = s;}
   int     getStatus(){return status;}
   int     getId(int sl) { return clusters[sl];}
   double  getMean(int sl) {return means[sl];}
   void    setCluster(int superlayer, int cid);
   void    setCluster(int superlayer, int cid, double mean);
   std::vector<int> getClusters(){return clusters;};
   /*
   void  setCluster(int superLayer, cluster &cluster);
   void  getFeatures(double* buffer, int offset);
   std::vector<int> getTrackIndexArray();*/
   void  print();
   bool  isValid();
};

class sector {

    std::vector<std::vector<cluster>>  sectorClusters;
    std::vector<track>    sectorTracks;

public:

    sector();
    virtual ~sector(){}

    void read(hipo::bank &hits, int sector);
    void makeTracks();
    void addCluster(cluster &cluster);
    void show();
    void reset();
    std::vector<double>  getFeatures();
    void setWeights(double *wgt);
};

}

#endif
