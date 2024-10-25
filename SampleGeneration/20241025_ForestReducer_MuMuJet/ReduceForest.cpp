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

   bool DoGenLevel                    = CL.GetBool("DoGenLevel", true);
   bool IsData                        = CL.GetBool("IsData", false);
   bool IsPP                          = CL.GetBool("IsPP", false);
   int Year                           = CL.GetInt("Year", 2018);
   bool IsBackground                  = CL.GetBool("IsBackground", false);
   double Fraction                    = CL.GetDouble("Fraction", 1.00);
   double MinZPT                      = CL.GetDouble("MinZPT", 20);
   double MinTrackPT                  = CL.GetDouble("MinTrackPT", 1);
   bool DoAlternateTrackSelection     = CL.GetBool("DoAlternateTrackSelection", false);
   int AlternateTrackSelection        = DoAlternateTrackSelection ? CL.GetInt("AlternateTrackSelection") : 0;
   bool DoSumET                       = CL.GetBool("DoSumET", false);
   double MuonVeto                    = CL.GetDouble("MuonVeto", 0.01);
   bool CheckZ                        = CL.GetBool("CheckZ", true);
   string TrackEfficiencyPath         = (DoGenLevel == false) ? CL.Get("TrackEfficiencyPath") : "";
   bool DoTrackResidual               = DoGenLevel ? false : CL.GetBool("DoTrackResidual", false);
   vector<string> TrackResidualPath   = (DoTrackResidual == true) ? CL.GetStringVector("TrackResidualPath") : vector<string>();
   bool DoInterSampleZWeight          = CL.GetBool("DoInterSampleZWeight", (IsPP == true && IsData == false));

   string PFTreeName                  = IsPP ? "pfcandAnalyzer/pfTree" : "particleFlowAnalyser/pftree";
   PFTreeName                         = CL.Get("PFTree", PFTreeName);

   TrkEff2017pp *TrackEfficiencyPP2017 = nullptr;
   TrkEff2018PbPb *TrackEfficiencyPbPb2018 = nullptr;
   TrkEff2023PbPb *TrackEfficiencyPbPb2023 = nullptr;
   if(DoGenLevel == false)
   {
      if(IsPP == true && (Year == 2017 || Year == 2018))   // 2018 does not have pp ref but for convenience
         TrackEfficiencyPP2017 = new TrkEff2017pp(false, TrackEfficiencyPath);
      else if(IsPP == false && Year == 2018)
      {
         if(DoAlternateTrackSelection == false)
            TrackEfficiencyPbPb2018 = new TrkEff2018PbPb("general", "", false, TrackEfficiencyPath);
         if(DoAlternateTrackSelection == true && AlternateTrackSelection == 0)
            TrackEfficiencyPbPb2018 = new TrkEff2018PbPb("general", "", false, TrackEfficiencyPath);
         if(DoAlternateTrackSelection == true && AlternateTrackSelection == 1)
            TrackEfficiencyPbPb2018 = new TrkEff2018PbPb("general", "Loose", false, TrackEfficiencyPath);
         if(DoAlternateTrackSelection == true && AlternateTrackSelection == 2)
            TrackEfficiencyPbPb2018 = new TrkEff2018PbPb("general", "Tight", false, TrackEfficiencyPath);
      }
      else if(IsPP == false && Year == 2023)
      {
         if(DoAlternateTrackSelection == false)
            TrackEfficiencyPbPb2023 = new TrkEff2023PbPb("general", "", false, TrackEfficiencyPath);
         if(DoAlternateTrackSelection == true && AlternateTrackSelection == 0)
            TrackEfficiencyPbPb2023 = new TrkEff2023PbPb("general", "", false, TrackEfficiencyPath);
         if(DoAlternateTrackSelection == true && AlternateTrackSelection == 1)
            TrackEfficiencyPbPb2023 = new TrkEff2023PbPb("general", "Loose", false, TrackEfficiencyPath);
         if(DoAlternateTrackSelection == true && AlternateTrackSelection == 2)
            TrackEfficiencyPbPb2023 = new TrkEff2023PbPb("general", "Tight", false, TrackEfficiencyPath);
      }
      else
      {
         cerr << endl;
         cerr << "Error in track efficiency!" << endl;
         cerr << "Data/Year combination (IsPP = " << IsPP << ", Year = " << Year << ") does not exist!" << endl;
         cerr << endl;
      }
   }
   TrackResidualCentralityCorrector TrackResidual(TrackResidualPath);

   TFile OutputFile(OutputFileName.c_str(), "RECREATE");
   TTree Tree("Tree", Form("Tree for MuMu tagged jet analysis (%s)", VersionString.c_str()));
   TTree InfoTree("InfoTree", "Information");

   MuMuJetMessenger MMuMuJet;
   MMuMuJet.SetBranch(&Tree);

   for(string InputFileName : InputFileNames)
   {
      TFile InputFile(InputFileName.c_str());

      HiEventTreeMessenger     MEvent(InputFile);
      TrackTreeMessenger       MTrackPP(InputFile);
      PbPbTrackTreeMessenger   MTrack(InputFile);
      GenParticleTreeMessenger MGen(InputFile);
      PFTreeMessenger          MPF(InputFile, PFTreeName);
      MuTreeMessenger          MMu(InputFile);
      SkimTreeMessenger        MSkim(InputFile);
      TriggerTreeMessenger     MTrigger(InputFile);
      
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
         if(IsPP == true)         MTrackPP.GetEntry(iE);
         else                     MTrack.GetEntry(iE);
         MPF.GetEntry(iE);
         MMu.GetEntry(iE);
         MSkim.GetEntry(iE);
         MTrigger.GetEntry(iE);

         MMuMuJet.Clear();

         ////////////////////////////////////////
         ////////// Global event stuff //////////
         ////////////////////////////////////////
         
         MMuMuJet.Run   = MEvent.Run;
         MMuMuJet.Lumi  = MEvent.Lumi;
         MMuMuJet.Event = MEvent.Event;
         MMuMuJet.hiBin = MEvent.hiBin;
         if(IsPP == false && IsData == true)   // need hiBin shifts!
         {
            MMuMuJet.hiBinUp   = GetHiBin(MEvent.hiHF, 1);
            MMuMuJet.hiBinDown = GetHiBin(MEvent.hiHF, -1);
         }
         MMuMuJet.hiHF  = MEvent.hiHF;
         MMuMuJet.NPU   = 0;
         if(MEvent.npus->size() == 9)
            MMuMuJet.NPU = MEvent.npus->at(5);
         else if(MEvent.npus->size() > 1)
            MMuMuJet.NPU = MEvent.npus->at(0);
         else
            MMuMuJet.NPU = 0;
         MMuMuJet.EventWeight = MEvent.weight;

         ////////////////////////////
         ////////// Vertex //////////
         ////////////////////////////

         MMuMuJet.NVertex = 0;
         int BestVertex = -1;
         for(int i = 0; i < (IsPP ? MTrackPP.nVtx : MTrack.VX->size()); i++)
         {
            if(IsPP == true && (BestVertex < 0 || MTrackPP.sumPtVtx[i] > MTrackPP.sumPtVtx[BestVertex]))
               BestVertex = i;
            if(IsPP == false && (BestVertex < 0 || MTrack.VPTSum->at(i) > MTrack.VPTSum->at(BestVertex)))
               BestVertex = i;

            MMuMuJet.NVertex = MMuMuJet.NVertex + 1;
         }

         if(BestVertex >= 0)
         {
            MMuMuJet.VX      = IsPP ? MTrackPP.xVtx[BestVertex] : MTrack.VX->at(BestVertex);
            MMuMuJet.VY      = IsPP ? MTrackPP.yVtx[BestVertex] : MTrack.VY->at(BestVertex);
            MMuMuJet.VZ      = IsPP ? MTrackPP.zVtx[BestVertex] : MTrack.VZ->at(BestVertex);
            MMuMuJet.VXError = IsPP ? MTrackPP.xVtxErr[BestVertex] : MTrack.VXError->at(BestVertex);
            MMuMuJet.VYError = IsPP ? MTrackPP.yVtxErr[BestVertex] : MTrack.VYError->at(BestVertex);
            MMuMuJet.VZError = IsPP ? MTrackPP.zVtxErr[BestVertex] : MTrack.VZError->at(BestVertex);

            if(IsData == false)
               MMuMuJet.VZWeight = IsPP ? GetVZWeightPP(MMuMuJet.VZ) : GetVZWeightPbPb(MMuMuJet.VZ);
            else
               MMuMuJet.VZWeight = 1;
         }

         /////////////////////////////////////
         ////////// Event selection //////////
         /////////////////////////////////////

         if(IsPP == true)
         {
            if(IsData == true)
            {
               int pprimaryVertexFilter = MSkim.PVFilter;
               int beamScrapingFilter = MSkim.BeamScrapingFilter;

               // Event selection criteria
               //    see https://twiki.cern.ch/twiki/bin/viewauth/CMS/HIPhotonJe5TeVpp2017PbPb2018
               if(pprimaryVertexFilter == 0 || beamScrapingFilter == 0)
                  continue;

               //HLT trigger to select dimuon events, see Kaya's note: AN2019_143_v12, p.5
               int HLT_HIL2Mu12_2018 = MTrigger.CheckTriggerStartWith("HLT_HIL2Mu12");
               int HLT_HIL3Mu12_2018 = MTrigger.CheckTriggerStartWith("HLT_HIL3Mu12");
               int HLT_HIL3Mu12_2023 = MTrigger.CheckTriggerStartWith("HLT_HIL3SingleMu12");
               if(HLT_HIL3Mu12_2018 == 0 && HLT_HIL2Mu12_2018 == 0 && HLT_HIL3Mu12_2023 == 0)
                  continue;

               MMuMuJet.NCollWeight = 1;
            }
            else
               MMuMuJet.NCollWeight = 1;
         }
         else
         {
            if(IsData == true)
            {
               int pprimaryVertexFilter = MSkim.PVFilter;
               int phfCoincFilter2Th4 = MSkim.HFCoincidenceFilter2Th4;
               int pclusterCompatibilityFilter = MSkim.ClusterCompatibilityFilter;

               // Event selection criteria
               //    see https://twiki.cern.ch/twiki/bin/viewauth/CMS/HIPhotonJe5TeVpp2017PbPb2018
               if(pprimaryVertexFilter == 0 || phfCoincFilter2Th4 == 0 || pclusterCompatibilityFilter == 0)
                  continue;

               //HLT trigger to select dimuon events, see Kaya's note: AN2019_143_v12, p.5
               int HLT_HIL2Mu12_2018 = MTrigger.CheckTriggerStartWith("HLT_HIL2Mu12");
               int HLT_HIL3Mu12_2018 = MTrigger.CheckTriggerStartWith("HLT_HIL3Mu12");
               int HLT_HIL3Mu12_2023 = MTrigger.CheckTriggerStartWith("HLT_HIL3SingleMu12");
               if(IsBackground == false && HLT_HIL3Mu12_2018 == 0 && HLT_HIL2Mu12_2018 == 0 && HLT_HIL3Mu12_2023 == 0)
                  continue;

               MMuMuJet.NCollWeight = 1;
            }
            else
               MMuMuJet.NCollWeight = FindNColl(MEvent.hiBin);
         }

         ///////////////////////////
         ////////// Muons //////////
         ///////////////////////////

         TLorentzVector VGenZ, VGenMu1, VGenMu2;

         if(MMu.NGen > 1)
         {
            for(int igen1 = 0; igen1 < MMu.NGen; igen1++)
            {
               // We only want muon from Z's
               if(MMu.GenMom[igen1] != 23)
                  continue;
               if(MMu.GenPT[igen1] < 20)
                  continue;
               if(fabs(MMu.GenEta[igen1]) > 2.4)
                  continue;

               VGenMu1.SetPtEtaPhiM(MMu.GenPT[igen1],
                     MMu.GenEta[igen1],
                     MMu.GenPhi[igen1],
                     M_MU);

               for(int igen2 = igen1 + 1; igen2 < MMu.NGen; igen2++)
               {
                  // We only want muon from Z's
                  if(MMu.GenMom[igen2] != 23)
                     continue;
                  if(MMu.GenPT[igen2] < 20)
                     continue;
                  if(fabs(MMu.GenEta[igen2]) > 2.4)
                     continue;

                  VGenMu2.SetPtEtaPhiM(MMu.GenPT[igen2],
                        MMu.GenEta[igen2],
                        MMu.GenPhi[igen2],
                        M_MU);

                  VGenZ = VGenMu1 + VGenMu2;

                  if(VGenZ.M() < 60 || VGenZ.M() > 120)
                     continue;
                  if(fabs(VGenZ.Rapidity()) > 2.4)
                     continue;

                  MMuMuJet.genZMass->push_back(VGenZ.M());
                  MMuMuJet.genZPt->push_back  (VGenZ.Pt());
                  MMuMuJet.genZPhi->push_back (VGenZ.Phi());
                  MMuMuJet.genZEta->push_back (VGenZ.Eta());
                  MMuMuJet.genZY->push_back   (VGenZ.Rapidity());

                  MMuMuJet.genMuPt1->push_back(MMu.GenPT[igen1]);
                  MMuMuJet.genMuPt2->push_back(MMu.GenPT[igen2]);
                  MMuMuJet.genMuEta1->push_back(MMu.GenEta[igen1]);
                  MMuMuJet.genMuEta2->push_back(MMu.GenEta[igen2]);
                  MMuMuJet.genMuPhi1->push_back(MMu.GenPhi[igen1]);
                  MMuMuJet.genMuPhi2->push_back(MMu.GenPhi[igen2]);

                  double genDeltaMuEta = MMu.GenEta[igen1] - MMu.GenEta[igen2];
                  double genDeltaMuPhi = PhiRangePositive(DeltaPhi(MMu.GenPhi[igen1], MMu.GenPhi[igen2]));

                  MMuMuJet.genMuDeta->push_back(genDeltaMuEta);
                  MMuMuJet.genMuDphi->push_back(genDeltaMuPhi);
                  MMuMuJet.genMuDR->push_back(sqrt(genDeltaMuEta * genDeltaMuEta + genDeltaMuPhi * genDeltaMuPhi));

                  double genDeltaPhiStar = tan((M_PI - genDeltaMuPhi) / 2)
                     * sqrt(1 - tanh(genDeltaMuEta / 2) * tanh(genDeltaMuEta / 2));
                  MMuMuJet.genMuDphiS->push_back(genDeltaPhiStar);
               }
            }
         }

         for(int ipair = 0; ipair < MMu.NDi; ipair++)
         {
            // We want opposite-charge muons with some basic kinematic cuts
            if(MMu.DiCharge1[ipair] == MMu.DiCharge2[ipair])        continue;
            if(fabs(MMu.DiEta1[ipair]) > 2.4)                       continue;
            if(fabs(MMu.DiEta2[ipair]) > 2.4)                       continue;
            if(fabs(MMu.DiPT1[ipair]) < 20)                         continue;
            if(fabs(MMu.DiPT2[ipair]) < 20)                         continue;
            if(MMu.DimuonPassTightCut(ipair) == false)              continue;
            if(MMu.DiMass[ipair] < 60 || MMu.DiMass[ipair] > 120)   continue;

            TLorentzVector Mu1, Mu2;
            Mu1.SetPtEtaPhiM(MMu.DiPT1[ipair], MMu.DiEta1[ipair], MMu.DiPhi1[ipair], M_MU);
            Mu2.SetPtEtaPhiM(MMu.DiPT2[ipair], MMu.DiEta2[ipair], MMu.DiPhi2[ipair], M_MU);
            TLorentzVector Z = Mu1 + Mu2;
            if(fabs(Z.Rapidity()) > 2.4)
               continue;

            MMuMuJet.zMass->push_back(MMu.DiMass[ipair]);
            MMuMuJet.zEta->push_back(MMu.DiEta[ipair]);
            MMuMuJet.zY->push_back(Z.Rapidity());
            MMuMuJet.zPhi->push_back(MMu.DiPhi[ipair]);
            MMuMuJet.zPt->push_back(MMu.DiPT[ipair]);

            MMuMuJet.muEta1->push_back(MMu.DiEta1[ipair]);
            MMuMuJet.muEta2->push_back(MMu.DiEta2[ipair]);
            MMuMuJet.muPhi1->push_back(MMu.DiPhi1[ipair]);
            MMuMuJet.muPhi2->push_back(MMu.DiPhi2[ipair]);

            MMuMuJet.muPt1->push_back(MMu.DiPT1[ipair]);
            MMuMuJet.muPt2->push_back(MMu.DiPT2[ipair]);

            double deltaMuEta = MMu.DiEta1[ipair] - MMu.DiEta2[ipair];
            double deltaMuPhi = PhiRangePositive(DeltaPhi(MMu.DiPhi1[ipair], MMu.DiPhi2[ipair]));

            MMuMuJet.muDeta->push_back(deltaMuEta);
            MMuMuJet.muDphi->push_back(deltaMuPhi);
            MMuMuJet.muDR->push_back(sqrt(deltaMuEta * deltaMuEta + deltaMuPhi * deltaMuPhi));

            double deltaPhiStar = tan((M_PI - deltaMuPhi) / 2) * sqrt(1 - tanh(deltaMuEta / 2) * tanh(deltaMuEta / 2));

            MMuMuJet.muDphiS->push_back(deltaPhiStar);
         }

         MMuMuJet.SignalHF = DoGenLevel ? GetGenHFSum(&MGen) : (DoSumET ? MEvent.hiHF : GetHFSum(&MPF));
         MMuMuJet.SignalVZ = MEvent.vz;
         MMuMuJet.SubEvent0HF = GetGenHFSum(&MGen, 0);
         MMuMuJet.SubEventAllHF = GetGenHFSum(&MGen, -1);
         
         bool GoodGenZ = MMuMuJet.genZPt->size() > 0 && (MMuMuJet.genZPt->at(0) > MinZPT);
         bool GoodRecoZ = MMuMuJet.zPt->size() > 0 && (MMuMuJet.zPt->at(0) > MinZPT);
         if(CheckZ == true)
         {
            // Yen-Jie: Propose to save the charged hadron information when either GoodGenZ or GoodRecoZ is identified 
            if(DoGenLevel == true && (GoodGenZ == false && GoodRecoZ == false))
            {
               MMuMuJet.FillEntry();
               continue;
            }
            if(DoGenLevel == false && GoodRecoZ == false)
            {
               MMuMuJet.FillEntry();
               continue;
            }
         }

         ///////////////////////////////
         ////////// Z weights //////////
         ///////////////////////////////

         MMuMuJet.InterSampleZWeight = 1;
         if(DoInterSampleZWeight == true)   // reweight Z from pp MC to embedded MC
         {
            if(DoGenLevel == true && GoodGenZ == true)
               MMuMuJet.InterSampleZWeight = GetInterSampleZWeight(MMuMuJet.genZPt->at(0));
            if(DoGenLevel == false && GoodRecoZ == true)
               MMuMuJet.InterSampleZWeight = GetInterSampleZWeight(MMuMuJet.zPt->at(0));
         }

         MMuMuJet.ZWeight = 1;
         if(DoGenLevel == false && GoodRecoZ == true)
         {
            TLorentzVector Z;
            Z.SetPtEtaPhiM(MMuMuJet.zPt->at(0), MMuMuJet.zEta->at(0), MMuMuJet.zPhi->at(0), MMuMuJet.zMass->at(0));
            if(IsPP == false)
            {
               if(IsData == false)
                  MMuMuJet.ZWeight = GetZWeightPbPbMC(Z.Pt(), Z.Rapidity(), MMuMuJet.hiBin);
               else
               {
                  MMuMuJet.ZWeight = GetZWeightPbPbDataTrigger(Z.Pt(), Z.Rapidity(), MMuMuJet.hiBin);

                  double Mu1Eta = MMuMuJet.muEta1->at(0);
                  double Mu1PT = MMuMuJet.muPt1->at(0);
                  double Mu2Eta = MMuMuJet.muEta1->at(0);
                  double Mu2PT = MMuMuJet.muPt1->at(0);
                  double Centrality = MMuMuJet.hiBin * 0.5;

                  MMuMuJet.ExtraZWeight[0] =
                     tnp_weight_glbPFtrk_pbpb(Mu1Eta, Centrality, -1)
                     / tnp_weight_glbPFtrk_pbpb(Mu1Eta, Centrality, 0)
                     * tnp_weight_glbPFtrk_pbpb(Mu2Eta, Centrality, -1)
                     / tnp_weight_glbPFtrk_pbpb(Mu2Eta, Centrality, 0);
                  MMuMuJet.ExtraZWeight[1] =
                     tnp_weight_glbPFtrk_pbpb(Mu1Eta, Centrality, -2)
                     / tnp_weight_glbPFtrk_pbpb(Mu1Eta, Centrality, 0)
                     * tnp_weight_glbPFtrk_pbpb(Mu2Eta, Centrality, -2)
                     / tnp_weight_glbPFtrk_pbpb(Mu2Eta, Centrality, 0);
                  MMuMuJet.ExtraZWeight[2] =
                     tnp_weight_muid_pbpb(Mu1Eta, -1)
                     / tnp_weight_muid_pbpb(Mu1Eta, 0)
                     * tnp_weight_muid_pbpb(Mu2Eta, -1)
                     / tnp_weight_muid_pbpb(Mu2Eta, 0);
                  MMuMuJet.ExtraZWeight[3] =
                     tnp_weight_muid_pbpb(Mu1Eta, -2)
                     / tnp_weight_muid_pbpb(Mu1Eta, 0)
                     * tnp_weight_muid_pbpb(Mu2Eta, -2)
                     / tnp_weight_muid_pbpb(Mu2Eta, 0);
                  // MMuMuJet.ExtraZWeight[4] =
                  //    tnp_weight_trig_pbpb(Mu1PT, Mu1Eta, Centrality, -1)
                  //    / tnp_weight_trig_pbpb(Mu1PT, Mu1Eta, Centrality, 0)
                  //    * tnp_weight_trig_pbpb(Mu2PT, Mu2Eta, Centrality, -1)
                  //    / tnp_weight_trig_pbpb(Mu2PT, Mu2Eta, Centrality, 0);
                  // MMuMuJet.ExtraZWeight[5] =
                  //    tnp_weight_trig_pbpb(Mu1PT, Mu1Eta, Centrality, -2)
                  //    / tnp_weight_trig_pbpb(Mu1PT, Mu1Eta, Centrality, 0)
                  //    * tnp_weight_trig_pbpb(Mu2PT, Mu2Eta, Centrality, -2)
                  //    / tnp_weight_trig_pbpb(Mu2PT, Mu2Eta, Centrality, 0);
                  MMuMuJet.ExtraZWeight[4] =
                     tnp_weight_trig_double_pbpb(Mu1PT, Mu1Eta, Centrality, Mu2PT, Mu2Eta, Centrality, -1)
                     / tnp_weight_trig_double_pbpb(Mu1PT, Mu1Eta, Centrality, Mu2PT, Mu2Eta, Centrality, 0);
                  MMuMuJet.ExtraZWeight[5] =
                     tnp_weight_trig_double_pbpb(Mu1PT, Mu1Eta, Centrality, Mu2PT, Mu2Eta, Centrality, -2)
                     / tnp_weight_trig_double_pbpb(Mu1PT, Mu1Eta, Centrality, Mu2PT, Mu2Eta, Centrality, 0);

                  MMuMuJet.ExtraZWeight[6] =
                     tnp_weight_glbPFtrk_pbpb(Mu1Eta, Centrality, 1)
                     / tnp_weight_glbPFtrk_pbpb(Mu1Eta, Centrality, 0)
                     * tnp_weight_glbPFtrk_pbpb(Mu2Eta, Centrality, 1)
                     / tnp_weight_glbPFtrk_pbpb(Mu2Eta, Centrality, 0);
                  MMuMuJet.ExtraZWeight[7] =
                     tnp_weight_glbPFtrk_pbpb(Mu1Eta, Centrality, 2)
                     / tnp_weight_glbPFtrk_pbpb(Mu1Eta, Centrality, 0)
                     * tnp_weight_glbPFtrk_pbpb(Mu2Eta, Centrality, 2)
                     / tnp_weight_glbPFtrk_pbpb(Mu2Eta, Centrality, 0);
                  MMuMuJet.ExtraZWeight[8] =
                     tnp_weight_muid_pbpb(Mu1Eta, 1)
                     / tnp_weight_muid_pbpb(Mu1Eta, 0)
                     * tnp_weight_muid_pbpb(Mu2Eta, 1)
                     / tnp_weight_muid_pbpb(Mu2Eta, 0);
                  MMuMuJet.ExtraZWeight[9] =
                     tnp_weight_muid_pbpb(Mu1Eta, 2)
                     / tnp_weight_muid_pbpb(Mu1Eta, 0)
                     * tnp_weight_muid_pbpb(Mu2Eta, 2)
                     / tnp_weight_muid_pbpb(Mu2Eta, 0);
                  // MMuMuJet.ExtraZWeight[10] =
                  //    tnp_weight_trig_pbpb(Mu1PT, Mu1Eta, Centrality, 1)
                  //    / tnp_weight_trig_pbpb(Mu1PT, Mu1Eta, Centrality, 0)
                  //    * tnp_weight_trig_pbpb(Mu2PT, Mu2Eta, Centrality, 1)
                  //    / tnp_weight_trig_pbpb(Mu2PT, Mu2Eta, Centrality, 0);
                  // MMuMuJet.ExtraZWeight[11] =
                  //    tnp_weight_trig_pbpb(Mu1PT, Mu1Eta, Centrality, 2)
                  //    / tnp_weight_trig_pbpb(Mu1PT, Mu1Eta, Centrality, 0)
                  //    * tnp_weight_trig_pbpb(Mu2PT, Mu2Eta, Centrality, 2)
                  //    / tnp_weight_trig_pbpb(Mu2PT, Mu2Eta, Centrality, 0);
                  MMuMuJet.ExtraZWeight[10] =
                     tnp_weight_trig_double_pbpb(Mu1PT, Mu1Eta, Centrality, Mu2PT, Mu2Eta, Centrality, 1)
                     / tnp_weight_trig_double_pbpb(Mu1PT, Mu1Eta, Centrality, Mu2PT, Mu2Eta, Centrality, 0);
                  MMuMuJet.ExtraZWeight[11] =
                     tnp_weight_trig_double_pbpb(Mu1PT, Mu1Eta, Centrality, Mu2PT, Mu2Eta, Centrality, 2)
                     / tnp_weight_trig_double_pbpb(Mu1PT, Mu1Eta, Centrality, Mu2PT, Mu2Eta, Centrality, 0);
               }
            }
            else
            {
               if(IsData == false)
                  MMuMuJet.ZWeight = GetZWeightPPMC(Z.Pt(), Z.Rapidity());
               else
               {
                  MMuMuJet.ZWeight = GetZWeightPPDataTrigger(Z.Pt(), Z.Rapidity());
                  // Extra Z weight for systematics

                  double Mu1Eta = MMuMuJet.muEta1->at(0);
                  double Mu1PT = MMuMuJet.muPt1->at(0);
                  double Mu2Eta = MMuMuJet.muEta1->at(0);
                  double Mu2PT = MMuMuJet.muPt1->at(0);

                  MMuMuJet.ExtraZWeight[0] =
                     tnp_weight_TightID_pp(Mu1Eta, 1)
                     / tnp_weight_TightID_pp(Mu1Eta, 0)
                     * tnp_weight_TightID_pp(Mu2Eta, 1)
                     / tnp_weight_TightID_pp(Mu2Eta, 0);
                  MMuMuJet.ExtraZWeight[1] =
                     tnp_weight_TightID_pp(Mu1Eta, -1)
                     / tnp_weight_TightID_pp(Mu1Eta, 0)
                     * tnp_weight_TightID_pp(Mu2Eta, -1)
                     / tnp_weight_TightID_pp(Mu2Eta, 0);
                  // MMuMuJet.ExtraZWeight[2] =
                  //    tnp_weight_L3Mu12_pp(Mu1Eta, 1)
                  //    / tnp_weight_L3Mu12_pp(Mu1Eta, 0)
                  //    * tnp_weight_L3Mu12_pp(Mu2Eta, 1)
                  //    / tnp_weight_L3Mu12_pp(Mu2Eta, 0);
                  // MMuMuJet.ExtraZWeight[3] =
                  //    tnp_weight_L3Mu12_pp(Mu1Eta, -1)
                  //    / tnp_weight_L3Mu12_pp(Mu1Eta, 0)
                  //    * tnp_weight_L3Mu12_pp(Mu2Eta, -1)
                  //    / tnp_weight_L3Mu12_pp(Mu2Eta, 0);
                  MMuMuJet.ExtraZWeight[2] =
                     tnp_weight_L3Mu12_double_pp(Mu1Eta, Mu2Eta, 1)
                     / tnp_weight_L3Mu12_double_pp(Mu1Eta, Mu2Eta, 0);
                  MMuMuJet.ExtraZWeight[3] =
                     tnp_weight_L3Mu12_double_pp(Mu1Eta, Mu2Eta, -1)
                     / tnp_weight_L3Mu12_double_pp(Mu1Eta, Mu2Eta, 0);

                  for(int i = 4; i < 12; i++)
                     MMuMuJet.ExtraZWeight[i] = 1;
               }
            }
         }

         ////////////////////////////
         ////////// Tracks //////////
         ////////////////////////////
            
         int NTrack = DoGenLevel ? MGen.Mult : (IsPP ? MTrackPP.nTrk : MTrack.TrackPT->size());
         for(int iT = 0; iT < NTrack; iT++)
         {
            if(DoGenLevel == true)
            {
               if(MGen.DaughterCount->at(iT) > 0)
                  continue;
               if(MGen.Charge->at(iT) == 0)
                  continue;
            }
            if(DoGenLevel == false)
            {
               if(IsPP == true)
               {
                  if(DoAlternateTrackSelection == false && MTrackPP.PassZHadron2022Cut(iT) == false)
                     continue;
                  if(DoAlternateTrackSelection == true && AlternateTrackSelection == 0 && MTrackPP.PassZHadron2022Cut(iT) == false)
                     continue;
                  if(DoAlternateTrackSelection == true && AlternateTrackSelection == 1 && MTrackPP.PassZHadron2022CutLoose(iT) == false)
                     continue;
                  if(DoAlternateTrackSelection == true && AlternateTrackSelection == 2 && MTrackPP.PassZHadron2022CutTight(iT) == false)
                     continue;
               }
               if(IsPP == false)
               {
                  if(DoAlternateTrackSelection == false && MTrack.PassZHadron2022Cut(iT) == false)
                     continue;
                  if(DoAlternateTrackSelection == true && AlternateTrackSelection == 0 && MTrack.PassZHadron2022Cut(iT) == false)
                     continue;
                  if(DoAlternateTrackSelection == true && AlternateTrackSelection == 1 && MTrack.PassZHadron2022CutLoose(iT) == false)
                     continue;
                  if(DoAlternateTrackSelection == true && AlternateTrackSelection == 2 && MTrack.PassZHadron2022CutTight(iT) == false)
                     continue;
               }
            }

            double TrackEta = DoGenLevel ? MGen.Eta->at(iT) : (IsPP ? MTrackPP.trkEta[iT] : MTrack.TrackEta->at(iT));
            double TrackPhi = DoGenLevel ? MGen.Phi->at(iT) : (IsPP ? MTrackPP.trkPhi[iT] : MTrack.TrackPhi->at(iT));
            double TrackPT  = DoGenLevel ? MGen.PT->at(iT) : (IsPP ? MTrackPP.trkPt[iT] : MTrack.TrackPT->at(iT));
            int TrackCharge = DoGenLevel ? MGen.Charge->at(iT) : (IsPP ? MTrackPP.trkCharge[iT] : MTrack.TrackCharge->at(iT));
            int SubEvent    = DoGenLevel ? (MGen.SubEvent->at(iT) + IsBackground) : (IsPP ? 0 : IsBackground);
            
            if(TrackPT < MinTrackPT)
               continue;
            if(TrackEta < -2.4)
               continue;
            if(TrackEta > +2.4)
               continue;
            
            TLorentzVector V; 
            V.SetPtEtaPhiM(TrackPT, TrackEta, TrackPhi, 0.139570);

            if(CheckZ == true && (DoGenLevel ? (GoodGenZ == true) : (GoodRecoZ == true)))
            {
               double Mu1Eta = DoGenLevel ? MMuMuJet.genMuEta1->at(0) : MMuMuJet.muEta1->at(0);
               double Mu1Phi = DoGenLevel ? MMuMuJet.genMuPhi1->at(0) : MMuMuJet.muPhi1->at(0);
               double Mu2Eta = DoGenLevel ? MMuMuJet.genMuEta2->at(0) : MMuMuJet.muEta2->at(0);
               double Mu2Phi = DoGenLevel ? MMuMuJet.genMuPhi2->at(0) : MMuMuJet.muPhi2->at(0);

               double DeltaEtaMu1 = TrackEta - Mu1Eta;
               double DeltaEtaMu2 = TrackEta - Mu2Eta;
               double DeltaPhiMu1 = DeltaPhi(TrackPhi, Mu1Phi);
               double DeltaPhiMu2 = DeltaPhi(TrackPhi, Mu2Phi);

               double DeltaRMu1 = sqrt(DeltaEtaMu1 * DeltaEtaMu1 + DeltaPhiMu1 * DeltaPhiMu1);
               double DeltaRMu2 = sqrt(DeltaEtaMu2 * DeltaEtaMu2 + DeltaPhiMu2 * DeltaPhiMu2);

               bool MuTagged = false;
               if(DeltaRMu1 < MuonVeto)   MuTagged = true;
               if(DeltaRMu2 < MuonVeto)   MuTagged = true;
            
               MMuMuJet.trackMuTagged->push_back(MuTagged);
               MMuMuJet.trackMuDR->push_back(min(DeltaRMu1, DeltaRMu2));
            }
            else
            {
               MMuMuJet.trackMuTagged->push_back(false);
               MMuMuJet.trackMuDR->push_back(-1);
            }

            MMuMuJet.trackPhi->push_back(TrackPhi);
            MMuMuJet.trackEta->push_back(TrackEta);
            MMuMuJet.trackY->push_back(V.Rapidity());
            MMuMuJet.trackPt->push_back(TrackPT);
            MMuMuJet.subevent->push_back(SubEvent);

            double TrackCorrection = 1;
            if(DoGenLevel == false)
            {
               // cout << TrackPT << " " << TrackEta << " " << MMuMuJet.hiBin << endl;

               if(IsPP == true && (Year == 2017 || Year == 2018))
                  TrackCorrection = TrackEfficiencyPP2017->getCorrection(TrackPT, TrackEta);
               else if(IsPP == false && Year == 2018)
                  TrackCorrection = TrackEfficiencyPbPb2018->getCorrection(TrackPT, TrackEta, MMuMuJet.hiBin);
               else if(IsPP == false && Year == 2023)
                  TrackCorrection = TrackEfficiencyPbPb2023->getCorrection(TrackPT, TrackEta, MMuMuJet.hiBin);
            }
            double TrackResidualCorrection = 1;
            if(DoTrackResidual == true && DoGenLevel == false)
               TrackResidualCorrection = TrackResidual.GetCorrectionFactor(TrackPT, TrackEta, TrackPhi, MMuMuJet.hiBin);
            MMuMuJet.trackWeight->push_back(TrackCorrection * TrackResidualCorrection);
            MMuMuJet.trackResidualWeight->push_back(TrackResidualCorrection);
            /// MMuMuJet.trackResidualWeight->push_back(1);
         }

         MMuMuJet.FillEntry();
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

