#include "ROOT/RDataFrame.hxx"
#include "ROOT/RVec.hxx"

#include "TCanvas.h"
#include "TFile.h"
#include "TH1D.h"
#include "TSystem.h"
#include "TTree.h"

#include <chrono>
#include <iostream>
#include <string>

#include <unistd.h>

enum How {
   kOptimized,
   kTreeDraw,
   kRdf,
   kRdfLambda,
};


void DrawRdf(const char *inputPath)
{
   ROOT::RDataFrame frame("clas12", inputPath);
   auto f1 = frame.Define("obs", "sqrt(px*px+py*py+pz*pz)");
   auto h = f1.Histo1D({"", "", 120, 0., 10.}, "obs");
   *h;
}


void DrawRdfLambda(const char *inputPath)
{
   ROOT::RDataFrame frame("clas12", inputPath);
   auto f1 = frame.Define("obs", [](ROOT::RVec<float> px, ROOT::RVec<float> py, ROOT::RVec<float> pz) {
      return sqrt(px*px + py*py + pz*pz); }, {"px", "py", "pz"});
   auto h = f1.Histo1D({"", "", 120, 0., 10.}, "obs");
   *h;
}


void DrawTree(TTree *tree)
{
   tree->Draw("sqrt(px*px+py*py+pz*pz)>>h1(120,0.0,10.0)");
}


void DrawOptimized(TTree *tree)
{
   int count;
   float   pid[1000];
   float px[1000];
   float py[1000];
   float pz[1000];
   float vx[1000];
   float vy[1000];
   float vz[1000];
   float vt[1000];
   float beta[1000];
   float chi2pid[1000];
   int16_t status[1000];
   Char_t  charge[1000];

   TBranch *br_count = nullptr;
   TBranch *br_px = nullptr;
   TBranch *br_py = nullptr;
   TBranch *br_pz = nullptr;

   TBranch *br_vx = nullptr;
   TBranch *br_vy = nullptr;
   TBranch *br_vz = nullptr;
   TBranch *br_vt = nullptr;

   TBranch *br_pid = nullptr;
   TBranch *br_beta = nullptr;
   TBranch *br_chi2pid = nullptr;
   TBranch *br_status = nullptr;
   TBranch *br_charge = nullptr;

   tree->SetBranchAddress("count", &count, &br_count);
   tree->SetBranchAddress("px", px, &br_px);
   tree->SetBranchAddress("py", py, &br_py);
   tree->SetBranchAddress("pz", pz, &br_pz);
   tree->SetBranchAddress("vx", vx, &br_vx);
   tree->SetBranchAddress("vy", vy, &br_vy);
   tree->SetBranchAddress("vz", vz, &br_vz);
   tree->SetBranchAddress("vt", vt, &br_vt);

   tree->SetBranchAddress("pid", pid, &br_pid);
   tree->SetBranchAddress("beta", beta, &br_beta);
   tree->SetBranchAddress("charge", charge, &br_charge);

   tree->SetBranchAddress("chi2pid", chi2pid, &br_chi2pid);
   tree->SetBranchAddress("status", status, &br_status);

   auto h = new TH1D("", "", 120, 0, 10);
   long runtime = 0;
   auto nEntries = tree->GetEntries();
   printf("drawing optimized, entries = %ld\n",nEntries);
   
   for (decltype(nEntries) entryId = 0; entryId < nEntries; ++entryId) {
      tree->LoadTree(entryId);
      auto tsStart = std::chrono::steady_clock::now();
      br_count->GetEntry(entryId);
      br_px->GetEntry(entryId);
      br_py->GetEntry(entryId);
      br_pz->GetEntry(entryId);
      br_vx->GetEntry(entryId);
      br_vy->GetEntry(entryId);
      br_vz->GetEntry(entryId);
      br_vt->GetEntry(entryId);
      br_pid->GetEntry(entryId);
      br_beta->GetEntry(entryId);
      br_chi2pid->GetEntry(entryId);
      br_status->GetEntry(entryId);


      for (int i = 0; i < count; ++i) {	
	h->Fill(
		sqrt(px[i]*px[i] + py[i]*py[i] + pz[i]*pz[i])*sqrt(vx[i]*vx[i]+vy[i]*vy[i]+vz[i]*vz[i])
		+ vt[i] + pid[i]*beta[i]*chi2pid[i]+status[i] - charge[i]);
      }
      auto tsStop = std::chrono::steady_clock::now();
      runtime += std::chrono::duration_cast<std::chrono::microseconds>(tsStop - tsStart).count();
   }
   std::cout << "Runtime algo: " << runtime << " us" << std::endl;
   new TCanvas("c1","",10,10,800,600);
   h->DrawCopy();
}


void draw(const char *pathInput, How how)
{
   auto file = TFile::Open(pathInput);
   auto tree = file->Get<TTree>("clas12");
   printf("drawing method = %d\n",how);

   auto tsStart = std::chrono::steady_clock::now();
   switch (how) {
   case kTreeDraw:
      DrawTree(tree);
      break;
   case kOptimized:
      DrawOptimized(tree);
      break;
   case kRdf:
      DrawRdf(pathInput);
      break;
   case kRdfLambda:
      DrawRdfLambda(pathInput);
      break;
   default:
      assert(false);
   }
   auto tsStop = std::chrono::steady_clock::now();
   auto runtime = std::chrono::duration_cast<std::chrono::microseconds>(tsStop - tsStart).count();
   std::cout << "Runtime: " << runtime << " us" << std::endl;
}


void Usage(const std::string &progname) {
   std::cout << progname << " -i <input> [-t(tree draw)] [-r(df)]" << std::endl;
}


int main(int argc, char **argv)
{
   std::string pathInput;
   How how = kOptimized;

   int c;
   while ((c = getopt(argc, argv, "hvi:trR")) != -1) {
      switch (c) {
      case 'h':
      case 'v':
         Usage(argv[0]);
         return 0;
      case 'i':
         pathInput = optarg;
         break;
      case 't':
         how = kTreeDraw;
         break;
      case 'r':
         how = kRdf;
         break;
      case 'R':
         how = kRdfLambda;
         break;
      default:
         fprintf(stderr, "Unknown option: -%c\n", c);
         Usage(argv[0]);
         return 1;
      }
   }

   if (pathInput.empty()) {
      Usage(argv[0]);
      return 1;
   }

   draw(pathInput.c_str(), how);
}
