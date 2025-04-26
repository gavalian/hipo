//
// Created by Maurik Holtrop on 7/19/22.
//
//pre-fix = 16.6s for 10 hipo files.
//
#include <string>
#include <iostream>
#include <chrono>
#include "RHipoDS.hxx"
#include "TCanvas.h"

using namespace ROOT;
using namespace ROOT::RDF;

std::vector<float> v_abs(std::vector<float>  &x, std::vector<float> &y, std::vector<float> &z){ std::vector<float> out;
   for(int i=0; i< x.size(); ++i){
      out.push_back(sqrt(x[i]*x[i]+y[i]*y[i]+z[i]*z[i]));
   };
   return out;
};

int main(int argc, char **argv) {
   // Very simple test of the Hipo DataFrame.
   // ROOT::EnableImplicitMT();
   int N_open = 100;
   std::chrono::nanoseconds delta_t;

   if(argc < 2){
      std::cout << "Please specify a HIPO data file on the command line. (Only one file.) \n";
      return 1;
   }else{
      std::cout << "Opening file " << argv[1] << std::endl;
   }

   auto start = std::chrono::high_resolution_clock::now();
   auto ds = std::make_unique<RHipoDS>(argv[1], N_open);
   auto cols_ds = ds->GetColumnNames();
   bool translated = ds->fColumnNameTranslation;
   auto stop = std::chrono::high_resolution_clock::now();
   auto total_events = ds->GetEntries();
   delta_t = std::chrono::duration_cast<std::chrono::nanoseconds>(stop-start);
   printf("Open file in  %6.5f ms  for %6lu events = %6.5f ns/event\n",
          delta_t.count()*1.e-6, total_events, double(delta_t.count())/total_events );

   //("/data/CLAS12/data/hipo/rec_clas_016321.evio.00001.hipo");
//   auto all_columns = ds->GetColumnNames();
//   for(int i=0; i< all_columns.size(); ++i){
//      printf("%40s  bank id: %4d  %s \n", all_columns[i].c_str(), i, ds->fColumnTypeIsVector[i] ? "vector":"scaler" );
//   }

   ds->fDebug = 5;
   auto df = RDataFrame(std::move(ds));
   auto cols_df = df.GetColumnNames();
   auto disp =
         df.Filter("ATOF_tdc_layer.size() > 0")
                     //.Display({"s","l","c","o","tdc","tot"},50,48);
               .Display({"ATOF_tdc_sector", "ATOF_tdc_layer", "ATOF_tdc_component",
                         "ATOF_tdc_order", "ATOF_tdc_TDC", "ATOF_tdc_ToT"},
                        5, 48);
   disp->Print();

}