#include <iostream>
using namespace std;

#include "TFile.h"
#include "TTree.h"
#include "TH2D.h"

#include "CommandLine.h"
#include "ProgressBar.h"
#include "Messenger.h"

int main(int argc, char *argv[]);
double GetDeltaPhi(double Phi1, double Phi2);

int main(int argc, char *argv[])
{
   CommandLine CL(argc, argv);

   bool SelfMixingMode       = CL.GetBool("SelfMixing", true);
   string SignalFileName     = CL.Get("Signal");
   string BackgroundFileName = SelfMixingMode ? "NONE" : CL.Get("Background");
   string OutputFileName     = CL.Get("Output");
   double Fraction           = CL.GetDouble("Fraction", 1.00);
   double MinPT              = CL.GetDouble("MinPT", 3.00);
   bool IsPP                 = CL.GetBool("IsPP", false);
   bool IsReco               = CL.GetBool("IsReco", true);
   bool CheckZ               = CL.GetBool("CheckZ", true);
   bool CheckSignalHiBin     = CL.GetBool("CheckSignalHiBin", true);
   double HiBinMin           = CL.GetDouble("HiBinMin", 0);
   double HiBinMax           = CL.GetDouble("HiBinMax", 60);
   bool CheckSignalHF        = CL.GetBool("CheckSignalHF", false);
   double SignalHFMin        = CL.GetDouble("SignalHFMin", 38220.226);
   double SignalHFMax        = CL.GetDouble("SignalHFMax", 155015.70);
   double HFShift            = CL.GetDouble("HFShift", 767.2);
   int OverSample            = SelfMixingMode ? 1 : CL.GetInt("OverSample", 100);
   int SubEvent              = CL.GetInteger("SubEvent", -1);

   TFile OutputFile(OutputFileName.c_str(), "RECREATE");

   TH1D HCount("HCount", ";", 10, 0, 10);

   // Z distributions
   TH1D HZY("HZY", ";y_{Z}", 100, -3, 3);
   TH1D HZPT("HZPT", ";p_{T,Z}", 100, 0, 100);
   TH1D HZMass("HZMass", ";m_{Z}", 100, 40, 140);
   
   // Z-hadron correlations
   TH1D HZHEta("HZHEta", ";y_{Z} - #eta_{h}", 100, -3, 3);
   TH1D HZHPhi("HZHPhi", ";#Delta#phi_{Z,h}", 100, -3, 3);
   TH1D HZHPT("HZHPT", ";p_{T,H}", 100, 0, 100);
   
   // Track-track correlations
   TH2D HEtaPhi("HEtaPhi", ";#Delta#eta;#Delta#phi", 100, -3, 3, 100, -M_PI, M_PI);
   TH1D HEta("HEta", ";#Delta#eta", 100, -3, 3);
   TH1D HPhi("HPhi", ";#Delta#phi", 100, -M_PI, M_PI);
   TH1D HPT1("HPT1", ";p_{T}", 100, 0, 50);
   TH1D HPT2("HPT2", ";p_{T}", 100, 0, 50);

   TFile SignalFile(SignalFileName.c_str());
   TFile BackgroundFile(BackgroundFileName.c_str());

   ZHadronMessenger MZHadron(SignalFile, "Tree");
   ZHadronMessenger MZHadronBackground(BackgroundFile, "Tree");

   int EntryCount = MZHadron.GetEntries() * Fraction;
   int BackgroundEntryCount = MZHadronBackground.GetEntries();

   ProgressBar Bar(cout, EntryCount);
   Bar.SetStyle(-1);
   
   int BackgroundIndex = (BackgroundEntryCount > 0) ? (rand() % BackgroundEntryCount) : 0;

   cout << EntryCount << endl;
   for(int iE = 0; iE < EntryCount; iE++)
   {
      MZHadron.GetEntry(iE);

      if(EntryCount < 500 || (iE % (EntryCount / 300) == 0))
      {
         Bar.Update(iE);
         Bar.Print();
      }

      if(IsPP == false)
      {
         if(CheckSignalHiBin == true)
         {
            if(MZHadron.hiBin < HiBinMin)   continue;
            if(MZHadron.hiBin >= HiBinMax)  continue;
         }
         if(CheckSignalHF == true)
         {
            if(MZHadron.SignalHF < SignalHFMin)   continue;
            if(MZHadron.SignalHF >= SignalHFMax)  continue;
         }
      }
         
      double EventWeight1 = MZHadron.EventWeight * MZHadron.VZWeight * MZHadron.ZWeight;

      if(IsPP == true && IsReco == true && MZHadron.NVertex != 1)
         continue;
      if(CheckZ == true)
      {
         if(IsReco == true)
         {
            if(MZHadron.zY == nullptr)
               continue;
            if(MZHadron.zY->size() == 0)
               continue;
            if(MZHadron.zPt->at(0) < 20)
               continue;
            if(MZHadron.zMass->at(0) > 120 || MZHadron.zMass->at(0) < 60)
               continue;
            if(MZHadron.zY->at(0) < -2.4 || MZHadron.zY->at(0) > 2.4)
               continue;

            HZY.Fill(MZHadron.zY->at(0), EventWeight1);
            HZPT.Fill(MZHadron.zPt->at(0), EventWeight1);
            HZMass.Fill(MZHadron.zMass->at(0), EventWeight1);
         }
         else
         {
            if(MZHadron.genZY == nullptr)
               continue;
            if(MZHadron.genZY->size() == 0)
               continue;
            if(MZHadron.genZPt->at(0) < 20)
               continue;
            if(MZHadron.genZMass->at(0) > 120 || MZHadron.genZMass->at(0) < 60)
               continue;
            if(MZHadron.genZY->at(0) < -2.4 || MZHadron.genZY->at(0) > 2.4)
               continue;

            HZY.Fill(MZHadron.genZY->at(0), EventWeight1);
            HZPT.Fill(MZHadron.genZPt->at(0), EventWeight1);
            HZMass.Fill(MZHadron.genZMass->at(0), EventWeight1);
         }
      }

      for(int iO = 0; iO < OverSample; iO++)
      {
         if(SelfMixingMode == false)
         {
            double BackgroundHF = -1;
            do
            {
               BackgroundIndex = BackgroundIndex + 1;
               if(BackgroundIndex >= BackgroundEntryCount)
                  BackgroundIndex = 0;
               MZHadronBackground.GetEntry(BackgroundIndex);
               BackgroundHF = MZHadronBackground.SignalHF;
            } while(BackgroundHF < SignalHFMin - HFShift || BackgroundHF > SignalHFMax - HFShift);
         }
      
         double EventWeight = EventWeight1;
         if(SelfMixingMode == false)
            EventWeight = EventWeight1 * MZHadronBackground.EventWeight * MZHadronBackground.VZWeight * MZHadronBackground.ZWeight;

         int N1 = (MZHadron.trackPt != nullptr) ? MZHadron.trackPt->size() : 0;
         int N2 = N1;
         if(SelfMixingMode == false)
            N2 = (MZHadronBackground.trackPt != nullptr) ? MZHadronBackground.trackPt->size() : 0;

         double SumWeight1 = 0;
      
         if(CheckZ == true)
         {
            for(int iP = 0; iP < N2; iP++)
            {
               bool IsMu = SelfMixingMode ? MZHadron.trackMuTagged->at(iP) : MZHadronBackground.trackMuTagged->at(iP);
               double PT = SelfMixingMode ? MZHadron.trackPt->at(iP) : MZHadronBackground.trackPt->at(iP);
               double Eta = SelfMixingMode ? MZHadron.trackEta->at(iP) : MZHadronBackground.trackEta->at(iP);
               double Phi = SelfMixingMode ? MZHadron.trackPhi->at(iP) : MZHadronBackground.trackPhi->at(iP);
               double TrackWeight = SelfMixingMode ? MZHadron.trackWeight->at(iP) : MZHadronBackground.trackWeight->at(iP);
               TrackWeight = TrackWeight * EventWeight;
               
               double DEta = (IsReco ? MZHadron.zY->at(0) : MZHadron.genZY->at(0)) - Eta;
               double DPhi = GetDeltaPhi((IsReco ? MZHadron.zPhi->at(0) : MZHadron.genZPhi->at(0)), Phi);

               if(IsMu == true)
                  continue;
               if(PT < MinPT)
                  continue;
            
               int SubEvent2 = SelfMixingMode ? MZHadron.subevent->at(iP) : MZHadronBackground.subevent->at(iP);
               if(SubEvent >= 0 && SubEvent2 != SubEvent)
                  continue;

               HZHEta.Fill(+DEta, 0.5 * TrackWeight);
               HZHEta.Fill(-DEta, 0.5 * TrackWeight);
               HZHPhi.Fill(+DPhi, 0.5 * TrackWeight);
               HZHPhi.Fill(-DPhi, 0.5 * TrackWeight);
               HZHPT.Fill(PT, TrackWeight);
            }
         }

         for(int iP1 = 0; iP1 < N1; iP1++)
         {
            double PT1 = MZHadron.trackPt->at(iP1);
            double Eta1 = MZHadron.trackEta->at(iP1);
            double Phi1 = MZHadron.trackPhi->at(iP1);
            double Weight1 = MZHadron.trackWeight->at(iP1);
            
            if(MZHadron.trackMuTagged->at(iP1) == true)
               continue;
            if(PT1 < MinPT)
               continue;

            if(SubEvent >= 0 && MZHadron.subevent->at(iP1) != SubEvent)
               continue;

            SumWeight1 = SumWeight1 + Weight1;

            for(int iP2 = 0; iP2 < N2; iP2++)
            {
               if(SelfMixingMode == true && iP1 == iP2)
                  continue;
            
               bool IsMu = SelfMixingMode ? MZHadron.trackMuTagged->at(iP2) : MZHadronBackground.trackMuTagged->at(iP2);
               double PT2 = SelfMixingMode ? MZHadron.trackPt->at(iP2) : MZHadronBackground.trackPt->at(iP2);
               double Eta2 = SelfMixingMode ? MZHadron.trackEta->at(iP2) : MZHadronBackground.trackEta->at(iP2);
               double Phi2 = SelfMixingMode ? MZHadron.trackPhi->at(iP2) : MZHadronBackground.trackPhi->at(iP2);
               double Weight2 = SelfMixingMode ? MZHadron.trackWeight->at(iP2) : MZHadronBackground.trackWeight->at(iP2);
               int SubEvent2 = SelfMixingMode ? MZHadron.subevent->at(iP2) : MZHadronBackground.subevent->at(iP2);

               if(IsMu == true)
                  continue;
               if(PT2 < MinPT)
                  continue;

               if(SubEvent >= 0 && SubEvent2 != SubEvent)
                  continue;

               double Weight = Weight1 * Weight2 * EventWeight;

               HEtaPhi.Fill(Eta1 - Eta2, GetDeltaPhi(Phi1, Phi2), 0.25 * Weight);
               HEtaPhi.Fill(Eta2 - Eta1, GetDeltaPhi(Phi1, Phi2), 0.25 * Weight);
               HEtaPhi.Fill(Eta1 - Eta2, -GetDeltaPhi(Phi1, Phi2), 0.25 * Weight);
               HEtaPhi.Fill(Eta2 - Eta1, -GetDeltaPhi(Phi1, Phi2), 0.25 * Weight);
               HEta.Fill(Eta1 - Eta2, 0.5 * Weight);
               HEta.Fill(Eta2 - Eta1, 0.5 * Weight);
               HPhi.Fill(+GetDeltaPhi(Phi1, Phi2), 0.5 * Weight);
               HPhi.Fill(-GetDeltaPhi(Phi1, Phi2), 0.5 * Weight);
               HPT1.Fill(max(PT1, PT2), Weight);
               HPT2.Fill(min(PT1, PT2), Weight);
            }
         }
         
         if(N1 > 0)
         {
            HCount.Fill(0.0, EventWeight);
            HCount.Fill(1.0, EventWeight * SumWeight1);
         }
      }
   }

   Bar.Update(EntryCount);
   Bar.Print();
   Bar.PrintLine();

   SignalFile.Close();
   BackgroundFile.Close();

   OutputFile.cd();

   HCount.Write();
   HZY.Write();
   HZPT.Write();
   HZMass.Write();
   HZHEta.Write();
   HZHPhi.Write();
   HZHPT.Write();
   HEtaPhi.Write();
   HEta.Write();
   HPhi.Write();
   HPT1.Write();
   HPT2.Write();

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


