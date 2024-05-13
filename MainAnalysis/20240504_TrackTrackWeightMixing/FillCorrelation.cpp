#include <iostream>
using namespace std;

#include "TFile.h"
#include "TTree.h"
#include "TH2D.h"

#include "CommandLine.h"
#include "ProgressBar.h"
#include "Messenger.h"
#include "CommonFunctions.h"

struct Event;
struct Match;
int main(int argc, char *argv[]);
double GetDeltaPhi(double Phi1, double Phi2);

struct Event
{
   int Index;
   double Weight;
   Event(int i, double w) : Index(i), Weight(w) {}
};

struct Match
{
   Event Signal;
   Event Background;
   Match(Event &E1, Event &E2) : Signal(E1), Background(E2) {}
};

int main(int argc, char *argv[])
{
   CommandLine CL(argc, argv);

   bool SelfMixingMode          = CL.GetBool("SelfMixing", true);
   string SignalFileName        = CL.Get("Signal");
   string BackgroundFileName    = SelfMixingMode ? "NONE" : CL.Get("Background");
   string OutputFileName        = CL.Get("Output");
   double Fraction              = CL.GetDouble("Fraction", 1.00);
   double MinPT                 = CL.GetDouble("MinPT", 3);
   bool IsPP                    = CL.GetBool("IsPP", false);
   bool IsReco                  = CL.GetBool("IsReco", true);
   bool CheckZ                  = CL.GetBool("CheckZ", true);
   bool CheckBackgroundZ        = SelfMixingMode ? false : CL.GetBool("CheckBackgroundZ", false);
   bool DoZReweight             = CL.GetBool("DoZReweight", false);
   bool CheckSignalHiBin        = CL.GetBool("CheckSignalHiBin", true);
   double HiBinMin              = CL.GetDouble("HiBinMin", 20);
   double HiBinMax              = CL.GetDouble("HiBinMax", 60);
   string SignalHFShiftFileName = CL.Get("SignalHFShiftFile", "NONE");
   string HFShiftFileName       = CL.Get("HFShiftFile", "NONE");
   vector<int> SubEvent         = CL.GetIntegerVector("SubEvent", vector<int>{-1});
   int OverSample               = SelfMixingMode ? 1 : CL.GetInt("OverSample", 100);

   if(SubEvent.size() == 0)   SubEvent = vector<int>{-1, -1};
   if(SubEvent.size() == 1)   SubEvent.push_back(SubEvent[0]);

   double BinMin = 0.001;
   double BinMax = 4;
   double Bins[101] = {0};
   for(int iB = 0; iB <= 100; iB++)
      Bins[iB] = BinMin * exp((log(BinMax) - log(BinMin)) / 100 * iB);

   TFile OutputFile(OutputFileName.c_str(), "RECREATE");

   TH1D HCount("HCount", ";", 10, 0, 10);

   // Event-wide distributions
   TH1D HSignalHF("HSignalHF", ";log(SumHF);", 100, 0, 13.5);
   TH1D HMatchedHF("HMatchedHF", ";log(SumHF);", 100, 0, 13.5);
   TH1D HSubEvent0HF("HSubEvent0HF", ";log(SumHF);", 100, 0, 13.5);
   TH1D HSubEvent1HF("HSubEvent1HF", ";log(SumHF);", 100, 0, 13.5);

   // Z distributions
   TH1D HZY("HZY", ";y_{Z}", 100, -3, 3);
   TH1D HZPT("HZPT", ";p_{T,Z}", 100, 0, 100);
   TH1D HZMass("HZMass", ";m_{Z}", 100, 40, 140);
   
   // Z-hadron correlations
   TH1D HZHEta("HZHEta", ";y_{Z} - #eta_{h}", 100, -3, 3);
   TH1D HZHPhi("HZHPhi", ";#Delta#phi_{Z,h}", 100, -M_PI, M_PI);
   TH1D HZHPT("HZHPT", ";p_{T,H}", 100, 0, 100);
   TH1D HZHPTP1("HZHPTP1", ";p_{T,H}", 100, 0, 100);
   TH1D HZHPTP2("HZHPTP2", ";p_{T,H}", 100, 0, 100);
   TH1D HZHPTP3("HZHPTP3", ";p_{T,H}", 100, 0, 100);
   TH1D HZHPTP4("HZHPTP4", ";p_{T,H}", 100, 0, 100);
   
   // Track-track correlations
   TH2D HEtaPhi("HEtaPhi", ";#Delta#eta;#Delta#phi", 100, -3, 3, 100, -M_PI, M_PI);
   TH1D HEta("HEta", ";#Delta#eta", 100, -3, 3);
   TH1D HPhi("HPhi", ";#Delta#phi", 100, -M_PI, M_PI);
   TH1D HPT1("HPT1", ";p_{T}", 100, 0, 50);
   TH1D HPT1P1("HPT1P1", ";p_{T}", 100, 0, 50);
   TH1D HPT1P2("HPT1P2", ";p_{T}", 100, 0, 50);
   TH1D HPT1P3("HPT1P3", ";p_{T}", 100, 0, 50);
   TH1D HPT1P4("HPT1P4", ";p_{T}", 100, 0, 50);
   TH1D HPT2("HPT2", ";p_{T}", 100, 0, 50);
   TH1D HDeltaR("HDeltaR", ";#DeltaR;", 100, 0, 4);
   TH1D HDeltaREEC("HDeltaREEC", ";#DeltaR;", 100, 0, 4);
   TH1D HDeltaRLog("HDeltaRLog", ";#DeltaR;", 100, Bins);
   TH1D HDeltaREECLog("HDeltaREECLog", ";#DeltaR;", 100, Bins);

   TFile SignalFile(SignalFileName.c_str());
   TFile BackgroundFile(BackgroundFileName.c_str());

   TFile SignalHFShiftFile(SignalHFShiftFileName.c_str());
   TH1D *HSignalHFShiftWeight = (TH1D *)SignalHFShiftFile.Get("HRatio");
   
   TFile HFShiftFile(HFShiftFileName.c_str());
   TH1D *HHFShiftWeight = (TH1D *)HFShiftFile.Get("HRatio");

   ZHadronMessenger MZHadron(SignalFile, "Tree");
   ZHadronMessenger MZHadronBackground(BackgroundFile, "Tree");

   int EntryCount = MZHadron.GetEntries() * Fraction;
   int BackgroundEntryCount = MZHadronBackground.GetEntries() * Fraction;

   // Now let's build the index!  Step 1, loop over signal events and pick out the interesting ones 
   ProgressBar BarS(cout, EntryCount);
   BarS.SetStyle(-1);
   
   vector<Event> SignalEvent;
   for(int iE = 0; iE < EntryCount; iE++)
   {
      MZHadron.GetEntry(iE);

      if(EntryCount < 500 || (iE % (EntryCount / 300) == 0))
      {
         BarS.Update(iE);
         BarS.Print();
      }

      // Check if signal event is within our target centrality range
      if(IsPP == false)
      {
         if(CheckSignalHiBin == true && MZHadron.hiBin < HiBinMin)       continue;
         if(CheckSignalHiBin == true && MZHadron.hiBin >= HiBinMax)      continue;
      }

      // Event weights
      if(IsReco == false) MZHadron.VZWeight = 1;
      // double EventWeight = MZHadron.EventWeight * MZHadron.VZWeight * MZHadron.ZWeight;
      double EventWeight = MZHadron.EventWeight * MZHadron.ZWeight;
      // double EventWeight = 1;
         
      // HFShift weight
      double HFShiftWeight = 1;
      if(HSignalHFShiftWeight != nullptr)
      {
         int Bin = HSignalHFShiftWeight->FindBin(MZHadron.SignalHF);
         HFShiftWeight = HSignalHFShiftWeight->GetBinContent(Bin);
         if(HFShiftWeight == 0)   // don't bother matching this
            continue;
      }
      EventWeight = EventWeight * HFShiftWeight;

      // Do event selection if it's reco
      if(IsPP == true && IsReco == true && MZHadron.NVertex != 1)
         continue;
      if(CheckZ == true)
      {
         if(IsReco == true)
         {
            if(MZHadron.GoodRecoZ == false)   continue;
            if(MZHadron.zPt->at(0) < 20)      continue;

            if(DoZReweight == true)
               EventWeight = EventWeight * GetInterSampleZWeight(MZHadron.zPt->at(0));

            HZY.Fill(MZHadron.zY->at(0), EventWeight);
            HZPT.Fill(MZHadron.zPt->at(0), EventWeight);
            HZMass.Fill(MZHadron.zMass->at(0), EventWeight);
         }
         else
         {
            if(MZHadron.GoodGenZ == false)    continue;
            if(MZHadron.genZPt->at(0) < 20)   continue;
            
            if(DoZReweight == true)
               EventWeight = EventWeight * GetInterSampleZWeight(MZHadron.genZPt->at(0));

            HZY.Fill(MZHadron.genZY->at(0), EventWeight);
            HZPT.Fill(MZHadron.genZPt->at(0), EventWeight);
            HZMass.Fill(MZHadron.genZMass->at(0), EventWeight);
         }
      }

      SignalEvent.push_back(Event(iE, EventWeight));
   }
   BarS.Update(EntryCount);
   BarS.Print();
   BarS.PrintLine();

   if(OverSample > 1)
   {
      vector<Event> Temp = SignalEvent;
      for(int iO = 1; iO < OverSample; iO++)
         SignalEvent.insert(SignalEvent.end(), Temp.begin(), Temp.end());
   }

   // And step 2, loop over background events and keep the interesting ones
   vector<Event> BackgroundEvent;
   if(SelfMixingMode == false)
   {
      ProgressBar BarB(cout, BackgroundEntryCount);
      BarB.SetStyle(-1);
      for(int iE = 0; iE < BackgroundEntryCount; iE++)
      {
         MZHadronBackground.GetEntry(iE);

         if(BackgroundEntryCount < 500 || (iE % (BackgroundEntryCount / 300) == 0))
         {
            BarB.Update(iE);
            BarB.Print();
         }
         
         if(MZHadronBackground.trackPt == nullptr)
            continue;
         // if(MZHadronBackground.trackPt->size() == 0)
         //    continue;

         bool GoodBackgroundZ = false;
         if(IsReco == true && MZHadronBackground.GoodRecoZ == true && MZHadronBackground.zPt->at(0) > 20)
            GoodBackgroundZ = true;
         if(IsReco == false && MZHadronBackground.GoodGenZ == true && MZHadronBackground.genZPt->at(0) > 20)
            GoodBackgroundZ = true;
         if(CheckBackgroundZ == true && GoodBackgroundZ == false)
            continue;
      
         // HFShift weight
         double HFShiftWeight = 1;
         if(HHFShiftWeight != nullptr)
         {
            int Bin = HHFShiftWeight->FindBin(MZHadronBackground.SignalHF);
            HFShiftWeight = HHFShiftWeight->GetBinContent(Bin);
            if(HFShiftWeight == 0)   // don't bother matching this
               continue;
         }

         // Event weights
         if(IsReco == false)
            MZHadronBackground.VZWeight = 1;
         // double EventWeight = MZHadronBackground.EventWeight
         //    * MZHadronBackground.VZWeight * MZHadronBackground.ZWeight
         //    * HFShiftWeight;
         double EventWeight = MZHadronBackground.EventWeight * MZHadronBackground.ZWeight;
         EventWeight = EventWeight * HFShiftWeight;

         BackgroundEvent.push_back(Event(iE, EventWeight));
      }
      BarB.Update(BackgroundEntryCount);
      BarB.Print();
      BarB.PrintLine();
   }

   // Finally last step we match them up
   int NS = SignalEvent.size();
   int NB = BackgroundEvent.size();
   vector<Match> Matches;
   if(SelfMixingMode == true)
   {
      for(int iS = 0; iS < NS; iS++)
         Matches.push_back(Match(SignalEvent[iS], SignalEvent[iS]));
   }
   else   // SelfMixingMode false
   {
      if(NS >= NB)   // can probably do something more clever to make this more concise but anyways...
      {
         int iS = 0;
         for(int iB = 0; iB < NB; iB++)
         {
            int N = NS / NB;
            if(iB < NS % NB)
               N = N + 1;

            for(int i = 0; i < N; i++)
            {
               Matches.push_back(Match(SignalEvent[iS], BackgroundEvent[iB]));
               iS = iS + 1;
            }
         }
      }
      else
      {
         int iB = 0;
         for(int iS = 0; iS < NS; iS++)
         {
            int N = NB / NS;
            if(iS < NB % NS)
               N = N + 1;

            for(int i = 0; i < N; i++)
            {
               Matches.push_back(Match(SignalEvent[iS], BackgroundEvent[iB]));
               iB = iB + 1;
            }
         }
      }
   }

   // Finally we actually loop over the matched events and build the stuffs
   int MatchCount = Matches.size();

   cout << "NS = " << NS << ", NB = " << NB << endl;
   cout << "Match count = " << MatchCount << endl;

   ProgressBar Bar(cout, MatchCount);
   Bar.SetStyle(6);
   
   for(int iM = 0; iM < MatchCount; iM++)
   {
      if(MatchCount < 20000 || (iM % (MatchCount / 10000) == 0))
      {
         Bar.Update(iM);
         Bar.Print();
      }

      // cout << Matches[iM].Signal.Index << " " << Matches[iM].Background.Index << endl;
      
      // Only load signal if it's a new event
      if(iM == 0 || Matches[iM].Signal.Index != Matches[iM-1].Signal.Index)
         MZHadron.GetEntry(Matches[iM].Signal.Index);
      // Only load background if it's a new event AND self mixing is false
      if(SelfMixingMode == false)
         if(iM == 0 || Matches[iM].Background.Index != Matches[iM-1].Background.Index)
            MZHadronBackground.GetEntry(Matches[iM].Background.Index);

      // Get the event weights
      double EventWeight = Matches[iM].Signal.Weight;
      if(SelfMixingMode == false)
         EventWeight = EventWeight * Matches[iM].Background.Weight;

      // Event-wide plots
      HSignalHF.Fill(log(MZHadron.SignalHF), EventWeight);
      HMatchedHF.Fill(log(MZHadronBackground.SignalHF), EventWeight);
      HSubEvent0HF.Fill(log(MZHadron.SubEvent0HF), EventWeight);
      HSubEvent1HF.Fill(log(MZHadron.SubEventAllHF - MZHadron.SubEvent0HF), EventWeight);

      // Then we loop over tracks and start making plots
      int N1 = (MZHadron.trackPt != nullptr) ? MZHadron.trackPt->size() : 0;
      int N2 = N1;
      if(SelfMixingMode == false)
         N2 = (MZHadronBackground.trackPt != nullptr) ? MZHadronBackground.trackPt->size() : 0;

      if(N1 == 0)   continue;  // no track to mix into?
      // if(N2 == 0)   cout << "oho! N1 = " << N1 << ", N2 = " << N2 << endl;

      double SumWeight1 = 0;

      // Now we prep the tracks so that we don't need so many if's in the expensive loops

      vector<float> PT1;            PT1.reserve(N1);
      vector<float> Eta1;           Eta1.reserve(N1);
      vector<float> Phi1;           Phi1.reserve(N1);
      vector<float> TrackWeight1;   TrackWeight1.reserve(N1);

      for(int iP = 0; iP < N1; iP++)
      {
         if(MZHadron.trackMuTagged->at(iP) == true)                         continue;
         if(MZHadron.trackPt->at(iP) < MinPT)                               continue;
         if(SubEvent[0] >= 0 && MZHadron.subevent->at(iP) != SubEvent[0])   continue;

         PT1.push_back(MZHadron.trackPt->at(iP));
         Eta1.push_back(MZHadron.trackEta->at(iP));
         Phi1.push_back(MZHadron.trackPhi->at(iP));
         TrackWeight1.push_back(MZHadron.trackWeight->at(iP));
      }

      vector<float> PT2;            PT2.reserve(N2);
      vector<float> Eta2;           Eta2.reserve(N2);
      vector<float> Phi2;           Phi2.reserve(N2);
      vector<float> TrackWeight2;   TrackWeight2.reserve(N2);

      if(SelfMixingMode == true)
      {
         PT2          = PT1;
         Eta2         = Eta1;
         Phi2         = Phi1;
         TrackWeight2 = TrackWeight1;
      }
      else
      {
         for(int iP = 0; iP < N2; iP++)
         {
            if(MZHadronBackground.trackMuTagged->at(iP) == true)                         continue;
            if(MZHadronBackground.trackPt->at(iP) < MinPT)                               continue;
            if(SubEvent[1] >= 0 && MZHadronBackground.subevent->at(iP) != SubEvent[1])   continue;

            PT2.push_back(MZHadronBackground.trackPt->at(iP));
            Eta2.push_back(MZHadronBackground.trackEta->at(iP));
            Phi2.push_back(MZHadronBackground.trackPhi->at(iP));
            TrackWeight2.push_back(MZHadronBackground.trackWeight->at(iP));
         }
      }

      N1 = PT1.size();
      N2 = PT2.size();

      // cout << "Ns = " << N1 << " " << N2 << endl;

      // Z-hadron correlation plots.  We always need a good Z otherwise this does not make sense.
      if(CheckZ == true)
      {
         for(int iP = 0; iP < N2; iP++)
         {
            double TrackWeight = TrackWeight2[iP] * EventWeight;
            double DEta = (IsReco ? MZHadron.zY->at(0) : MZHadron.genZY->at(0)) - Eta2[iP];
            double DPhi = GetDeltaPhi((IsReco ? MZHadron.zPhi->at(0) : MZHadron.genZPhi->at(0)), Phi2[iP]);

            HZHEta.Fill(+DEta, 0.5 * TrackWeight);
            HZHEta.Fill(-DEta, 0.5 * TrackWeight);
            HZHPhi.Fill(+DPhi, 0.5 * TrackWeight);
            HZHPhi.Fill(-DPhi, 0.5 * TrackWeight);
            HZHPT.Fill(PT2[iP], TrackWeight);
            if(iM % 4 == 0)   HZHPTP1.Fill(PT2[iP], TrackWeight);
            if(iM % 4 == 1)   HZHPTP2.Fill(PT2[iP], TrackWeight);
            if(iM % 4 == 2)   HZHPTP3.Fill(PT2[iP], TrackWeight);
            if(iM % 4 == 3)   HZHPTP4.Fill(PT2[iP], TrackWeight);
         }
      }

      // Track-pair correlation plots.  Here we do not necessary need a good Z
      for(int iP1 = 0; iP1 < N1; iP1++)
      {
         SumWeight1 = SumWeight1 + TrackWeight1[iP1];

         for(int iP2 = 0; iP2 < N2; iP2++)
         {
            if(SelfMixingMode == true && iP1 == iP2)
               continue;

            double Weight = TrackWeight1[iP1] * TrackWeight2[iP2] * EventWeight;

            double DeltaEta = Eta1[iP1] - Eta2[iP2];
            double DeltaPhi = GetDeltaPhi(Phi1[iP1], Phi2[iP2]);
            double DeltaR = sqrt(DeltaEta * DeltaEta + DeltaPhi * DeltaPhi);

            HEtaPhi.Fill(+DeltaEta, +DeltaPhi, 0.25 * Weight);
            HEtaPhi.Fill(-DeltaEta, +DeltaPhi, 0.25 * Weight);
            HEtaPhi.Fill(+DeltaEta, -DeltaPhi, 0.25 * Weight);
            HEtaPhi.Fill(-DeltaEta, -DeltaPhi, 0.25 * Weight);
            HEta.Fill(+DeltaEta, 0.5 * Weight);
            HEta.Fill(-DeltaEta, 0.5 * Weight);
            HPhi.Fill(+DeltaPhi, 0.5 * Weight);
            HPhi.Fill(-DeltaPhi, 0.5 * Weight);
            HPT1.Fill(max(PT1[iP1], PT2[iP2]), Weight);
            HPT2.Fill(min(PT1[iP1], PT2[iP2]), Weight);
            HDeltaR.Fill(DeltaR, Weight);
            HDeltaREEC.Fill(DeltaR, Weight * PT1[iP1] * PT2[iP2]);
            HDeltaRLog.Fill(DeltaR, Weight);
            HDeltaREECLog.Fill(DeltaR, Weight * PT1[iP1] * PT2[iP2]);
            
            if(iM % 4 == 0)   HPT1P1.Fill(max(PT1[iP1], PT2[iP2]), Weight);
            if(iM % 4 == 1)   HPT1P2.Fill(max(PT1[iP1], PT2[iP2]), Weight);
            if(iM % 4 == 2)   HPT1P3.Fill(max(PT1[iP1], PT2[iP2]), Weight);
            if(iM % 4 == 3)   HPT1P4.Fill(max(PT1[iP1], PT2[iP2]), Weight);
         }
      }

      if(N1 > 0)
      {
         HCount.Fill(0.0, EventWeight);
         HCount.Fill(1.0, EventWeight * SumWeight1);
         if(iM % 4 == 0)   HCount.Fill(2.0, EventWeight);
         if(iM % 4 == 1)   HCount.Fill(3.0, EventWeight);
         if(iM % 4 == 2)   HCount.Fill(4.0, EventWeight);
         if(iM % 4 == 3)   HCount.Fill(5.0, EventWeight);
      }
   }
   Bar.Update(MatchCount);
   Bar.Print();
   Bar.PrintLine();

   HFShiftFile.Close();
   SignalHFShiftFile.Close();
   SignalFile.Close();
   BackgroundFile.Close();

   OutputFile.cd();

   HCount.Write();
   HSignalHF.Write();
   HMatchedHF.Write();
   HSubEvent0HF.Write();
   HSubEvent1HF.Write();
   HZY.Write();
   HZPT.Write();
   HZMass.Write();
   HZHEta.Write();
   HZHPhi.Write();
   HZHPT.Write();
   HZHPTP1.Write();
   HZHPTP2.Write();
   HZHPTP3.Write();
   HZHPTP4.Write();
   HEtaPhi.Write();
   HEta.Write();
   HPhi.Write();
   HPT1.Write();
   HPT1P1.Write();
   HPT1P2.Write();
   HPT1P3.Write();
   HPT1P4.Write();
   HPT2.Write();
   HDeltaR.Write();
   HDeltaREEC.Write();
   HDeltaRLog.Write();
   HDeltaREECLog.Write();

   OutputFile.Close();

   return 0;
}

double GetDeltaPhi(double Phi1, double Phi2)
{
   double DeltaPhi = Phi1 - Phi2;
   if(DeltaPhi > +M_PI)
      DeltaPhi = DeltaPhi - 2 * M_PI;
   if(DeltaPhi < -M_PI)
      DeltaPhi = DeltaPhi + 2 * M_PI;

   return DeltaPhi;
}
