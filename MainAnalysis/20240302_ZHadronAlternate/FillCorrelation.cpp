#include <iostream>
using namespace std;

#include "TFile.h"
#include "TTree.h"
#include "TH2D.h"

#include "CommandLine.h"
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
   bool IsPP                 = CL.GetBool("IsPP", false);
   double HiBinMin           = CL.GetDouble("HiBinMin", 0);
   double HiBinMax           = CL.GetDouble("HiBinMax", 60);
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

   int EntryCount = MZHadron.GetEntries();
   int BackgroundEntryCount = MZHadronBackground.GetEntries();
   
   int BackgroundIndex = (BackgroundEntryCount > 0) ? (rand() % BackgroundEntryCount) : 0;

   cout << EntryCount << endl;
   for(int iE = 0; iE < EntryCount; iE++)
   {
      MZHadron.GetEntry(iE);

      if(IsPP == false)
      {
         if(MZHadron.hiBin < HiBinMin)
            continue;
         if(MZHadron.hiBin >= HiBinMax)
            continue;
      }

      if(IsPP == true && MZHadron.NVertex != 1)
         continue;
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

         int N = (MZHadron.trackPt != nullptr) ? MZHadron.trackPt->size() : 0;
         if(SelfMixingMode == false)
            N = (MZHadronBackground.trackPt != nullptr) ? MZHadronBackground.trackPt->size() : 0;

         double Eta1 = MZHadron.zY->at(0);
         double Phi1 = MZHadron.zPhi->at(0);

         double EventWeight = MZHadron.VZWeight * MZHadron.ZWeight;

         if(N > 0)
            HCount.Fill(0.0, EventWeight);

         for(int iP = 0; iP < N; iP++)
         {
            double Eta2 = SelfMixingMode ? MZHadron.trackEta->at(iP) : MZHadronBackground.trackEta->at(iP);
            double Phi2 = SelfMixingMode ? MZHadron.trackPhi->at(iP) : MZHadronBackground.trackPhi->at(iP);
            double Weight = SelfMixingMode ? MZHadron.trackWeight->at(iP) : MZHadronBackground.trackWeight->at(iP);
            Weight = Weight * EventWeight;

            HEtaPhi.Fill(Eta1 - Eta2, GetDeltaPhi(Phi1, Phi2), 0.25 * Weight);
            HEtaPhi.Fill(Eta2 - Eta1, GetDeltaPhi(Phi1, Phi2), 0.25 * Weight);
            HEtaPhi.Fill(Eta1 - Eta2, -GetDeltaPhi(Phi1, Phi2), 0.25 * Weight);
            HEtaPhi.Fill(Eta2 - Eta1, -GetDeltaPhi(Phi1, Phi2), 0.25 * Weight);
            HPhi.Fill(+GetDeltaPhi(Phi1, Phi2), 0.5 * Weight);
            HPhi.Fill(-GetDeltaPhi(Phi1, Phi2), 0.5 * Weight);
         }
      }
   }

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


