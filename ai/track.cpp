#include "track.h"
#include <cmath>

namespace clas12 {

cluster::cluster(){
  wires.resize(112*6,0);
  sector  = -1;
  region  = -1;
  trackid = -1;
}

cluster::cluster(const cluster &c){
    wires  = c.wires;
    region = c.region;
    sector = c.sector;
}

double cluster::getLayerCenterX(int layer){
    double center = -1;
    int    count  = 0;
    int    summ   = 0;
    for(int w = 0; w < 112; w++){
      if(wires[layer*112+w]>0) {
        summ += w;
        count++;
      }
    }
    if(count > 0) center = ((double) summ)/count;
    return center;
}

double cluster::getCenterX(){
    int   count = 0;
    double summ = 0.0;
    for(int l = 0; l < 6; l++){
      double value = getLayerCenterX(l);
      if(value>0){
        summ += value; count++;
      }
    }
    if(count>0) return (summ/count);
    return 0.0;
  }

  double cluster::getCenterY(){
    return 0;
  }

  void   cluster::print(){
    printf("********* CLUSTER : region = %5d, sector = %5d, trackid = %5d, center X = %8.2f\n"
            ,region, sector, trackid, getCenterX());
    for(int l = 0; l < 6; l++){
      for(int w = 0; w < 112 ; w++){
        if(wires[l*112+w]>0){
            printf("X");
        } else {
           printf("-");
        }
      }
      double center = getLayerCenterX(l);
      printf("  %5.2f\n",center);
    }
  }

  void   cluster::reset(){
    wires.resize(112*6,0);
    //for(int l = 0; l < 6; l++)
    //for(int w = 0; w < 112; w++) wires[l*112+w] = 0;
  }

  void   cluster::copy(cluster &c){
     for(int l = 0; l < 6; l++)
      for(int w = 0; w < 112; w++){
         setWire(l,w,c.getWire(l,w));
      }
  }

track::track(){
  for(int i = 0; i < 6; i++){
    clusters.push_back(new cluster());
  }
}

track::~track(){
  clusters.clear();
}

void track::setCluster(int superLayer, cluster &cluster){
  clusters[superLayer]->copy(cluster);
}

bool track::isValid(){
  if(fabs(clusters[0]->getCenterX()-clusters[1]->getCenterX())>15.0) return false;
  if(fabs(clusters[2]->getCenterX()-clusters[3]->getCenterX())>15.0) return false;
  if(fabs(clusters[4]->getCenterX()-clusters[5]->getCenterX())>15.0) return false;
  return true;
}

void track::getFeatures(double* buffer, int offset){
  for(int i = 0; i < 6; i++){
    buffer[offset + i] = clusters[i]->getCenterX();
  }
}

std::vector<int> track::getTrackIndexArray(){
  std::vector<int> vec;
  for(int i = 0; i < 6; i++){
    for(int l = 0; l < 6; l++){
      for(int w = 0; w < 112; w++){
         int value = clusters[i]->getWire(l,w);
         if(value>0) vec.push_back(value);
      }
    }
  }
  return vec;
}

void track::print(){
   printf("**************************************************************\n");
   printf("*** TRACK \n");
   printf("**************************************************************\n");
    for(int i = 0; i < 6; i++){
      clusters[i]->print();
    }
}
}
