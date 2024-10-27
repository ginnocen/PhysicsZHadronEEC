// ===================================================
// Script Based on Yi Chen Reducer script
// Yi Chen (Vanderbilt) Yi.Chen@cern.ch
// https://github.com/FHead/PhysicsZHadronEEC
// ===================================================

#include <iostream>
using namespace std;

#include "TFile.h"
#include "TTree.h"
#include "TLorentzVector.h"

#include "CommonFunctions.h"
#include "CommandLine.h"
#include "ProgressBar.h"
#include "Messenger.h"

#include "tnp_weight.h"
#include "trackingEfficiency2017pp.h"
#include "trackingEfficiency2018PbPb.h"
#include "trackingEfficiency2023PbPb.h"
#include "TrackResidualCorrector.h"

int main(int argc, char *argv[]);
double GetHFSum(PFTreeMessenger *M);
double GetGenHFSum(GenParticleTreeMessenger *M, int SubEvent = -1);

int main(int argc, char *argv[])
{
   string VersionString = "V8";
   
   CommandLine CL(argc, argv);

   vector<string> InputFileNames = CL.GetStringVector("Input");
   string OutputFileName = CL.Get("Output");

   //bool DoGenLevel                    = CL.GetBool("DoGenLevel", true);
   int Year                           = CL.GetInt("Year", 2023);
   double MinDzeroPT                  = CL.GetDouble("MinDzeroPT", 0.);
   double MinTrackPT                  = CL.GetDouble("MinTrackPT", 1);
   double Fraction                    = CL.GetDouble("Fraction", 1.00);
   bool IsData                        = CL.GetBool("IsData", false);
   string PFTreeName                  = "pfcandAnalyzer/pfTree";
   PFTreeName                         = CL.Get("PFTree", PFTreeName);
   
   TFile OutputFile(OutputFileName.c_str(), "RECREATE");
   TTree Tree("Tree", Form("Tree for UPC Dzero analysis (%s)", VersionString.c_str()));
   TTree InfoTree("InfoTree", "Information");

   DzeroUPCTreeMessenger MDzeroUPC;
   MDzeroUPC.SetBranch(&Tree);

   for(string InputFileName : InputFileNames)
   {
      TFile InputFile(InputFileName.c_str());

      HiEventTreeMessenger     MEvent(InputFile);
      PbPbUPCTrackTreeMessenger       MTrackPbPbUPC(InputFile);
      GenParticleTreeMessenger MGen(InputFile);
      PFTreeMessenger          MPF(InputFile, PFTreeName);
      SkimTreeMessenger        MSkim(InputFile);
      TriggerTreeMessenger     MTrigger(InputFile);
      DzeroTreeMessenger       MDzero(InputFile);

      int EntryCount = MEvent.GetEntries() * Fraction;
      ProgressBar Bar(cout, EntryCount);
      Bar.SetStyle(-1);
      
      /////////////////////////////////
      //////// Main Event Loop ////////
      /////////////////////////////////   

      for(int iE = 0; iE < EntryCount; iE++)
      {
         if(EntryCount < 300 || (iE % (EntryCount / 250)) == 0)
         {
            Bar.Update(iE);
            Bar.Print();
         }
         
         MEvent.GetEntry(iE);
         MGen.GetEntry(iE);
         MTrackPbPbUPC.GetEntry(iE);
         MPF.GetEntry(iE);
         MSkim.GetEntry(iE);
         MTrigger.GetEntry(iE);
         MDzero.GetEntry(iE);

         MDzeroUPC.Clear();

         ////////////////////////////////////////
         ////////// Global event stuff //////////
         ////////////////////////////////////////
         
         MDzeroUPC.Run   = MEvent.Run;
         MDzeroUPC.Lumi  = MEvent.Lumi;
         MDzeroUPC.Event = MEvent.Event;

         ////////////////////////////
         ////////// Vertex //////////
         ////////////////////////////

         int BestVertex = -1;

         for(int i = 0; i < MTrackPbPbUPC.nVtx; i++)
         {  
            if(BestVertex < 0 || MTrackPbPbUPC.ptSumVtx->at(i) > MTrackPbPbUPC.ptSumVtx->at(BestVertex))
               BestVertex = i;
         }
         if(BestVertex >= 0)
         {
            MDzeroUPC.VX      = MTrackPbPbUPC.xVtx->at(BestVertex);
            MDzeroUPC.VY      = MTrackPbPbUPC.yVtx->at(BestVertex);
            MDzeroUPC.VZ      = MTrackPbPbUPC.zVtx->at(BestVertex);
            MDzeroUPC.VXError = MTrackPbPbUPC.xErrVtx->at(BestVertex);
            MDzeroUPC.VYError = MTrackPbPbUPC.yErrVtx->at(BestVertex);
            MDzeroUPC.VZError = MTrackPbPbUPC.zErrVtx->at(BestVertex);
         }
         /////////////////////////////////////
         ////////// Event selection //////////
         /////////////////////////////////////

         if(IsData == true)
         {
         int pprimaryVertexFilter = MSkim.PVFilter;
         int pclusterCompatibilityFilter = MSkim.ClusterCompatibilityFilter;

         if(pprimaryVertexFilter == 0 ||  pclusterCompatibilityFilter == 0)
            continue;

         //HLT trigger to select dimuon events, see Kaya's note: AN2019_143_v12, p.5
         // FIXME: need to be replaced with the actual PbPb triggers
         //int HLT_HIUPC_SingleJet8_ZDC1nXOR_MaxPixelCluster50000_2023 =
         //  MTrigger.CheckTriggerStartWith("HLT_HIUPC_SingleJet8_ZDC1nXOR_MaxPixelCluster50000");
         int HLT_HIUPC_ZDC1nOR_SinglePixelTrackLowPt_MaxPixelCluster400_2023 =
           MTrigger.CheckTriggerStartWith("HLT_HIUPC_ZDC1nOR_SinglePixelTrackLowPt_MaxPixelCluster400");
         int HLT_HIUPC_ZDC1nOR_MinPixelCluster400_MaxPixelCluster10000_2023 = 
           MTrigger.CheckTriggerStartWith("HLT_HIUPC_ZDC1nOR_MinPixelCluster400_MaxPixelCluster10000");
         if (
             //HLT_HIUPC_SingleJet8_ZDC1nXOR_MaxPixelCluster50000_2023           == 0 &&
             HLT_HIUPC_ZDC1nOR_SinglePixelTrackLowPt_MaxPixelCluster400_2023   == 0 &&
             HLT_HIUPC_ZDC1nOR_MinPixelCluster400_MaxPixelCluster10000_2023 == 0
            )
	    continue;
         }
         for(int iD = 0; iD < MDzero.Dsize; iD++){
           if(MDzero.Dpt[iD] < MinDzeroPT || MDzero.PassUPCDzero2023Cut(iD) == false) continue;
           MDzeroUPC.Dphi->push_back(MDzero.Dphi[iD]);
           MDzeroUPC.Dpt->push_back(MDzero.Dpt[iD]);
           MDzeroUPC.Dy->push_back(MDzero.Dy[iD]);
           MDzeroUPC.Dmass->push_back(MDzero.Dmass[iD]);
           MDzeroUPC.Dtrk1Pt->push_back(MDzero.Dtrk1Pt[iD]);
           MDzeroUPC.Dtrk2Pt->push_back(MDzero.Dtrk2Pt[iD]);
           MDzeroUPC.Dchi2cl->push_back(MDzero.Dchi2cl[iD]);
           MDzeroUPC.DsvpvDistance->push_back(MDzero.DsvpvDistance[iD]);
           MDzeroUPC.DsvpvDisErr->push_back(MDzero.DsvpvDisErr[iD]);
           MDzeroUPC.DsvpvDistance_2D->push_back(MDzero.DsvpvDistance_2D[iD]);
           MDzeroUPC.DsvpvDisErr_2D->push_back(MDzero.DsvpvDisErr_2D[iD]);
           MDzeroUPC.Dalpha->push_back(MDzero.Dalpha[iD]);
           MDzeroUPC.Ddtheta->push_back(MDzero.Ddtheta[iD]); 
         }
         
         MDzeroUPC.FillEntry();
      }
   
      Bar.Update(EntryCount);
      Bar.Print();
      Bar.PrintLine();
   
      InputFile.Close();
   }

   OutputFile.cd();
   Tree.Write();
   InfoTree.Write();

   OutputFile.Close();

   return 0;
}

