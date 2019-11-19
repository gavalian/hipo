#include "track.h"

namespace clas12 {

cluster::cluster(const cluster &c){
    for(int l = 0; l < 6; l++)
    for(int w = 0; w < 112; w++){
      wires[l][w] = c.getWire(l,w);
    }
}

  double cluster::getLayerCenterX(int layer){
    double center = -1;
    int    count  = 0;
    int    summ   = 0;
    for(int w = 0; w < 112; w++){
      if(wires[layer][w]>0) {
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
    for(int l = 0; l < 6; l++){
      for(int w = 0; w < 112 ; w++){
        if(wires[l][w]>0){
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
    for(int l = 0; l < 6; l++)
    for(int w = 0; w < 112; w++) wires[l][w] = 0;
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
}
