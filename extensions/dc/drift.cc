#include "drift.h"

namespace dc {

  int  drift::getIndex(int layer, int component){
    int index = (layer-1)*rowSize + (component);
    return index;
  }
  
  void  drift::initvec(std::vector<float> &tensor){
    if(tensor.size()!=rowSize*36) tensor.resize(rowSize*36);
    for(int i = 0; i < rowSize*36; i++) tensor[i] = 0.0;
  }
  
  int  drift::check(std::vector<float> &vec, int layer, int component, double threshold){
    int index = getIndex(layer,component);
    if(vec[index]>=threshold&&vec[index]<=1.0) return 1;
    return 0;
  }
  
  int  drift::check(std::vector<float> &vec, int layer, int component, double threshold_min, double threshold_max,
		    bool include_max){
    int index = getIndex(layer,component);
    if(include_max == true) if(vec[index]>=threshold_min&&vec[index]<=threshold_max) return 1;
    if(include_max ==false) if(vec[index]>=threshold_min&&vec[index]<threshold_max) return 1;
    return 0;
  }

  void  drift::map(std::vector<int> &index, std::vector<float> &output, hipo::bank &bank, int sector,
		   double threshold_min, double threshold_max, bool include_max){
    int nrows = bank.getRows();
    for(int i = 0; i < nrows; i++){
      int     sec = bank.getInt(0,i);
      int   layer = bank.getInt(1,i);
      int    comp = bank.getInt(2,i);
      if(sector==sec){
	if(check(output,layer,comp, threshold_min,threshold_max,include_max)>0) index.push_back(i);
      }
    }
  }

  void  drift::map(std::vector<int> &index, std::vector<float> &output, hipo::bank &bank, int sector){
    
    int nrows = bank.getRows();
    for(int i = 0; i < nrows; i++){
      /*int     sec = bank.getInt("sector",i);
	int   layer = bank.getInt("layer",i);
	int    comp = bank.getInt("component",i);
      */
      int     sec = bank.getInt(0,i);
      int   layer = bank.getInt(1,i);
      int    comp = bank.getInt(2,i);
      
      if(sector==sec){
	 if(check(output,layer,comp, threshold)>0) index.push_back(i);
      }
    }
  }
  
  void  drift::create(std::vector<float> &tensor, hipo::bank &bank, int sector){
    int nrows = bank.getRows();
    for(int i = 0; i < nrows; i++){
      /*int     sec = bank.getInt("sector",i);
	int   layer = bank.getInt("layer",i);
	int    comp = bank.getInt("component",i);*/
      int     sec = bank.getInt(0,i);
      int   layer = bank.getInt(1,i);
      int    comp = bank.getInt(2,i);
      
      if(sector==sec){
	int index = getIndex(layer,comp);
	tensor[index] = 1.0;
      }
    }
  }
  
  int drift::process(const fdeep::model &model, hipo::bank &bank){
    
    std::vector<float>  dcmat;
    std::vector<int>    bank_iter;
    //int counter = 0;
    for(int s = 1; s <= 6; s++){
      initvec(dcmat);
      create(dcmat,bank,s);
      fdeep::tensors result = model.predict(
					    {fdeep::tensor(
							   fdeep::tensor_shape(static_cast<std::size_t>(36),
									       static_cast<std::size_t>(rowSize),
									       static_cast<std::size_t>(1)),
							   dcmat)}
					    );
      auto output = result[0].as_vector();
      std::vector<float>  buffer;
      for(int jj=0; jj< output->size(); jj++){ buffer.push_back( (*output)[jj]);}
      map(bank_iter,buffer,bank,s);
    }
    for(int nn = 0; nn < bank.getRows(); nn++) bank.putByte(3,nn,(int8_t) 10);
    int nindex = bank_iter.size();
    for(int nn = 0; nn < nindex; nn++){
      bank.putByte(3,bank_iter[nn],(int8_t) 0);
    }
    return nindex;
  }

