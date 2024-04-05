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
   bool IsPP                 = CL.GetBool("IsPP", false);
   bool CheckZ               = CL.GetBool("CheckZ", true);
   bool CheckSignalHiBin     = CL.GetBool("CheckSignalHiBin", true);
   double HiBinMin           = CL.GetDouble("HiBinMin", 0);
   double HiBinMax           = CL.GetDouble("HiBinMax", 60);
   bool CheckSignalHF        = CL.GetBool("CheckSignalHF", false);
   double SignalHFMin        = CL.GetDouble("SignalHFMin", 38220.226);
   double SignalHFMax        = CL.GetDouble("SignalHFMax", 155015.70);
   double HFShift            = CL.GetDouble("HFShift", 767.2);
   int OverSample            = SelfMixingMode ? 1 : CL.GetInt("OverSample", 100);

   TFile OutputFile(OutputFileName.c_str(), "RECREATE");

   TH1D HCount("HCount", ";", 10, 0, 10);
   TH2D HEtaPhi("HEtaPhi", ";#Delta#eta;#Delta#phi", 100, -3, 3, 100, -M_PI, M_PI);
   TH1D HPhi("HPhi", ";#Delta#phi", 100, -M_PI, M_PI);

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

      if(IsPP == true && MZHadron.NVertex != 1)
         continue;
      if(CheckZ == true)
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

         int N1 = (MZHadron.trackPt != nullptr) ? MZHadron.trackPt->size() : 0;
         int N2 = N1;
         if(SelfMixingMode == false)
            N2 = (MZHadronBackground.trackPt != nullptr) ? MZHadronBackground.trackPt->size() : 0;

         double EventWeight = MZHadron.VZWeight * MZHadron.ZWeight;

         double SumWeight1 = 0;

         for(int iP1 = 0; iP1 < N1; iP1++)
         {
            double Eta1 = MZHadron.trackEta->at(iP1);
            double Phi1 = MZHadron.trackPhi->at(iP1);
            double Weight1 = MZHadron.trackWeight->at(iP1);
            SumWeight1 = SumWeight1 + Weight1;

            for(int iP2 = 0; iP2 < N2; iP2++)
            {
               if(SelfMixingMode == true && iP1 == iP2)
                  continue;

               double Eta2 = SelfMixingMode ? MZHadron.trackEta->at(iP2) : MZHadronBackground.trackEta->at(iP2);
               double Phi2 = SelfMixingMode ? MZHadron.trackPhi->at(iP2) : MZHadronBackground.trackPhi->at(iP2);
               double Weight2 = SelfMixingMode ? MZHadron.trackWeight->at(iP2) : MZHadronBackground.trackWeight->at(iP2);
               
               double Weight = Weight1 * Weight2 * EventWeight;

               HEtaPhi.Fill(Eta1 - Eta2, GetDeltaPhi(Phi1, Phi2), 0.25 * Weight);
               HEtaPhi.Fill(Eta2 - Eta1, GetDeltaPhi(Phi1, Phi2), 0.25 * Weight);
               HEtaPhi.Fill(Eta1 - Eta2, -GetDeltaPhi(Phi1, Phi2), 0.25 * Weight);
               HEtaPhi.Fill(Eta2 - Eta1, -GetDeltaPhi(Phi1, Phi2), 0.25 * Weight);
               HPhi.Fill(+GetDeltaPhi(Phi1, Phi2), 0.5 * Weight);
               HPhi.Fill(-GetDeltaPhi(Phi1, Phi2), 0.5 * Weight);
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
   HEtaPhi.Write();
   HPhi.Write();

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


