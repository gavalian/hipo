#include "iostream"
#include "stdlib.h"
#include "writer.h"
#include "event.h"

#include "TFile.h"
#include "TTree.h"
#include "TMath.h"
#include "TVector3.h"

using namespace std;

static const int MAXPARTHIT = 1;
static const int MAXHIT = 100;


   // Declaration of leaf types
   Int_t           EventNumber;
   Int_t           PartId;
   Int_t           nPhotons;
   Int_t           nTrkHits;
   Double_t        PartVx;
   Double_t        PartVy;
   Double_t        PartVz;
   Double_t        PartCx;
   Double_t        PartCy;
   Double_t        PartCz;
   Double_t        PartP;
   Double_t        PartBeta;
   Double_t        PartTheta;
   Double_t        PartPhi;
   Int_t           AerogelLayer;
   Int_t           AerogelTile;
   Double_t        PartHitX[MAXPARTHIT];   //[nTrkHits]
   Double_t        PartHitY[MAXPARTHIT];   //[nTrkHits]
   Double_t        PartHitZ[MAXPARTHIT];   //[nTrkHits]
   Int_t           PartPixel[MAXPARTHIT];   //[nTrkHits]
   Int_t           PartPmt[MAXPARTHIT];   //[nTrkHits]
   Double_t        PartTrueHitX[MAXPARTHIT];   //[nTrkHits]
   Double_t        PartTrueHitY[MAXPARTHIT];   //[nTrkHits]
   Double_t        PartTrueHitZ[MAXPARTHIT];   //[nTrkHits]
   Int_t           nHits;
   Int_t           nBkg;
   Int_t           nLost;
   Int_t           nAbs;
   Int_t           nKilled;
   Int_t           nFail;
   Double_t        EtaC[MAXHIT];   //[nHits]
   Double_t        PhiC[MAXHIT];   //[nHits]
   Double_t        TrueEtaC[MAXHIT];   //[nHits]
   Double_t        TruePhiC[MAXHIT];   //[nHits]
   Double_t        EmiX[MAXHIT];   //[nHits]
   Double_t        EmiY[MAXHIT];   //[nHits]
   Double_t        EmiZ[MAXHIT];   //[nHits]
   Double_t        EmiD[MAXHIT];   //[nHits]
   Double_t        MirrorA1LX[MAXHIT];   //[nHits]
   Double_t        MirrorA1LY[MAXHIT];   //[nHits]
   Double_t        MirrorA1LZ[MAXHIT];   //[nHits]
   Int_t           NhitsMirrorA1L[MAXHIT];   //[nHits]
   Double_t        MirrorA2LX[MAXHIT];   //[nHits]
   Double_t        MirrorA2LY[MAXHIT];   //[nHits]
   Double_t        MirrorA2LZ[MAXHIT];   //[nHits]
   Int_t           NhitsMirrorA2L[MAXHIT];   //[nHits]
   Double_t        MirrorA1RX[MAXHIT];   //[nHits]
   Double_t        MirrorA1RY[MAXHIT];   //[nHits]
   Double_t        MirrorA1RZ[MAXHIT];   //[nHits]
   Int_t           NhitsMirrorA1R[MAXHIT];   //[nHits]
   Double_t        MirrorA2RX[MAXHIT];   //[nHits]
   Double_t        MirrorA2RY[MAXHIT];   //[nHits]
   Double_t        MirrorA2RZ[MAXHIT];   //[nHits]
   Int_t           NhitsMirrorA2R[MAXHIT];   //[nHits]
   Double_t        MirrorA3X[MAXHIT];   //[nHits]
   Double_t        MirrorA3Y[MAXHIT];   //[nHits]
   Double_t        MirrorA3Z[MAXHIT];   //[nHits]
   Int_t           NhitsMirrorA3[MAXHIT];   //[nHits]
   Double_t        MirrorB1X[MAXHIT];   //[nHits]
   Double_t        MirrorB1Y[MAXHIT];   //[nHits]
   Double_t        MirrorB1Z[MAXHIT];   //[nHits]
   Int_t           NhitsMirrorB1[MAXHIT];   //[nHits]
   Double_t        MirrorB2X[MAXHIT];   //[nHits]
   Double_t        MirrorB2Y[MAXHIT];   //[nHits]
   Double_t        MirrorB2Z[MAXHIT];   //[nHits]
   Int_t           NhitsMirrorB2[MAXHIT];   //[nHits]
   Double_t        SpheMirrorS1X[MAXHIT];   //[nHits]
   Double_t        SpheMirrorS1Y[MAXHIT];   //[nHits]
   Double_t        SpheMirrorS1Z[MAXHIT];   //[nHits]
   Int_t           NhitsSpheMirrorS1[MAXHIT];   //[nHits]
   Double_t        SpheMirrorS2X[MAXHIT];   //[nHits]
   Double_t        SpheMirrorS2Y[MAXHIT];   //[nHits]
   Double_t        SpheMirrorS2Z[MAXHIT];   //[nHits]
   Int_t           NhitsSpheMirrorS2[MAXHIT];   //[nHits]
   Double_t        SpheMirrorS2cX[MAXHIT];   //[nHits]
   Double_t        SpheMirrorS2cY[MAXHIT];   //[nHits]
   Double_t        SpheMirrorS2cZ[MAXHIT];   //[nHits]
   Int_t           NhitsSpheMirrorS2c[MAXHIT];   //[nHits]
   Double_t        SpheMirrorS3X[MAXHIT];   //[nHits]
   Double_t        SpheMirrorS3Y[MAXHIT];   //[nHits]
   Double_t        SpheMirrorS3Z[MAXHIT];   //[nHits]
   Int_t           NhitsSpheMirrorS3[MAXHIT];   //[nHits]
   Double_t        SpheMirrorS3cX[MAXHIT];   //[nHits]
   Double_t        SpheMirrorS3cY[MAXHIT];   //[nHits]
   Double_t        SpheMirrorS3cZ[MAXHIT];   //[nHits]
   Int_t           NhitsSpheMirrorS3c[MAXHIT];   //[nHits]
   Double_t        SpheMirrorS4X[MAXHIT];   //[nHits]
   Double_t        SpheMirrorS4Y[MAXHIT];   //[nHits]
   Double_t        SpheMirrorS4Z[MAXHIT];   //[nHits]
   Int_t           NhitsSpheMirrorS4[MAXHIT];   //[nHits]
   Double_t        SpheMirrorS4cX[MAXHIT];   //[nHits]
   Double_t        SpheMirrorS4cY[MAXHIT];   //[nHits]
   Double_t        SpheMirrorS4cZ[MAXHIT];   //[nHits]
   Int_t           NhitsSpheMirrorS4c[MAXHIT];   //[nHits]
   Double_t        SpheMirrorS5X[MAXHIT];   //[nHits]
   Double_t        SpheMirrorS5Y[MAXHIT];   //[nHits]
   Double_t        SpheMirrorS5Z[MAXHIT];   //[nHits]
   Int_t           NhitsSpheMirrorS5[MAXHIT];   //[nHits]
   Double_t        SpheMirrorS5cX[MAXHIT];   //[nHits]
   Double_t        SpheMirrorS5cY[MAXHIT];   //[nHits]
   Double_t        SpheMirrorS5cZ[MAXHIT];   //[nHits]
   Int_t           NhitsSpheMirrorS5c[MAXHIT];   //[nHits]
   Double_t        SpheMirrorS6X[MAXHIT];   //[nHits]
   Double_t        SpheMirrorS6Y[MAXHIT];   //[nHits]
   Double_t        SpheMirrorS6Z[MAXHIT];   //[nHits]
   Int_t           NhitsSpheMirrorS6[MAXHIT];   //[nHits]
   Double_t        HitX[MAXHIT];   //[nHits]
   Double_t        HitY[MAXHIT];   //[nHits]
   Double_t        HitZ[MAXHIT];   //[nHits]
   Int_t           Pixel[MAXHIT];   //[nHits]
   Int_t           Pmt[MAXHIT];   //[nHits]
   Double_t        PathT[MAXHIT];   //[nHits]
   Double_t        PathL[MAXHIT];   //[nHits]
   Double_t        TrueHitX[MAXHIT];   //[nHits]
   Double_t        TrueHitY[MAXHIT];   //[nHits]
   Double_t        TrueHitZ[MAXHIT];   //[nHits]
   Double_t        TrueTime[MAXHIT];   //[nHits]
   Int_t           Nsteps[MAXHIT];   //[nHits]
   Int_t           Nrefl[MAXHIT];   //[nHits]
   Int_t           Nrefr[MAXHIT];   //[nHits]
   ULong64_t       TruePathLayer[MAXHIT];   //[nHits]
   ULong64_t       TruePathComp[MAXHIT];   //[nHits]
   ULong64_t       PathLayer[MAXHIT];   //[nHits]
   ULong64_t       PathComp[MAXHIT];   //[nHits]
   Double_t        Chi2[MAXHIT];   //[nHits]
   Double_t        DB[MAXHIT];   //[nHits]

   // List of branches
   TBranch        *b_EventNumber;   //!
   TBranch        *b_PartId;   //!
   TBranch        *b_nPhotons;   //!
   TBranch        *b_nTrkHits;   //!
   TBranch        *b_PartVx;   //!
   TBranch        *b_PartVy;   //!
   TBranch        *b_PartVz;   //!
   TBranch        *b_PartCx;   //!
   TBranch        *b_PartCy;   //!
   TBranch        *b_PartCz;   //!
   TBranch        *b_PartP;   //!
   TBranch        *b_PartBeta;   //!
   TBranch        *b_PartTheta;   //!
   TBranch        *b_PartPhi;   //!
   TBranch        *b_AerogelLayer;   //!
   TBranch        *b_AerogelTile;   //!
   TBranch        *b_PartHitX;   //!
   TBranch        *b_PartHitY;   //!
   TBranch        *b_PartHitZ;   //!
   TBranch        *b_PartPixel;   //!
   TBranch        *b_PartPmt;   //!
   TBranch        *b_PartTrueHitX;   //!
   TBranch        *b_PartTrueHitY;   //!
   TBranch        *b_PartTrueHitZ;   //!
   TBranch        *b_nHits;   //!
   TBranch        *b_nBkg;   //!
   TBranch        *b_nLost;   //!
   TBranch        *b_nAbs;   //!
   TBranch        *b_nKilled;   //!
   TBranch        *b_nFail;   //!
   TBranch        *b_EtaC;   //!
   TBranch        *b_PhiC;   //!
   TBranch        *b_TrueEtaC;   //!
   TBranch        *b_TruePhiC;   //!
   TBranch        *b_EmiX;   //!
   TBranch        *b_EmiY;   //!
   TBranch        *b_EmiZ;   //!
   TBranch        *b_EmiD;   //!
   TBranch        *b_MirrorA1LX;   //!
   TBranch        *b_MirrorA1LY;   //!
   TBranch        *b_MirrorA1LZ;   //!
   TBranch        *b_NhitsMirrorA1L;   //!
   TBranch        *b_MirrorA2LX;   //!
   TBranch        *b_MirrorA2LY;   //!
   TBranch        *b_MirrorA2LZ;   //!
   TBranch        *b_NhitsMirrorA2L;   //!
   TBranch        *b_MirrorA1RX;   //!
   TBranch        *b_MirrorA1RY;   //!
   TBranch        *b_MirrorA1RZ;   //!
   TBranch        *b_NhitsMirrorA1R;   //!
   TBranch        *b_MirrorA2RX;   //!
   TBranch        *b_MirrorA2RY;   //!
   TBranch        *b_MirrorA2RZ;   //!
   TBranch        *b_NhitsMirrorA2R;   //!
   TBranch        *b_MirrorA3X;   //!
   TBranch        *b_MirrorA3Y;   //!
   TBranch        *b_MirrorA3Z;   //!
   TBranch        *b_NhitsMirrorA3;   //!
   TBranch        *b_MirrorB1X;   //!
   TBranch        *b_MirrorB1Y;   //!
   TBranch        *b_MirrorB1Z;   //!
   TBranch        *b_NhitsMirrorB1;   //!
   TBranch        *b_MirrorB2X;   //!
   TBranch        *b_MirrorB2Y;   //!
   TBranch        *b_MirrorB2Z;   //!
   TBranch        *b_NhitsMirrorB2;   //!
   TBranch        *b_SpheMirrorS1X;   //!
   TBranch        *b_SpheMirrorS1Y;   //!
   TBranch        *b_SpheMirrorS1Z;   //!
   TBranch        *b_NhitsSpheMirrorS1;   //!
   TBranch        *b_SpheMirrorS2X;   //!
   TBranch        *b_SpheMirrorS2Y;   //!
   TBranch        *b_SpheMirrorS2Z;   //!
   TBranch        *b_NhitsSpheMirrorS2;   //!
   TBranch        *b_SpheMirrorS2cX;   //!
   TBranch        *b_SpheMirrorS2cY;   //!
   TBranch        *b_SpheMirrorS2cZ;   //!
   TBranch        *b_NhitsSpheMirrorS2c;   //!
   TBranch        *b_SpheMirrorS3X;   //!
   TBranch        *b_SpheMirrorS3Y;   //!
   TBranch        *b_SpheMirrorS3Z;   //!
   TBranch        *b_NhitsSpheMirrorS3;   //!
   TBranch        *b_SpheMirrorS3cX;   //!
   TBranch        *b_SpheMirrorS3cY;   //!
   TBranch        *b_SpheMirrorS3cZ;   //!
   TBranch        *b_NhitsSpheMirrorS3c;   //!
   TBranch        *b_SpheMirrorS4X;   //!
   TBranch        *b_SpheMirrorS4Y;   //!
   TBranch        *b_SpheMirrorS4Z;   //!
   TBranch        *b_NhitsSpheMirrorS4;   //!
   TBranch        *b_SpheMirrorS4cX;   //!
   TBranch        *b_SpheMirrorS4cY;   //!
   TBranch        *b_SpheMirrorS4cZ;   //!
   TBranch        *b_NhitsSpheMirrorS4c;   //!
   TBranch        *b_SpheMirrorS5X;   //!
   TBranch        *b_SpheMirrorS5Y;   //!
   TBranch        *b_SpheMirrorS5Z;   //!
   TBranch        *b_NhitsSpheMirrorS5;   //!
   TBranch        *b_SpheMirrorS5cX;   //!
   TBranch        *b_SpheMirrorS5cY;   //!
   TBranch        *b_SpheMirrorS5cZ;   //!
   TBranch        *b_NhitsSpheMirrorS5c;   //!
   TBranch        *b_SpheMirrorS6X;   //!
   TBranch        *b_SpheMirrorS6Y;   //!
   TBranch        *b_SpheMirrorS6Z;   //!
   TBranch        *b_NhitsSpheMirrorS6;   //!
   TBranch        *b_HitX;   //!
   TBranch        *b_HitY;   //!
   TBranch        *b_HitZ;   //!
   TBranch        *b_Pixel;   //!
   TBranch        *b_Pmt;   //!
   TBranch        *b_PathT;   //!
   TBranch        *b_PathL;   //!
   TBranch        *b_TrueHitX;   //!
   TBranch        *b_TrueHitY;   //!
   TBranch        *b_TrueHitZ;   //!
   TBranch        *b_TrueTime;   //!
   TBranch        *b_Nsteps;   //!
   TBranch        *b_Nrefl;   //!
   TBranch        *b_Nrefr;   //!
   TBranch        *b_TruePathLayer;   //!
   TBranch        *b_TruePathComp;   //!
   TBranch        *b_PathLayer;   //!
   TBranch        *b_PathComp;   //!
   TBranch        *b_Chi2;   //!
   TBranch        *b_DB;   //!