  int drift::countOrder(hipo::bank &bank, int ord){
     int count = 0;
     for(int nn = 0; nn < bank.getRows(); nn++) {
      int order = bank.getInt(3,nn);
      if(order==ord) count++;
     }
     return count;
  }

  int drift::processRange(const fdeep::model &model, hipo::bank &bank){
    
    std::vector<float>  dcmat;
    std::vector<int>    bank_iterbb;
    std::vector<int>    bank_iter00;
    std::vector<int>    bank_iter40;
    std::vector<int>    bank_iter50;

    //int counter = 0;
    for(int s = 1; s <= 6; s++){
      initvec(dcmat);
      create(dcmat,bank,s);
      fdeep::tensors result = model.predict(
					    {fdeep::tensor(
							   fdeep::tensor_shape(static_cast<std::size_t>(36),
									       static_cast<std::size_t>(rowSize),
									       static_cast<std::size_t>(1)),
							   dcmat)}
					    );
      auto output = result[0].as_vector();
      std::vector<float>  buffer;
      
      for(int jj=0; jj< output->size(); jj++){ buffer.push_back( (*output)[jj]);}
      
      //map(bank_iter00,buffer, bank, s,  0.050,  1.00);
      //map(bank_iter40,buffer, bank, s,  0.010,  0.05);
      //map(bank_iter50,buffer, bank, s,  0.005,  0.01);

      map(bank_iter00,buffer, bank, s,  0.050,  1.000,  true);
      map(bank_iter40,buffer, bank, s,  0.010,  0.050, false);
      map(bank_iter50,buffer, bank, s,  0.005,  0.010, false);
      
      map(bank_iterbb,buffer,bank,s);
    }


  //printf(">>>>> bb = %8d, 00 = %8d, 40 = %8d, 50 = %8d\n", 
  //      (int) bank_iterbb.size(),
  //      (int) bank_iter00.size(),
  //      (int) bank_iter40.size(),
  //      (int) bank_iter50.size() 
  //      );
    
    int nindex = bank_iter00.size();
    
    
    for(int nn = 0; nn < bank.getRows(); nn++) {
      int order = bank.getInt(3,nn);
      if(order==0){
	         bank.putByte(3,nn,(int8_t) 60);
      }
      else if(order==10) {
	        bank.putByte(3,nn,(int8_t) 90);
      }
    }
    

    //printf("SIZE = %d, 60 = %8d\n",bank.getRows(),countOrder(bank,60));
    for(int nn = 0; nn < nindex; nn++){
      int index = bank_iter00[nn];
      int order = bank.getInt(3,index);
      if(order==60) bank.putByte(3,bank_iter00[nn],(int8_t)  0);
      if(order==90) bank.putByte(3,bank_iter00[nn],(int8_t) 10);
    }
    
    for(int nn = 0; nn < bank_iter40.size(); nn++){
      int index = bank_iter40[nn];
      int order = bank.getInt(3,index);
      //bank.putByte(3,bank_iter40[nn],(int8_t) 40);
      if(order==60) bank.putByte(3,bank_iter40[nn],(int8_t) 40);
      if(order==90) bank.putByte(3,bank_iter40[nn],(int8_t) 70);
    }
    
    for(int nn = 0; nn < bank_iter50.size(); nn++){
      int index = bank_iter50[nn];
      int order = bank.getInt(3,index);
      //bank.putByte(3,bank_iter50[nn],(int8_t) 50);
      if(order==60) bank.putByte(3,bank_iter50[nn],(int8_t) 50);
      if(order==90) bank.putByte(3,bank_iter50[nn],(int8_t) 80);
    }

    //printf(" >>>>> final count , 00 = %8d, 40 = %8d, 50 = %8d\n---\n",
    //    countOrder(bank,0),countOrder(bank,40),countOrder(bank,50));

    return nindex;
  }
}
