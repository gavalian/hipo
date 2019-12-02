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

   std::vector<cluster *> clusters;

 public:

   track();
   virtual ~track();

   void  setCluster(int superLayer, cluster &cluster);
   void  getFeatures(double* buffer, int offset);
   std::vector<int> getTrackIndexArray();
   void  print();
   bool  isValid();
};

}

#endif