void rotate(TVector3 &vec, float angle1, float angle2);

int main(int argc, char** argv){
    cout << " Convert simulation ROOT file to HIPO file" << endl;

    char inputFile[512];
    char outputFile[512];
    if(argc > 1){
        sprintf(inputFile, "%s",argv[1]);
        sprintf(outputFile, "%s.hipo", argv[1]);
    } else{
        cout << "WARNING:Input file is required! " << endl;
        return 1;
    }
    cout << "Converting '"<< inputFile << "' ROOT file to '" << outputFile << "' HIPO file!" << endl;
    
    TFile f1(inputFile);
    TTree* tree = (TTree*) f1.Get("RichTree");


    tree->SetBranchAddress("EventNumber", &EventNumber, &b_EventNumber);
    tree->SetBranchAddress("PartId", &PartId, &b_PartId);
    tree->SetBranchAddress("nPhotons", &nPhotons, &b_nPhotons);
    tree->SetBranchAddress("nTrkHits", &nTrkHits, &b_nTrkHits);
    tree->SetBranchAddress("PartVx", &PartVx, &b_PartVx);
    tree->SetBranchAddress("PartVy", &PartVy, &b_PartVy);
    tree->SetBranchAddress("PartVz", &PartVz, &b_PartVz);
    tree->SetBranchAddress("PartCx", &PartCx, &b_PartCx);
    tree->SetBranchAddress("PartCy", &PartCy, &b_PartCy);
    tree->SetBranchAddress("PartCz", &PartCz, &b_PartCz);
    tree->SetBranchAddress("PartP", &PartP, &b_PartP);
    tree->SetBranchAddress("PartBeta", &PartBeta, &b_PartBeta);
    tree->SetBranchAddress("PartTheta", &PartTheta, &b_PartTheta);
    tree->SetBranchAddress("PartPhi", &PartPhi, &b_PartPhi);
    tree->SetBranchAddress("AerogelLayer", &AerogelLayer, &b_AerogelLayer);
    tree->SetBranchAddress("AerogelTile", &AerogelTile, &b_AerogelTile);
    tree->SetBranchAddress("PartHitX", PartHitX, &b_PartHitX);
    tree->SetBranchAddress("PartHitY", PartHitY, &b_PartHitY);
    tree->SetBranchAddress("PartHitZ", PartHitZ, &b_PartHitZ);
    tree->SetBranchAddress("PartPixel", PartPixel, &b_PartPixel);
    tree->SetBranchAddress("PartPmt", PartPmt, &b_PartPmt);
    tree->SetBranchAddress("PartTrueHitX", PartTrueHitX, &b_PartTrueHitX);
    tree->SetBranchAddress("PartTrueHitY", PartTrueHitY, &b_PartTrueHitY);
    tree->SetBranchAddress("PartTrueHitZ", PartTrueHitZ, &b_PartTrueHitZ);
    tree->SetBranchAddress("nHits", &nHits, &b_nHits);
    tree->SetBranchAddress("nBkg", &nBkg, &b_nBkg);
    tree->SetBranchAddress("nLost", &nLost, &b_nLost);
    tree->SetBranchAddress("nAbs", &nAbs, &b_nAbs);
    tree->SetBranchAddress("nKilled", &nKilled, &b_nKilled);
    tree->SetBranchAddress("nFail", &nFail, &b_nFail);
    tree->SetBranchAddress("EtaC", EtaC, &b_EtaC);
    tree->SetBranchAddress("PhiC", PhiC, &b_PhiC);
    tree->SetBranchAddress("TrueEtaC", TrueEtaC, &b_TrueEtaC);
    tree->SetBranchAddress("TruePhiC", TruePhiC, &b_TruePhiC);
    tree->SetBranchAddress("EmiX", EmiX, &b_EmiX);
    tree->SetBranchAddress("EmiY", EmiY, &b_EmiY);
    tree->SetBranchAddress("EmiZ", EmiZ, &b_EmiZ);
    tree->SetBranchAddress("EmiD", EmiD, &b_EmiD);
    tree->SetBranchAddress("MirrorA1LX", MirrorA1LX, &b_MirrorA1LX);
    tree->SetBranchAddress("MirrorA1LY", MirrorA1LY, &b_MirrorA1LY);
    tree->SetBranchAddress("MirrorA1LZ", MirrorA1LZ, &b_MirrorA1LZ);
    tree->SetBranchAddress("NhitsMirrorA1L", NhitsMirrorA1L, &b_NhitsMirrorA1L);
    tree->SetBranchAddress("MirrorA2LX", MirrorA2LX, &b_MirrorA2LX);
    tree->SetBranchAddress("MirrorA2LY", MirrorA2LY, &b_MirrorA2LY);
    tree->SetBranchAddress("MirrorA2LZ", MirrorA2LZ, &b_MirrorA2LZ);
    tree->SetBranchAddress("NhitsMirrorA2L", NhitsMirrorA2L, &b_NhitsMirrorA2L);
    tree->SetBranchAddress("MirrorA1RX", MirrorA1RX, &b_MirrorA1RX);
    tree->SetBranchAddress("MirrorA1RY", MirrorA1RY, &b_MirrorA1RY);
    tree->SetBranchAddress("MirrorA1RZ", MirrorA1RZ, &b_MirrorA1RZ);
    tree->SetBranchAddress("NhitsMirrorA1R", NhitsMirrorA1R, &b_NhitsMirrorA1R);
    tree->SetBranchAddress("MirrorA2RX", MirrorA2RX, &b_MirrorA2RX);
    tree->SetBranchAddress("MirrorA2RY", MirrorA2RY, &b_MirrorA2RY);
    tree->SetBranchAddress("MirrorA2RZ", MirrorA2RZ, &b_MirrorA2RZ);
    tree->SetBranchAddress("NhitsMirrorA2R", NhitsMirrorA2R, &b_NhitsMirrorA2R);
    tree->SetBranchAddress("MirrorA3X", MirrorA3X, &b_MirrorA3X);
    tree->SetBranchAddress("MirrorA3Y", MirrorA3Y, &b_MirrorA3Y);
    tree->SetBranchAddress("MirrorA3Z", MirrorA3Z, &b_MirrorA3Z);
    tree->SetBranchAddress("NhitsMirrorA3", NhitsMirrorA3, &b_NhitsMirrorA3);
    tree->SetBranchAddress("MirrorB1X", MirrorB1X, &b_MirrorB1X);
    tree->SetBranchAddress("MirrorB1Y", MirrorB1Y, &b_MirrorB1Y);
    tree->SetBranchAddress("MirrorB1Z", MirrorB1Z, &b_MirrorB1Z);
    tree->SetBranchAddress("NhitsMirrorB1", NhitsMirrorB1, &b_NhitsMirrorB1);
    tree->SetBranchAddress("MirrorB2X", MirrorB2X, &b_MirrorB2X);
    tree->SetBranchAddress("MirrorB2Y", MirrorB2Y, &b_MirrorB2Y);
    tree->SetBranchAddress("MirrorB2Z", MirrorB2Z, &b_MirrorB2Z);
    tree->SetBranchAddress("NhitsMirrorB2", NhitsMirrorB2, &b_NhitsMirrorB2);
    tree->SetBranchAddress("SpheMirrorS1X", SpheMirrorS1X, &b_SpheMirrorS1X);
    tree->SetBranchAddress("SpheMirrorS1Y", SpheMirrorS1Y, &b_SpheMirrorS1Y);
    tree->SetBranchAddress("SpheMirrorS1Z", SpheMirrorS1Z, &b_SpheMirrorS1Z);
    tree->SetBranchAddress("NhitsSpheMirrorS1", NhitsSpheMirrorS1, &b_NhitsSpheMirrorS1);
    tree->SetBranchAddress("SpheMirrorS2X", SpheMirrorS2X, &b_SpheMirrorS2X);
    tree->SetBranchAddress("SpheMirrorS2Y", SpheMirrorS2Y, &b_SpheMirrorS2Y);
    tree->SetBranchAddress("SpheMirrorS2Z", SpheMirrorS2Z, &b_SpheMirrorS2Z);
    tree->SetBranchAddress("NhitsSpheMirrorS2", NhitsSpheMirrorS2, &b_NhitsSpheMirrorS2);
    tree->SetBranchAddress("SpheMirrorS2cX", SpheMirrorS2cX, &b_SpheMirrorS2cX);
    tree->SetBranchAddress("SpheMirrorS2cY", SpheMirrorS2cY, &b_SpheMirrorS2cY);
    tree->SetBranchAddress("SpheMirrorS2cZ", SpheMirrorS2cZ, &b_SpheMirrorS2cZ);
    tree->SetBranchAddress("NhitsSpheMirrorS2c", NhitsSpheMirrorS2c, &b_NhitsSpheMirrorS2c);
    tree->SetBranchAddress("SpheMirrorS3X", SpheMirrorS3X, &b_SpheMirrorS3X);
    tree->SetBranchAddress("SpheMirrorS3Y", SpheMirrorS3Y, &b_SpheMirrorS3Y);
    tree->SetBranchAddress("SpheMirrorS3Z", SpheMirrorS3Z, &b_SpheMirrorS3Z);
    tree->SetBranchAddress("NhitsSpheMirrorS3", NhitsSpheMirrorS3, &b_NhitsSpheMirrorS3);
    tree->SetBranchAddress("SpheMirrorS3cX", SpheMirrorS3cX, &b_SpheMirrorS3cX);
    tree->SetBranchAddress("SpheMirrorS3cY", SpheMirrorS3cY, &b_SpheMirrorS3cY);
    tree->SetBranchAddress("SpheMirrorS3cZ", SpheMirrorS3cZ, &b_SpheMirrorS3cZ);
    tree->SetBranchAddress("NhitsSpheMirrorS3c", NhitsSpheMirrorS3c, &b_NhitsSpheMirrorS3c);
    tree->SetBranchAddress("SpheMirrorS4X", SpheMirrorS4X, &b_SpheMirrorS4X);
    tree->SetBranchAddress("SpheMirrorS4Y", SpheMirrorS4Y, &b_SpheMirrorS4Y);
    tree->SetBranchAddress("SpheMirrorS4Z", SpheMirrorS4Z, &b_SpheMirrorS4Z);
    tree->SetBranchAddress("NhitsSpheMirrorS4", NhitsSpheMirrorS4, &b_NhitsSpheMirrorS4);
    tree->SetBranchAddress("SpheMirrorS4cX", SpheMirrorS4cX, &b_SpheMirrorS4cX);
    tree->SetBranchAddress("SpheMirrorS4cY", SpheMirrorS4cY, &b_SpheMirrorS4cY);
    tree->SetBranchAddress("SpheMirrorS4cZ", SpheMirrorS4cZ, &b_SpheMirrorS4cZ);
    tree->SetBranchAddress("NhitsSpheMirrorS4c", NhitsSpheMirrorS4c, &b_NhitsSpheMirrorS4c);
    tree->SetBranchAddress("SpheMirrorS5X", SpheMirrorS5X, &b_SpheMirrorS5X);
    tree->SetBranchAddress("SpheMirrorS5Y", SpheMirrorS5Y, &b_SpheMirrorS5Y);
    tree->SetBranchAddress("SpheMirrorS5Z", SpheMirrorS5Z, &b_SpheMirrorS5Z);
    tree->SetBranchAddress("NhitsSpheMirrorS5", NhitsSpheMirrorS5, &b_NhitsSpheMirrorS5);
    tree->SetBranchAddress("SpheMirrorS5cX", SpheMirrorS5cX, &b_SpheMirrorS5cX);
    tree->SetBranchAddress("SpheMirrorS5cY", SpheMirrorS5cY, &b_SpheMirrorS5cY);
    tree->SetBranchAddress("SpheMirrorS5cZ", SpheMirrorS5cZ, &b_SpheMirrorS5cZ);
    tree->SetBranchAddress("NhitsSpheMirrorS5c", NhitsSpheMirrorS5c, &b_NhitsSpheMirrorS5c);
    tree->SetBranchAddress("SpheMirrorS6X", SpheMirrorS6X, &b_SpheMirrorS6X);
    tree->SetBranchAddress("SpheMirrorS6Y", SpheMirrorS6Y, &b_SpheMirrorS6Y);
    tree->SetBranchAddress("SpheMirrorS6Z", SpheMirrorS6Z, &b_SpheMirrorS6Z);
    tree->SetBranchAddress("NhitsSpheMirrorS6", NhitsSpheMirrorS6, &b_NhitsSpheMirrorS6);
    tree->SetBranchAddress("HitX", HitX, &b_HitX);
    tree->SetBranchAddress("HitY", HitY, &b_HitY);
    tree->SetBranchAddress("HitZ", HitZ, &b_HitZ);
    tree->SetBranchAddress("Pixel", Pixel, &b_Pixel);
    tree->SetBranchAddress("Pmt", Pmt, &b_Pmt);
    tree->SetBranchAddress("PathT", PathT, &b_PathT);
    tree->SetBranchAddress("PathL", PathL, &b_PathL);
    tree->SetBranchAddress("TrueHitX", TrueHitX, &b_TrueHitX);
    tree->SetBranchAddress("TrueHitY", TrueHitY, &b_TrueHitY);
    tree->SetBranchAddress("TrueHitZ", TrueHitZ, &b_TrueHitZ);
    tree->SetBranchAddress("TrueTime", TrueTime, &b_TrueTime);
    tree->SetBranchAddress("Nsteps", Nsteps, &b_Nsteps);
    tree->SetBranchAddress("Nrefl", Nrefl, &b_Nrefl);
    tree->SetBranchAddress("Nrefr", Nrefr, &b_Nrefr);
    tree->SetBranchAddress("TruePathLayer", TruePathLayer, &b_TruePathLayer);
    tree->SetBranchAddress("TruePathComp", TruePathComp, &b_TruePathComp);
    tree->SetBranchAddress("PathLayer", PathLayer, &b_PathLayer);
    tree->SetBranchAddress("PathComp", PathComp, &b_PathComp);
    tree->SetBranchAddress("Chi2", Chi2, &b_Chi2);
    tree->SetBranchAddress("DB", DB, &b_DB);


// Create HIPO bank schemas
    hipo::schema schemaParticles("richx::particle", 21800, 42);
    hipo::schema schemaHits( "richx::hits", 21800, 40);
    
// Parse HIPO bank data
    schemaParticles.parse("pid/S, px/F, py/F, pz/F, x/F, y/F, z/F, cx/F, cy/F, cz/F, status/S");
    schemaHits.parse("index/S,pindex/S,sector/B,layer/S,component/S,time/F,energy/F,x/F,y/F,z/F,status/S");

// Create HIPO writer and register schemas
    hipo::writer writer;
    writer.getDictionary().addSchema(schemaParticles);
    writer.getDictionary().addSchema(schemaHits);
    writer.open(outputFile);
    
    hipo::event *outEvent = new hipo::event();



// Additional HIPO variables 
    float px, py, pz;

    int nentries = tree->GetEntries();
    //nentries = 9250;
    
    for(int i=0; i<nentries; i++){
        tree->GetEntry(i);

	//cout << "Starting entry " << i << "  nhits=" << nHits << endl;

        px = PartP * PartCx;
        py = PartP * PartCy;
        pz = PartP * PartCz;

        TVector3 moment(px, py, pz);
        TVector3 coses(PartCx, PartCy, PartCz);

	/* emission point from the first photon */
	TVector3 emit;
	for (int h=0; h<nHits; h++) {
	  emit.SetXYZ(EmiX[h], EmiY[h], EmiZ[h]);
	  break;
	}



        rotate(moment, -90, 25);
        rotate(coses, -90, 25);
        rotate(emit, -90, 25);


        outEvent->reset();



        // Fill particle bank
        hipo::bank partBank(schemaParticles, 1);
        partBank.putInt("pid", 0, (short)PartId);
        partBank.putFloat("px", 0, (float)moment.X());
        partBank.putFloat("py", 0, (float)moment.Y());
        partBank.putFloat("pz", 0, (float)moment.Z());
        partBank.putFloat("x", 0, (float)emit.X());
        partBank.putFloat("y", 0, (float)emit.Y());
        partBank.putFloat("z", 0, (float)emit.Z());
        partBank.putFloat("cx", 0, (float)coses.X());
        partBank.putFloat("cy", 0, (float)coses.Y());
        partBank.putFloat("cz", 0, (float)coses.Z());
        partBank.putInt("status", 0, (short)1);
        outEvent->addStructure(partBank);

	int pindex = 0;
	int sector = 4;
	int status;
	float time, energy;

	int nWrittenHits = nHits;
	//nWrittenHits = 1;//WRITING ONLY ONE HIT 

	hipo::bank hitBank(schemaHits, nWrittenHits);
	for (int h=0; h<nWrittenHits; h++) {

	  status = 1;
	  time = PathT[h];
	  energy = 60;//duration for single photon hits

	  TVector3 hit(HitX[h], HitY[h], HitZ[h]);
	  rotate(hit, -90, 25);

	  // Fill hits bank
	  hitBank.putInt("index", h, (short)h);
	  hitBank.putInt("pindex", h, (short)pindex);
	  hitBank.putInt("sector", h, (bool)sector);
	  hitBank.putInt("layer", h, (short)AerogelLayer);
	  hitBank.putInt("component", h, (short)AerogelTile);
	  hitBank.putFloat("time", h, (float)time);
	  hitBank.putFloat("energy", h, (float)energy);
	  hitBank.putFloat("x", h, (float)hit.X());
	  hitBank.putFloat("y", h, (float)hit.Y());
	  hitBank.putFloat("z", h, (float)hit.Z());
	  hitBank.putInt("status", h, (short)status);
    }

	outEvent->addStructure(hitBank);

	  
	//}

	//cout << "   end event npart=" << partBank.getRows() << "  nhits=" << hitBank.getRows() << endl;

        writer.addEvent(*outEvent);
    }

    writer.close();

    return 0;
}

void rotate(TVector3 &vec, float angle1, float angle2){
    // Rotation angles
    angle1 = angle1*TMath::DegToRad();
    angle2 = angle2*TMath::DegToRad();

    vec.RotateZ(angle1);
    vec.RotateY(angle2);
}
