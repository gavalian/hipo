#include "cluster.h"
#include <cmath>

namespace clas12 {

std::vector<point*> match::range_query(std::vector<point*>* points, point* s, int eps){
  std::vector<point*> neighbors;
  int min = 10000;
  int start = s->y-1;
  int i = start;
  point* q = s;
  while (i>=0)
  {
      point* pnt = NULL;
      min = 10000;
      for (size_t j=0; j< points[i].size() ; j++)
      {
          point* p = points[i][j];
          int dist = point::distance(q,p);
          if (dist <= eps * (start-i+1))
          {
              if ( dist < min )
              {
                  if ( p->label == -2)
                  {
                      pnt = p;
                      min = dist;
                  }
              }
          }
      }

      if (pnt != NULL)
      {
          neighbors.push_back(pnt);
          q = pnt;
          start = i-1;
      }
      i -= 1;
  }
  return neighbors;
}


/** Given a set of points representing detections in the drift chambers, assigns each point to a cluster
 *  Points assigned label -1 are tagged as noise.
 *
 *  This algorithm works similarly to the DBSCAN algorithm, specifically:
 *  Starting from the top layer once a point is found we try we find the closest point in the
 *  next layer up to eps distance in the x-axis. Next we start from this point and find the closest
 *  one in the next layer. If a point is not found in distance eps we check the following layer but
 *  the maximum distance is now doubled.
 *  This continues until we reach the last layer. If the number of points found when reaching the
 *  last layer is at least minPts a cluster is formed.
 *
 *  @param points A vector containing the points of detections in the drift chambers
 *  @param eps The maximum distance in the x-axis to look for points to form a cluster
 *  @param minPts The minimum number of points that can form a cluster
 */
void match::find_clusters(std::vector<point*>& __points, int eps, size_t minPts){
    int C = -1;
    std::vector<point*> all_points[6];
    //printf("---- start procedure");
    for (size_t i = 0; i< __points.size(); i++)
    {
        all_points[__points[i]->y].push_back(__points[i]);
    }
    int i = 5;


    while (i >= 0)
    {
      //printf("--- iteration %d\n",i);
        for (size_t j = 0; j< all_points[i].size(); j++)
        {
            point* p = all_points[i][j];
            if (p->label != -2)
            {
                continue;
            }

            std::vector<point*> neighbors = range_query(all_points, p, eps);

            if ( neighbors.size() < (minPts-1) )
            {
                p->label = -1;
                continue;
            }

            C += 1;
            p->label = C;

            for(size_t k = 0; k< neighbors.size(); k++)
            {
                point* q = neighbors[k];
                q->label = C;
            }
        }
        //printf("--- insight %d\n",i);
        int aps = all_points[i].size();
      /*  try {
          for (size_t j = 0; j< all_points[i].size(); j++)
          {
            printf("---- erasing %d : %d %d\n",i,(int) j,aps);
              point* p = all_points[i][j];
              if (p->label == -1)
              {
                  for (size_t k = 0; k< all_points[i].size(); k++)
                  {
                    point* q = all_points[i][k];
                    if ( (point::distance(p,q) == 1) && q->label != -1)
                    {
                        p->label = q->label;
                        all_points[i].erase(all_points[i].begin()+k);
                        if ( k < j)
                        {
                            all_points[i].erase(all_points[i].begin()+(j-1));
                        }
                        else
                        {
                            all_points[i].erase(all_points[i].begin()+j);
                        }
                      }
                  }
              }
          }
       } catch(...){
         printf("well..... exception happened...\n");
       }*/

        i -= 1;
    }
}

void match::find(int eps, size_t minPts){
    find_clusters(points,eps,minPts);
}

void match::show(){
  std::sort(points.begin(), points.end(), point::compare);
  for ( std::vector<point*>::iterator it = points.begin(), end = points.end(); it != end ; ++it)
  {
      std::cout<< (**it);
      if ( (it+1) != end )
      {
          std::cout<< ", ";
      }
  }
  std::cout<<std::endl;
}
}
