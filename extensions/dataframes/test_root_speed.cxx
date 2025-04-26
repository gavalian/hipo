//
// Created by Maurik Holtrop on 7/19/22.
//
//pre-fix = 16.6s for 10 hipo files.
//
#include <string>
#include <iostream>
#include <chrono>
#include "ROOT/RDataFrame.hxx"
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
      std::cout << "Please specify a ROOT data file on the command line. \n";
      return 1;
   }else{
      std::cout << "Opening file " << argv[1] << std::endl;
   }

   auto start = std::chrono::high_resolution_clock::now();
   auto df = RDataFrame("hipotree", argv[1]);
   auto stop = std::chrono::high_resolution_clock::now();
   delta_t = std::chrono::duration_cast<std::chrono::nanoseconds>(stop-start);
   printf("Open file in  %6.5f ms\n", delta_t.count()*1.e-6);

   auto h_pid= df.Histo1D({"h_pid","Particle ID",4601,-2300,2300},"REC_Particle_pid");
   auto h_evt = df.Histo1D({"h_evt", "Event number", 1000001, 0, 1000000}, "RUN_config_event");
   auto h_px = df.Histo1D({"h_px", "P_x", 1000, 0., 12.},"REC_Particle_px");
   auto h_py = df.Histo1D({"h_py", "P_y", 1000, 0., 12.},"REC_Particle_py");
   auto h_pz = df.Histo1D({"h_pz", "P_z", 1000, 0., 12.},"REC_Particle_pz");

   // Lambda function for the absolute of a vector component set.
   auto v_abs_l = [](
         std::vector<float> &x, std::vector<float> &y, std::vector<float> &z)
   { RVec<double> out;
      for(int i=0; i< x.size(); ++i){
         out.push_back(sqrt(x[i]*x[i]+y[i]*y[i]+z[i]*z[i]));
      };
      return out;
   };

   auto h_p = df.Define("p",v_abs,{"REC_Particle_px","REC_Particle_py","REC_Particle_pz"}).Histo1D({"h_p","P (Momentum)", 1000, 0., 12.}, "p");
   //
   // Note that for the following style of DataFrame definitions, you *must* use aliasses. The original names
   // of columns in HIPO are incompatible with C++ (or Python or anything really) code direct access to these variables.
   //
   //   auto h_p = df2.Define("p","vector<float> out;for(int i=0; i< px.size(); ++i){out.push_back(sqrt(px[i]*px[i]+py[i]*py[i]+pz[i]*pz[i]));}; return out;").Histo1D({"h_p","P (Momentum)", 1000, 0., 12.}, "p");

   TCanvas* c = new TCanvas("c", "Test RHipoDS", 0, 0, 2000, 1000);
   c->Divide(2, 2);
   c->cd(1);
   // First pass through the data
   start = std::chrono::high_resolution_clock::now();
   h_p->Draw();
   stop = std::chrono::high_resolution_clock::now();
   delta_t = std::chrono::duration_cast<std::chrono::nanoseconds>(stop-start);
   double time_ns = double(delta_t.count());
   auto count_ptr = df.Count();
   long total_events = count_ptr.GetValue();
   printf("processed events = %7lu  in  %6.5f s, or %10.3f ns/event. \n", total_events, time_ns*1.e-9,
          time_ns/total_events);

}