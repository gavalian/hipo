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

        hipo::event hev;
        hipo::bank  bank;

        int  getIndex(int layer, int component);
        //int  getIndex112(int layer, int component);
        int  check(std::vector<float> &vec, int layer, int component,double threshold);

    public:

      drift(){}

      virtual ~drift(){}
      void setRows(int rows) { rowSize = rows;}
      void  map(std::vector<int> &index, std::vector<float> &output, hipo::bank &bank, int sector);
      void  initvec(std::vector<float> &tensor);
      void  create(std::vector<float> &tensor, hipo::bank &bank, int sector);
      void  process(const fdeep::model &model, hipo::bank &bank);

      hipo::event &event(){ return hev;}
  };
}
#endif