//=============================================================
//- 
//=============================================================
#include "reader.h"
#include <fdeep/fdeep.hpp>
#include "event.h"
#include <vector>

#ifndef __DC_DRIFT__
#define __DC_DRIFT__

namespace dc {

  class drift {
    
  private:
    int rowSize = 114;
    double threshold = 0.05;
    
    hipo::event hev;
    hipo::bank  bank;
    
    int  getIndex(int layer, int component);
    //int  getIndex112(int layer, int component);
    int  check(std::vector<float> &vec, int layer, int component,double threshold);
    int  check(std::vector<float> &vec, int layer, int component,double threshold_min, double threshold_max, bool include_max);
    void  map(std::vector<int> &index, std::vector<float> &output, hipo::bank &bank, int sector,
	      double threshold_min, double threshold_max, bool include_max);
    
  public:
    
    drift(){}
    drift(double __th){threshold = __th;}
    virtual ~drift(){}
    void setRows(int rows) { rowSize = rows;}
    void  map(std::vector<int> &index, std::vector<float> &output, hipo::bank &bank, int sector);
    
    void  initvec(std::vector<float> &tensor);
    int   countOrder(hipo::bank &bank, int order);
    void  create(std::vector<float> &tensor, hipo::bank &bank, int sector);
    int   process(const fdeep::model &model, hipo::bank &bank);
    int   processRange(const fdeep::model &model, hipo::bank &bank);
    void  setThreshold(double __th){threshold = __th;}
    hipo::event &event(){ return hev;}
  };
}
#endif
