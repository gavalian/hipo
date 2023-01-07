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
        if(vec[index]>threshold) return 1;
        return 0;
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
}
