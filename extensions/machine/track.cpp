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
    trackid = c.trackid;
}

void   cluster::getWireHits(std::vector<int> &hitsVector){
  for(int i = 0; i < wires.size(); i++){
    if(wires[i]>0) hitsVector.push_back(wires[i]);
  }
}

void   cluster::getWireHits(std::vector<int> &hitsVector, std::vector<int> &hitsID, int id){
  for(int i = 0; i < wires.size(); i++){
    if(wires[i]>0){
      hitsVector.push_back(wires[i]);
      hitsID.push_back(id);
    }
  }
}

cluster    cluster::createMap(clas12::match &m){
  cluster c;
  std::vector<point*> &mt = m.getData();
  int size = mt.size();
  for(int i = 0; i < size; i++){
    c.setWire(mt[i]->y,mt[i]->x,mt[i]->index);
  }
  return c;
}

std::vector<cluster> cluster::create(clas12::match &m){
  std::vector<cluster> c;
  std::vector<point*> &mt = m.getData();
  int size = mt.size();
  if(size==0) return c;

  int     cid = -1;//mt[0]->label;
  int counter = -1;

  for(int i = 0; i < size; i++){
    int l = mt[i]->label;
    if(l<0) break;

    if(l!=cid){
      cid = l;
      cluster ct;
      c.push_back(ct);
      counter++;
    }
    //printf("%d %d %d %d %d\n",counter,mt[i]->x,mt[i]->y,mt[i]->index,mt[i]->label);
    c[counter].setWire(mt[i]->y,mt[i]->x,mt[i]->index);
  }
  return c;
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

track::track(const track &trk){
  weight = trk.weight;
  status = trk.status;
  clusters = trk.clusters;
  means    = trk.means;
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

bool  track::isNegative(){
    double a1 = (means[0]+means[1])*0.5;
    double a2 = (means[2]+means[3])*0.5;
    double a3 = (means[4]+means[5])*0.5;
    return ((a1-a2)>0 &&(a2-a3)>0);
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

bool  track::contains(track &trk){
  for(int i = 0; i < 6; i++){
    if(trk.getId(i)==getId(i)) return true;
  }
  return false;
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
  int id_trk   = hits.getSchema().getEntryOrder("trkID");
  for(int i = 0; i < nrows; i++){
     int hitsec = hits.getInt(id_sector,i);
     if(hitsec==sector){
       int hitid  = hits.getInt(id_hitid,i);
       int   cid  = hits.getInt(id_cid,i);
       int region = hits.getInt(id_region,i);
       int layer  = hits.getInt(id_layer,i);
       int wire   = hits.getInt(id_wire,i);
       int trkID  = hits.getInt(id_trk,i);
       if(clusterMap.count(cid)==0){
         clusterMap[cid] = new cluster();
       }
       clusterMap[cid]->setWire(layer-1,wire-1,hitid);
       clusterMap[cid]->setRegion(region-1);
       clusterMap[cid]->setSector(hitsec);
       clusterMap[cid]->setTrackId(trkID);
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

int  sector::getTrackId(int track){
  int trkid = sectorClusters[0][sectorTracks[track].getId(0)].getTrackId();
  //printf("trakid = %d\n",trkid);
  for(int i = 1; i < 6; i++){
      if(sectorClusters[i][sectorTracks[track].getId(i)].getTrackId()!=trkid) return 0;
  }
  if(trkid<0) trkid = 0;
  return trkid;
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
               //if(track.isValid()==true&&track.isNegative()==true)
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
  for(int i = 0; i < sectorTracks.size();i++){
    printf("%4d : ",getTrackId(i)); sectorTracks[i].print();
  }
}

void sector::showTrackInfo(){
  for(int i = 0; i < trackInfo.size(); i++){
    printf("track info %5d : id = %3d, charge = %3d , chi2 = %8.2f, sector = %4d, p = %8.3f\n",i,
       trackInfo[i].getId(), trackInfo[i].getCharge(), trackInfo[i].getChi2(),
       trackInfo[i].getSector(),trackInfo[i].getP());
  }
}

void sector::showBest(){
  int ns = sectorClusters.size();
  printf(" n tracks= %lu\n", sectorTracks.size());
  for(int s = 0; s < ns; s++){
    printf("SL : %2d => %5lu , ",s,sectorClusters[s].size());
  }

  printf("\n");
  for(int i = 0; i < sectorTracks.size();i++){
    int track_id = getTrackId(i);
    if(sectorTracks[i].getWeight()>0.5||track_id>0){
     printf("%4d : ",track_id);sectorTracks[i].print();
   }
  }
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

int  sector::getTrackCount(){
return sectorTracks.size();
}

int  sector::getBestTrackCount(){
  int count = 0;
  for(int i = 0; i < sectorTracks.size(); i++){
    if(sectorTracks[i].getStatus()>0) count++;
  }
return count;
}

int  sector::getBestTrack(){
  int     best_index = -1;
  double   best_prob = 0.5;
  for(int i = 0; i < sectorTracks.size(); i++){
    if(sectorTracks[i].getStatus()==0&&sectorTracks[i].getWeight()>best_prob){
       best_prob  = sectorTracks[i].getWeight();
       best_index = i;
    }
  }
  return best_index;
}

void sector::analyze(){

  int best_index = getBestTrack();
  if(best_index<0){
    for(int i = 0; i < sectorTracks.size(); i++) sectorTracks[i].setStatus(-1);
    return;
  }

  while(best_index>=0){
    sectorTracks[best_index].setStatus(2);
    for(int i = 0; i < sectorTracks.size(); i++){
      if(sectorTracks[i].getStatus()==0){
        if(sectorTracks[best_index].contains(sectorTracks[i])==true)
           sectorTracks[i].setStatus(-1);
      }
    }
    best_index = getBestTrack();
  }
}


void sector::readRaw(hipo::bank &hits, int sector){


  int rows = hits.getRows();

  std::vector<clas12::match> m;
  m.resize(6);
  for(int i = 0; i < rows; i++){
    /*int sec   = hits.getInt("sector",i);
    int layer = hits.getInt("layer",i);
    int comp  = hits.getInt("component",i);
*/

    int sec   = hits.getInt(0,i);
    int layer = hits.getInt(1,i);
    int comp  = hits.getInt(2,i);

    if(sec==sector){
      int   region = (int) (layer-1)/6;
      int sublayer = layer - region*6 -1;
      m[region].addPoint(comp-1,sublayer,i);
      //printf("add region = %d, sublayer = %d, layer = %d, component = %d\n",region,sublayer,layer,comp);
      /*if(layer>30&&layer<=36){
        int nl = layer-31;
        m.addPoint(comp-1,nl,i);
      }*/
    }
  }

  for(int i = 0; i < 6; i++) {
    m[i].find(2,4);
    m[i].sort();
    //m[i].show();
   }

  for(int i = 0; i < 6; i++){
    std::vector<cluster>  clusters = cluster::create(m[i]);
    for(int r = 0; r < clusters.size(); r++){
      clusters[r].setRegion(i);
      addCluster(clusters[r]);
    }
    //printf(">>>>>> region %d : clusters %d\n",i+1,(int) clusters.size());
    m[i].reset();
  }
  /*
  int msize = m.getData().size();
  printf(" data size for ML is %d\n",msize);
  m.find(2,4);
  m.show();

  std::vector<cluster>  clusters = cluster::create(m);
  printf("--> clusters size = %d\n",(int) clusters.size());
  for(int i = 0; i < clusters.size(); i++){
    clusters[i].print();
  }

  m.reset();*/
}

void sector::createWireHits(hipo::bank &bank){

  sectorWireHits.clear();

  std::vector<track> tracks;

  for(int i = 0; i < sectorTracks.size(); i++){
    if(sectorTracks[i].getStatus()>0) tracks.push_back(sectorTracks[i]);
  }
  int tracksSize = tracks.size();
  printf("-------->>>>>> identified tracks array size = %d\n",tracksSize);
  sectorWireHits.resize(tracksSize);

  for(int i = 0; i < tracksSize; i++){
      for(int s = 0; s < 6; s++){
        int index = tracks[i].getId(s);
        sectorClusters[s][index].getWireHits(sectorWireHits[i]);
      }
  }

  std::vector<int> hitsList;
  std::vector<int>   idList;

  for(int i = 0; i < tracksSize; i++){
      for(int s = 0; s < 6; s++){
        int index = tracks[i].getId(s);
        sectorClusters[s][index].getWireHits(hitsList,idList,i+1);
      }
  }

  int bankRows = hitsList.size();
  bank.setRows(bankRows);
  //bank.show();
  for(int i = 0; i < bankRows; i++){
    bank.putByte("id",i, (int8_t) idList[i]);
    bank.putShort("index",i,(int16_t) hitsList[i]);
  }


  for(int i = 0; i < tracksSize; i++){
    printf("******** %5d : wires = %lu\n",i,sectorWireHits[i].size());
  }
}

void sector::readTrackInfo(hipo::bank &trkBank){
   trackInfo.clear();
   int nrows = trkBank.getRows();
   for(int i = 0; i < nrows; i++){
     int id = trkBank.getInt("id",i);
     int q  = trkBank.getInt("q",i);
     int sector = trkBank.getInt("sector",i);
     double chi2 = trkBank.getFloat("chi2",i);
     double px = trkBank.getFloat("p0_x",i);
     double py = trkBank.getFloat("p0_y",i);
     double pz = trkBank.getFloat("p0_z",i);
     double vz = trkBank.getFloat("Vtx0_z",i);

     //printf("MOM = %f %f %f \n",px,py,pz);
     trackinfo info;
     info.setTrack(id,q,chi2);
     info.setSector(sector);
     info.setVz(vz);
     info.setP(sqrt(px*px+py*py+pz*pz));
     double theta = acos(pz/sqrt(px*px+py*py+pz*pz));
     info.setTheta(theta);
     trackInfo.push_back(info);
   }
}

int  sector::getTrackInfoIndex(int charge, int sector){
  int count = -1;
  for(int i = 0; i < trackInfo.size(); i++){
    if(trackInfo[i].getCharge()==charge&&trackInfo[i].getSector()==sector){
      return i;
    }
  }
  return count;
}

int  sector::getTrackInfoCount(int charge, int sector){
  int count = 0;
  for(int i = 0; i < trackInfo.size(); i++){
    if(trackInfo[i].getCharge()==charge&&trackInfo[i].getSector()==sector){
      count++;
    }
  }
  return count;
}

std::string sector::getTrackInfoString(int charge, int sector){
  double chi2 = -1.0;
  double p    = -1.0;
  double vz   = -1000.0;
  double theta = 0.0;
  int index = getTrackInfoIndex(charge,sector);
  if(index>=0){
    chi2 = trackInfo[index].getChi2();
    p    = trackInfo[index].getP();
    vz   = trackInfo[index].getVz();
    theta = trackInfo[index].getTheta()*57.29;
  }
  char line[128];
  sprintf(line," %12.3f %8.3f %12.3f %8.3f ",chi2,p,vz,theta);
  return std::string(line);
}



}
