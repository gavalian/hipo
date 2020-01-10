#include "dcana.h"

namespace clas12 {


void dcana::show(){
  printf("\n------ dcana : nothing to show... come back later\n");
}
/*
  void dcsector::add(cluster *cl){
    int region = cl->getRegion();
    regionclusters[region].push_back(cl);
  }

  void dcsector::print(){
      for(int i = 0; i < 6; i++){
        printf("REGION %4d : SIZE = %5lu\n", i, regionclusters[i].size());
      }
  }


void dcana::readClusters(hipo::bank &hits, int sector){

    int nrows = hits.getRows();
    std::map<int,cluster*> clusterMap;
    int id_sector = hits.getSchema().getEntryOrder("sector");
    int id_hitid  = hits.getSchema().getEntryOrder("id");
    int id_cid    = hits.getSchema().getEntryOrder("clusterID");
    int id_region = hits.getSchema().getEntryOrder("superlayer");
    int id_layer  = hits.getSchema().getEntryOrder("layer");
    int id_wire   = hits.getSchema().getEntryOrder("wire");
    for(int i = 0; i < nrows; i++){

        int hitsec = hits.getInt(id_sector,i);
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

    dcSectors.clear();
    dcSectors.resize(6);

    std::map<int,cluster*>::iterator it = clusterMap.begin();
    while(it != clusterMap.end()){
      //printf("----- cluster id = %d\n",it->first);
      //it->second->print();
      //if(it->second->getSector()==sector) dc.add(it->second);
      //vec.push_back(it->second);
      int sec = it->second->getSector();
      dcSectors[sec-1].add(it->second);
      it++;
    }

}

void dcana::makeTracks(){
    dcTracks.clear();
    for(int i = 0; i < 1; i++){
      makeTracks(i);
    }
    int valid = 0;
    for(int i = 0; i < dcTracks.size(); i++)
      if(dcTracks[i]->isValid()==true) valid++;
    printf("TRACKS SIZE = %5lu, VALID = %5d\n", dcTracks.size(),valid);

}

void dcana::makeTracks(int sector){

    int n1 = dcSectors[sector].getRegion(0).size();
    int n2 = dcSectors[sector].getRegion(1).size();
    int n3 = dcSectors[sector].getRegion(2).size();
    int n4 = dcSectors[sector].getRegion(3).size();
    int n5 = dcSectors[sector].getRegion(4).size();
    int n6 = dcSectors[sector].getRegion(5).size();

    for(int r1 = 0; r1 < n1; r1++){
      for(int r2 = 0; r2 < n2; r2++){
        for(int r3 = 0; r3 < n3; r3++){
          for(int r4 = 0; r4 < n4; r4++){
            for(int r5 = 0; r5 < n5; r5++){
              for(int r6 = 0; r6 < n6; r6++){
                 clas12::track *track = new clas12::track();
                 track->setCluster(0,*dcSectors[sector].getRegion(0)[r1] );
                 track->setCluster(1,*dcSectors[sector].getRegion(1)[r2] );
                 track->setCluster(2,*dcSectors[sector].getRegion(2)[r3] );
                 track->setCluster(3,*dcSectors[sector].getRegion(3)[r4] );
                 track->setCluster(4,*dcSectors[sector].getRegion(4)[r5] );
                 track->setCluster(5,*dcSectors[sector].getRegion(5)[r6] );
                 if(track->isValid()==true)
                  dcTracks.push_back(track);
              }
            }
          }
        }
      }
    }
}

void dcana::print(){
  //std::map<std::string, int>::iterator it = mapOfWordCount.begin();
}

int dcana::getNtracks(){
   return dcTracks.size();
}

void dcana::getFeatures(double* features){
    int ntracks = dcTracks.size();
    for(int i = 0; i < ntracks; i++){
      dcTracks[i]->getFeatures(features,i*6);
    }
}

void  dcana::showFeatures(){
    std::vector<double> features;
    int ntracks = getNtracks();
    features.resize(ntracks*6);
    getFeatures(&features[0]);
    printf("n tracks = %d\n",ntracks);
    for(int i = 0; i < features.size(); i++){
      if(i%6==0) printf("---\n");
      printf(" %6.3f ",features[i]);
    }
    printf("\n");
}
void dcana::showTracks(){
    for(int i = 0; i < dcTracks.size(); i++){
      dcTracks[i]->print();
    }
}*/
}
