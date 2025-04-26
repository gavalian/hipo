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

ROOT::RVec<float> v_abs(ROOT::RVec<float>  &x, ROOT::RVec<float> &y, ROOT::RVec<float> &z){ ROOT::RVec<float> out;
 return sqrt(x*x+y*y+z*z);
};

int test_hipo_speed(const std::string& filename) {
   // Very simple test of the Hipo DataFrame.
  ROOT::EnableImplicitMT(4);
   int N_open = 100;
   std::chrono::nanoseconds delta_t;

   std::cout << "Opening file " << filename << std::endl;


   ///For speed tests can use same file many times
   std::vector<std::string> files = {filename,filename,filename,filename,filename,filename,filename,filename,filename};

   auto start = std::chrono::high_resolution_clock::now();
   // auto ds = std::make_unique<RHipoDS>(filename, N_open);
   auto ds = std::make_unique<RHipoDS>(files, N_open);
   bool translated = ds->fColumnNameTranslation;
   auto stop = std::chrono::high_resolution_clock::now();
   delta_t = std::chrono::duration_cast<std::chrono::nanoseconds>(stop-start);
   printf("Open file in  %6.5f ms\n", delta_t.count()*1.e-6);

   auto total_events = ds->GetEntries();
   auto df = RDataFrame(std::move(ds));
   auto h_pid= df.Histo1D({"h_pid","Particle ID",4601,-2300,2300},"REC_Particle_pid");
   auto h_evt = df.Histo1D({"h_evt", "Event number", 1000001, 0, 1000000}, "RUN_config_event");
   auto h_px = df.Histo1D({"h_px", "P_x", 1000, 0., 12.},"REC_Particle_px");
   auto h_py = df.Histo1D({"h_py", "P_y", 1000, 0., 12.},"REC_Particle_py");
   auto h_pz = df.Histo1D({"h_pz", "P_z", 1000, 0., 12.},"REC_Particle_pz");

    auto h_p = df.Define("p",v_abs,{"REC_Particle_px","REC_Particle_py","REC_Particle_pz"}).Histo1D({"h_p","P (Momentum)", 1000, 0., 12.}, "p");
   //
   // Note that for the following style of DataFrame definitions, you *must* use aliasses. The original names
   // of columns in HIPO are incompatible with C++ (or Python or anything really) code direct access to these variables.

   TCanvas* c = new TCanvas("c", "Test RHipoDS", 0, 0, 2000, 1000);
   c->Divide(2, 2);
   c->cd(1);
   // First pass through the data
   start = std::chrono::high_resolution_clock::now();
   h_p->DrawCopy();
   c->cd(2);
   h_px->DrawCopy();
   c->cd(3);
   h_py->DrawCopy();
   c->cd(4);
   h_pz->DrawCopy();
   
    stop = std::chrono::high_resolution_clock::now();
   delta_t = std::chrono::duration_cast<std::chrono::nanoseconds>(stop-start);
   double time_ns = double(delta_t.count());
   printf("processed events = %7lu  in  %6.5f s, or %10.3f ns/event. \n", total_events, time_ns*1.e-9, time_ns/total_events);

   return 1;
}
