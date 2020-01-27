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
#include "cluster.h"

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

    void   getWireHits(std::vector<int> &hitsVector);
    void   getWireHits(std::vector<int> &hitsVector, std::vector<int> &hitsID, int id);
    double getLayerCenterX(int layer);
    double getCenterX();
    double getCenterY();
    void   print();
    void   copy(cluster &c);

    static std::vector<cluster> create(clas12::match &m);
    static cluster              createMap(clas12::match &m);
};

class track {

   //std::vector<cluster *> clusters;
   std::vector<int>     clusters;
   std::vector<double>  means;
   double               weight;
   int                  status;

 public:

   track();
   track(const track &trk);
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
   bool  isNegative();
   bool  contains(track &trk);
};

class trackinfo {

   int     trackid;
   int     charge;
   int     sector;
   double  p;
   double  chi2;
   double  vz;
   double theta;

 public:
   trackinfo(){}
   trackinfo(const trackinfo &t){
     trackid = t.trackid;
     charge  = t.charge;
     sector  = t.sector;
     chi2    = t.chi2;
     p       = t.p;
     vz      = t.vz;
     theta   = t.theta;
   }
   ~trackinfo(){};

   void setTrack(int id, int c, double c2){
     trackid = id; charge = c; chi2 = c2;
   }
   void setP(double __p){p=__p;}
   double getP(){return p;}
   void setSector(int s){ sector = s;}
   void setTheta(double __th){theta = __th;}
   void setVz(double __vz){ vz = __vz;}
   int getId(){ return trackid;};
   int getCharge(){ return charge;}
   int getSector(){ return sector;}
   double getTheta(){return theta;}
   double getVz(){ return vz;}
   double getChi2(){ return chi2;}
};

class sector {

    std::vector<std::vector<cluster>>  sectorClusters;
    std::vector<track>    sectorTracks;

    std::vector< std::vector<int> > sectorWireHits;
    std::vector<trackinfo>          trackInfo;

    int  getBestTrack();

public:

    sector();
    virtual ~sector(){}

    int  getTrackId(int track);

    void read(hipo::bank &hits, int sector);
    void readRaw(hipo::bank &hits, int sector);
    void readTrackInfo(hipo::bank &trkBank);

    void makeTracks();
    void addCluster(cluster &cluster);
    void show();
    void showBest();
    void showTrackInfo();
    void reset();
    std::vector<double>  getFeatures();
    void setWeights(double *wgt);
    int  getTrackCount();

    int  getTrackInfoIndex(int charge, int sector);
    int  getTrackInfoCount(int charge, int sector);
    std::string getTrackInfoString(int charge, int sector);

    int  getBestTrackCount();
    void analyze();
    void createWireHits(hipo::bank &bank);

};

}

#endif
