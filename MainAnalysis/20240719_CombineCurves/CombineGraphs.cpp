#include <iostream>
#include <vector>
using namespace std;

#include "TFile.h"
#include "TH1D.h"
#include "TLegend.h"

#include "CommandLine.h"
#include "PlotHelper4.h"
#include "SetStyle.h"
#include "DataHelper.h"

int main(int argc, char *argv[]);
void DivideByBinWidth(TH1D *H);
double Integral(TH1D *H);

int main(int argc, char *argv[])
{
   vector<int> Colors = GetCVDColors8();

   SetThesisStyle();

   CommandLine CL(argc, argv);

   vector<string> InputFileNames = CL.GetStringVector("Input");
   vector<double> Coefficients   = CL.GetDoubleVector("Coefficients");
   vector<string> Histograms     = CL.GetStringVector("Histograms");
   string NormalizationState     = CL.Get("NormalizationState", "");
   vector<string> Normalization  = CL.GetStringVector("NormalizationKey", vector<string>{"", "", "", ""});
   vector<int> Rebin             = CL.GetIntVector("Rebin", vector<int>{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1});
   string OutputFileName         = CL.Get("Output");

   int N = InputFileNames.size();

   Coefficients.insert(Coefficients.end(), 1, N);
   Rebin.insert(Rebin.end(), 1, N);

   vector<TFile *> InputFiles(N);
   for(int i = 0; i < N; i++)
      InputFiles[i] = new TFile(InputFileNames[i].c_str());
   
   TFile OutputFile(OutputFileName.c_str(), "RECREATE");

   DataHelper DHFile("Normalization.dh");

   for(string H : Histograms)
   {
      vector<TH1D *> H1(N);
      vector<double> N1(N);
      for(int i = 0; i < N; i++)
      {
         H1[i] = (TH1D *)InputFiles[i]->Get(H.c_str());
         H1[i]->Rebin(Rebin[i]);
         TH1D *HCount = (TH1D *)InputFiles[i]->Get("HCount");
         H1[i]->Scale(1 / HCount->GetBinContent(1));
         if(NormalizationState != "" && Normalization[i] != "" && Normalization[i] != "None")
         {
            double Override = DHFile[NormalizationState][Normalization[i]].GetDouble();
            if(Override == 0)
               Override = 1;

            cout << "Normalization override!" << endl;
            cout << "Before: " << H1[i]->Integral(0, H1[i]->GetNbinsX() + 1) << endl;
            cout << "Target: " << Override << endl;
            H1[i]->Scale(Override / H1[i]->Integral(0, H1[i]->GetNbinsX() + 1));
            cout << "After: " << H1[i]->Integral(0, H1[i]->GetNbinsX() + 1) << endl;
         }
         DivideByBinWidth(H1[i]);
         N1[i] = HCount->GetBinContent(1);

         cout << "Histogram " << H << ", index " << i << ", integral = " << HCount->GetBinContent(1) << endl;
      }

      OutputFile.cd();
      
      TH1D *HDiff = (TH1D *)H1[0]->Clone(H.c_str());
      HDiff->Scale(Coefficients[0]);
      for(int i = 1; i < N; i++)
         HDiff->Add(H1[i], Coefficients[i]);

      HDiff->Write();
   }

   OutputFile.Close();

   for(int i = 0; i < N; i++)
   {
      InputFiles[i]->Close();
      delete InputFiles[i];
   }

   return 0;
}

void DivideByBinWidth(TH1D *H)
{
   if(H == nullptr)
      return;

   int N = H->GetNbinsX();
   for(int i = 1; i <= N; i++)
   {
      double L = H->GetXaxis()->GetBinLowEdge(i);
      double R = H->GetXaxis()->GetBinUpEdge(i);

      H->SetBinContent(i, H->GetBinContent(i) / (R - L));
      H->SetBinError(i, H->GetBinError(i) / (R - L));
   }
}

double Integral(TH1D *H)
{
   double Answer = 0;

   if(H == nullptr)
      return Answer;

   int N = H->GetNbinsX();
   for(int i = 1; i <= N; i++)
   {
      double L = H->GetXaxis()->GetBinLowEdge(i);
      double R = H->GetXaxis()->GetBinUpEdge(i);

      Answer = Answer + H->GetBinContent(i) * (R - L);
   }

   return Answer;
}



