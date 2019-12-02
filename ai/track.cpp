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
  weight = 0.0;
  status = 0;
  clusters.resize(6,-1);
  means.resize(6,-1.0);
  /*for(int i = 0; i < 6; i++){
    clusters.push_back(-1);
    means.push_back(-1.0);
  }*/
}

track::~track(){
  clusters.clear();
}
/*
void track::setCluster(int superLayer, cluster &cluster){
  clusters[superLayer]->copy(cluster);
}*/

bool track::isValid(){
    if(fabs(means[0]-means[1])>15.0) return false;
    if(fabs(means[2]-means[3])>15.0) return false;
    if(fabs(means[4]-means[5])>15.0) return false;

  /*if(fabs(clusters[0]->getCenterX()-clusters[1]->getCenterX())>15.0) return false;
  if(fabs(clusters[2]->getCenterX()-clusters[3]->getCenterX())>15.0) return false;
  if(fabs(clusters[4]->getCenterX()-clusters[5]->getCenterX())>15.0) return false;*/
  return true;
}
/*
void track::getFeatures(double* buffer, int offset){
  for(int i = 0; i < 6; i++){
    buffer[offset + i] = clusters[i]->getCenterX();
  }
}*/
/*
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
}*/
void  track::setCluster(int superlayer, int cid){
  clusters[superlayer] = cid;
}
void  track::setCluster(int superlayer, int cid, double mean){
  clusters[superlayer] = cid;
  means[superlayer] = mean;
}

void track::print(){
   //printf("**************************************************************\n");
   //printf("*** TRACK \n");
   //printf("**************************************************************\n");
   printf("track = ");
  for(int i = 0; i < 6; i++){
    printf(" (%3d,%8.2f) ", clusters[i],means[i]);
      //clusters[i]->print();
  }
  printf("  -  (%4d) %9.5f\n", getStatus(), getWeight());
}

sector::sector(){
  sectorClusters.resize(6);
  /*for(int i = 0; i < 6; i++){
    sectorClusters.push_back(std::vector<cluster>);
  }*/
}

void sector::read(hipo::bank &hits, int sector){
  std::map<int,cluster*> clusterMap;
  int nrows = hits.getRows();
  int id_sector = hits.getSchema().getEntryOrder("sector");
  int id_hitid  = hits.getSchema().getEntryOrder("id");
  int id_cid    = hits.getSchema().getEntryOrder("clusterID");
  int id_region = hits.getSchema().getEntryOrder("superlayer");
  int id_layer  = hits.getSchema().getEntryOrder("layer");
  int id_wire   = hits.getSchema().getEntryOrder("wire");
  for(int i = 0; i < nrows; i++){
     int hitsec = hits.getInt(id_sector,i);
     if(hitsec==sector){
       int hitid  = hits.getInt(id_hitid,i);
       int   cid  = hits.getInt(id_cid,i);
       int region = hits.getInt(id_region,i);
       int layer  = hits.getInt(id_layer,i);
       int wire   = hits.getInt(id_wire,i);

       if(clusterMap.count(cid)==0){
         clusterMap[cid] = new cluster();
       }
       clusterMap[cid]->setWire(layer-1,wire-1,hitid);
       clusterMap[cid]->setRegion(region-1);
       clusterMap[cid]->setSector(hitsec);
     }
  }

  std::map<int,cluster*>::iterator it = clusterMap.begin();
  while(it != clusterMap.end()){
    int region = it->second->getRegion();
    addCluster(*it->second);
    it++;
  }

  clusterMap.clear();
}

void sector::makeTracks(){
  sectorTracks.clear();
  int n1 = sectorClusters[0].size();
  int n2 = sectorClusters[1].size();
  int n3 = sectorClusters[2].size();
  int n4 = sectorClusters[3].size();
  int n5 = sectorClusters[4].size();
  int n6 = sectorClusters[5].size();

  for(int r1 = 0; r1 < n1; r1++){
    for(int r2 = 0; r2 < n2; r2++){
      for(int r3 = 0; r3 < n3; r3++){
        for(int r4 = 0; r4 < n4; r4++){
          for(int r5 = 0; r5 < n5; r5++){
            for(int r6 = 0; r6 < n6; r6++){
               clas12::track track;// = new clas12::track();
               track.setCluster(0,r1, sectorClusters[0][r1].getCenterX());
               track.setCluster(1,r2, sectorClusters[1][r2].getCenterX());
               track.setCluster(2,r3, sectorClusters[2][r3].getCenterX());
               track.setCluster(3,r4, sectorClusters[3][r4].getCenterX());
               track.setCluster(4,r5, sectorClusters[4][r5].getCenterX());
               track.setCluster(5,r6, sectorClusters[5][r6].getCenterX());
               if(track.isValid()==true)
                sectorTracks.push_back(track);
            }
          }
        }
      }
    }
  }
}

void sector::addCluster(cluster &cluster){
  int region = cluster.getRegion();
  sectorClusters[region].push_back(cluster);
}

void sector::show(){
  int ns = sectorClusters.size();
  printf(" n tracks= %lu\n", sectorTracks.size());
  for(int s = 0; s < ns; s++){
    printf("SL : %2d => %5lu , ",s,sectorClusters[s].size());
  }
  printf("\n");
  for(int i = 0; i < sectorTracks.size();i++)
    sectorTracks[i].print();
}
void sector::reset(){
  for(int i = 0; i < 6; i++)
    sectorClusters[i].clear();
}

std::vector<double>  sector::getFeatures(){
  std::vector<double> features;
  for(int i = 0; i < sectorTracks.size(); i++){
    for(int s = 0; s < 6; s++){
      features.push_back(sectorTracks[i].getMean(s));
    }
  }
  return features;
}

void sector::setWeights(double *wgt){
    for(int i = 0; i < sectorTracks.size(); i++){
        sectorTracks[i].setWeight(wgt[i]);
    }
}
}
