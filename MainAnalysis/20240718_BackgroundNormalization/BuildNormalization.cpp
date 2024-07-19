#include <iostream>
using namespace std;

#include "CommandLine.h"
#include "Messenger.h"

#include "TProfile.h"
#include "TF1.h"

int main(int argc, char *argv[])
{
   CommandLine CL(argc, argv);

   vector<string> InputFileName = CL.GetStringVector("Input");
   string OutputFileName = CL.Get("Output");
   double Fraction  = CL.GetDouble("Fraction", 1.00);
   double MinPT = CL.GetDouble("MinPT", 1);
   double MaxPT = CL.GetDouble("MaxPT", -1);
   double MaxAbsEta = CL.GetDouble("MaxAbsEta", -1);

   bool CheckZ = CL.GetBool("CheckZ", false);
   double MinZPT = CL.GetDouble("MinZPT", -1);
   bool DoShift = CL.GetBool("DoShift", false);
   string ShiftFileName = CL.Get("ShiftFile", "");

   TFile OutputFile(OutputFileName.c_str(), "RECREATE");

   TFile ShiftFile(ShiftFileName.c_str());
   TF1 *F = (TF1 *)ShiftFile.Get("HFShift");
   cout << F << endl;

   TProfile P("P", "", 1000, 0, 180000, "S");
   TH1D HN0("HN0", "", 1000, 0, 180000);
   TH1D HN1("HN1", "", 1000, 0, 180000);
   TH1D HN2("HN2", "", 1000, 0, 180000);
   TH1D HNN("HNN", "", 1000, 0, 180000);

   for(string FileName : InputFileName)
   {
      TFile InputFile(FileName.c_str());
      ZHadronMessenger M(InputFile, "Tree");

      int EntryCount = M.GetEntries() * Fraction;
      for(int iE = 0; iE < EntryCount; iE++)
      {
         M.GetEntry(iE);

         if(CheckZ == true)
         {
            if(M.GoodRecoZ == false)                  continue;
            if(MinZPT > 0 && M.zPt->at(0) < MinZPT)   continue;
         }

         int NTrack = (M.trackPt != nullptr) ? M.trackPt->size() : 0;

         double N = 0;
         double NN = 0;
         for(int iT = 0; iT < NTrack; iT++)
         {
            if(M.trackMuTagged->at(iT) == true)                         continue;
            if(M.trackPt->at(iT) < MinPT)                               continue;
            if(MaxPT > 0 && M.trackPt->at(iT) > MaxPT)                  continue;
            if(MaxAbsEta > 0 && fabs(M.trackEta->at(iT)) > MaxAbsEta)   continue;

            double W = M.trackWeight->at(iT);
            N = N + W * M.trackPt->at(iT);
            NN = NN + W * M.trackPt->at(iT) * M.trackPt->at(iT);
         }

         double HF = M.SignalHF;
         if(DoShift == true)
            HF = HF - F->Eval(HF);

         P.Fill(HF, N);
         HN0.Fill(HF, 1);
         HN1.Fill(HF, N);
         HN2.Fill(HF, N * N);
         HNN.Fill(HF, NN);
      }

      InputFile.Close();
   }
   
   ShiftFile.Close();

   OutputFile.cd();

   P.Write();
   // H.Write();
   HN0.Write();
   HN1.Write();
   HN2.Write();
   HNN.Write();

   OutputFile.Close();

   return 0;
}




