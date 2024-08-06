#include <iostream>
#include <iomanip>
using namespace std;

#include "TFile.h"
#include "TH1D.h"

#include "CommandLine.h"

int main(int argc, char *argv[])
{
   CommandLine CL(argc, argv);

   string InputFileName = CL.Get("Input");
   string DataFileName = CL.Get("Data");
   string HFFileName = CL.Get("HF");

   TFile InputFile(InputFileName.c_str());
   TFile DataFile(DataFileName.c_str());
   TFile HFFile(HFFileName.c_str());

   TH1D *HDY = (TH1D *)HFFile.Get("HDY");
   TH1D *HN0 = (TH1D *)InputFile.Get("HN0");
   TH1D *HN1 = (TH1D *)InputFile.Get("HN1");
   TH1D *HN2 = (TH1D *)InputFile.Get("HN2");
   TH1D *HNN = (TH1D *)InputFile.Get("HNN");
   TH1D *HD0 = (TH1D *)DataFile.Get("HN0");
   TH1D *HD1 = (TH1D *)DataFile.Get("HN1");
   
   double AnswerBBSame = 0;
   double AnswerBBDiff = 0;
   double AnswerSBDiff = 0;

   int NBin = HDY->GetNbinsX();

   for(int i = 1; i <= NBin; i++)
   {
      if(HN0->GetBinContent(i) == 0 && HDY->GetBinContent(i) != 0)
      {
         cout << "ERROR" << endl;
         continue;
      }

      if(HN0->GetBinContent(i) == 0 || HDY->GetBinContent(i) == 0)
         continue;

      double A1 = HN1->GetBinContent(i) / HN0->GetBinContent(i);
      double A2 = HN2->GetBinContent(i) / HN0->GetBinContent(i);
      double AA = HNN->GetBinContent(i) / HN0->GetBinContent(i);

      AnswerBBSame = AnswerBBSame + HDY->GetBinContent(i) * (A2 - AA);
   }

   for(int i = 1; i <= NBin; i++)
   {
      if(HN0->GetBinContent(i) == 0 || HDY->GetBinContent(i) == 0)
         continue;
   
      for(int j = 1; j <= NBin; j++)
      {
         if(HN0->GetBinContent(j) == 0 || HDY->GetBinContent(j) == 0)
            continue;

         double A1i = HN1->GetBinContent(i) / HN0->GetBinContent(i);
         double A1j = HN1->GetBinContent(j) / HN0->GetBinContent(j);

         AnswerBBDiff = AnswerBBDiff + HDY->GetBinContent(i) * HDY->GetBinContent(j) * A1i * A1j;
      }
   }
   
   for(int i = 1; i <= NBin; i++)
   {
      if(HN0->GetBinContent(i) == 0 || HDY->GetBinContent(i) == 0)
         continue;
   
      for(int j = 1; j <= NBin; j++)
      {
         if(HD0->GetBinContent(j) == 0 || HDY->GetBinContent(j) == 0)
            continue;

         double A1i = HN1->GetBinContent(i) / HN0->GetBinContent(i);
         double A1j = HD1->GetBinContent(j) / HD0->GetBinContent(j);

         AnswerSBDiff = AnswerSBDiff + HDY->GetBinContent(i) * HDY->GetBinContent(j) * A1i * A1j;
      }
   }

   double NZ = HDY->Integral(0, HDY->GetNbinsX() + 1);

   cout << "NZ = " << NZ << endl;
   cout << "SxB (diff) = " << AnswerSBDiff << " " << setprecision(10) << AnswerSBDiff / NZ / NZ << endl;
   cout << "BxB (same) = " << AnswerBBSame << " " << setprecision(10) << AnswerBBSame / NZ << endl;
   cout << "BxB (diff) = " << AnswerBBDiff << " " << setprecision(10) << AnswerBBDiff / NZ / NZ << endl;

   HFFile.Close();
   DataFile.Close();
   InputFile.Close();

   return 0;
}




