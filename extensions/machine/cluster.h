/*
 * File:   track.h
 * Author: gavalian
 *
 * Created on November 19, 2019, 10:14 AM
 */

#ifndef __CLAS12_CLUSTER__
#define __CLAS12_CLUSTER__

#include <iostream>
#include <vector>
#include "bank.h"

namespace clas12 {

class point {

public:
  point(int x, int y){
    this->x = x;
    this->y = y;
    label   = -2;
    index   = -1;
  }

  point(int x, int y, int idx){
    this->x = x;
    this->y = y;
    label   = -2;
    index   = idx;
  }

  friend std::ostream& operator<<(std::ostream &out, const point &p){
    return out << "(" << p.x << "," << p.y <<","<< p.label <<")";
  }

  static bool compare(const point* p1, const point* p2){
      if (p1->label == p2->label)
      { return p1->y>p2->y;}
      return p1->label>p2->label;
  }

  static int distance(point* p1, point* p2){
      int dist = (p1->x - p2->x);
      if ( dist < 0){
          return -dist;
      }
      return dist;
  }

  int x,y,label, index;
};

class match{

    std::vector<point*> points;

  public:
    match(){}
    match(const match &m){
      points = m.points;
    }
    virtual ~match(){}

    void addPoint(int x, int y, int index){
      points.push_back(new point(x,y,index));
    }

    std::vector<point*>  &getData(){ return points;}
    void reset(){points.clear();}
    std::vector<point*> range_query(std::vector<point*>* points, point* s, int eps);
    void find_clusters(std::vector<point*>& points, int eps, size_t minPts);
    void find(int eps, size_t minPts);
    void sort(){ std::sort(points.begin(), points.end(), point::compare);}
    void show();

};
}

#endif
