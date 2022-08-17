#include "TFile.h"
#include "TTree.h"
#include "TTreeReader.h"

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#include <unistd.h>


void convert(const std::string &pathInput, const std::string &pathOutput, int compression, bool iofeatures) {
   auto fileInput = TFile::Open(pathInput.c_str());
   auto treeInput = fileInput->Get<TTree>("clas12");

   TTreeReader reader(treeInput);
   TTreeReaderValue<std::vector<int>> pid(reader, "pid");
   TTreeReaderValue<std::vector<float>> px(reader, "px");
   TTreeReaderValue<std::vector<float>> py(reader, "py");
   TTreeReaderValue<std::vector<float>> pz(reader, "pz");
   TTreeReaderValue<std::vector<float>> vx(reader, "vx");
   TTreeReaderValue<std::vector<float>> vy(reader, "vy");
   TTreeReaderValue<std::vector<float>> vz(reader, "vz");
   TTreeReaderValue<std::vector<float>> vt(reader, "vt");
   TTreeReaderValue<std::vector<float>> beta(reader, "beta");
   TTreeReaderValue<std::vector<float>> chi2pid(reader, "chi2pid");
   TTreeReaderValue<std::vector<char>>  charge(reader, "charge");
   TTreeReaderValue<std::vector<short>> status(reader, "status");

   auto fileOutput = new TFile(pathOutput.c_str(), "RECREATE");
   fileOutput->SetCompressionSettings(compression);
   ROOT::TIOFeatures features;
   features.Set(ROOT::Experimental::EIOFeatures::kGenerateOffsetMap);
   auto treeOutput = new TTree("clas12", "");
   if (iofeatures)
      treeOutput->SetIOFeatures(features);

   int   count;
   int   outPid[1000];
   float outPx[1000];
   float outPy[1000];
   float outPz[1000];
   float outVx[1000];
   float outVy[1000];
   float outVz[1000];
   float outVt[1000];
   float outBeta[1000];
   float outChi2pid[1000];
   char  outCharge[1000];
   short outStatus[1000];
   treeOutput->Branch("count",   &count,     "count/I",          512000);
   treeOutput->Branch("pid",     outPid,     "pid[count]/F",     512000);
   treeOutput->Branch("px",      outPx,      "px[count]/F",      512000);
   treeOutput->Branch("py",      outPy,      "py[count]/F",      512000);
   treeOutput->Branch("pz",      outPz,      "pz[count]/F",      512000);
   treeOutput->Branch("vx",      outVx,      "vx[count]/F",      512000);
   treeOutput->Branch("vy",      outVy,      "vy[count]/F",      512000);
   treeOutput->Branch("vz",      outVz,      "vz[count]/F",      512000);
   treeOutput->Branch("vt",      outVt,      "vt[count]/F",      512000);
   treeOutput->Branch("beta",    outBeta,    "beta[count]/F",    512000);
   treeOutput->Branch("chi2pid", outChi2pid, "chi2pid[count]/F", 512000);
   treeOutput->Branch("charge",  outCharge,  "charge[count]/B",  512000);
   treeOutput->Branch("status",  outStatus,  "status[count]/S",  512000);

   int nEv = 0;
   long runtime = 0;

   while(reader.Next()) {
      if (++nEv % 100000 == 0)
         std::cout << "Converted " << nEv << " events" << std::endl;

      count = pid->size();
      assert(count < 1000);
      for (int i = 0; i < count; ++i) {
         outPid[i]      = pid->at(i);
         outPx[i]       = px->at(i);
         outPy[i]       = py->at(i);
         outPz[i]       = pz->at(i);
         outVx[i]       = vx->at(i);
         outVy[i]       = vy->at(i);
         outVz[i]       = vz->at(i);
         outVt[i]       = vt->at(i);
         outBeta[i]     = beta->at(i);
         outChi2pid[i]  = chi2pid->at(i);
         outCharge[i]   = charge->at(i);
         outStatus[i]   = status->at(i);
      }
      auto tsStart = std::chrono::steady_clock::now();
      treeOutput->Fill();
      auto tsStop = std::chrono::steady_clock::now();
      runtime += std::chrono::duration_cast<std::chrono::microseconds>(tsStop - tsStart).count();
   }
   fileOutput->Write();
   fileOutput->Close();
   std::cout << "writing time : " << runtime << " usec" << std::endl; 
}


void Usage(const std::string &progname)
{
   std::cout << progname << " -i <input> -o <output> [-c compression] [-k(GenerateOffsetMap)]" << std::endl;
}


int main(int argc, char **argv) {
   std::string pathInput;
   std::string pathOutput;
   int compression = 404;
   bool generateOffsetMap = false;

   int c;
   while ((c = getopt(argc, argv, "hvi:o:c:k")) != -1) {
      switch (c) {
      case 'h':
      case 'v':
         Usage(argv[0]);
         return 0;
      case 'i':
         pathInput = optarg;
         break;
      case 'o':
         pathOutput = optarg;
         break;
      case 'c':
         compression = std::stoi(optarg);
         break;
      case 'k':
         generateOffsetMap = true;
         break;
      default:
         fprintf(stderr, "Unknown option: -%c\n", c);
         Usage(argv[0]);
         return 1;
      }
   }

   if (pathInput.empty() || pathOutput.empty()) {
      Usage(argv[0]);
      return 1;
   }

   convert(pathInput, pathOutput, compression, generateOffsetMap);
}
