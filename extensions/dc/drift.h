//=============================================================
//- 
//=============================================================
#include "reader.h"
#include <fdeep/fdeep.hpp>
#include "event.h"
#include <vector>

namespace dc {

  class drift {

    private:

        hipo::event hev;
        hipo::bank  bank;

        int  getIndex(int layer, int component);
        int  check(std::vector<float> &vec, int layer, int component,double threshold);

    public:

      drift(){}

      virtual ~drift(){}
      void  map(std::vector<int> &index, std::vector<float> &output, hipo::bank &bank, int sector);
      void  initvec(std::vector<float> &tensor);
      void  create(std::vector<float> &tensor, hipo::bank &bank, int sector);
      void  process(const fdeep::model &model, hipo::bank &bank);

      hipo::event &event(){ return hev;}
  };
}