double GetHFSum(PFTreeMessenger *M)
{
   if(M == nullptr)
      return -1;
   if(M->Tree == nullptr)
      return -1;

   double Sum = 0;
   for(int iPF = 0; iPF < M->ID->size(); iPF++)
   {
      if(fabs(M->Eta->at(iPF)) < 3)
         continue;
      if(fabs(M->Eta->at(iPF)) > 5)
         continue;
      Sum = Sum + M->E->at(iPF);
   }

   // cout << Sum << endl;

   return Sum;
}

double GetGenHFSum(GenParticleTreeMessenger *M, int SubEvent)
{
   if(M == nullptr)
      return -1;
   if(M->Tree == nullptr)
      return -1;

   double Sum = 0;
   for(int iGen = 0; iGen < M->Mult; iGen++)
   {
      if(fabs(M->Eta->at(iGen)) < 3)
         continue;
      if(fabs(M->Eta->at(iGen)) > 5)
         continue;
      if(M->DaughterCount->at(iGen) > 0)
         continue;
      if(M->PT->at(iGen) < 0.4)   // for now...
         continue;

      if(SubEvent >= 0)   // if SubEvent >= 0, check subevent
      {
         if(M->SubEvent->at(iGen) != SubEvent)
            continue;
      }

      Sum = Sum + M->PT->at(iGen) * cosh(M->Eta->at(iGen));
   }

   return Sum;
}

            
