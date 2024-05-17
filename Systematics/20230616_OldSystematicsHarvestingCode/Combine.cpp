#include <iostream>
#include <vector>
using namespace std;

#include "TH1D.h"
#include "TFile.h"
#include "TKey.h"

#include "CommandLine.h"
#include "SetStyle.h"

int main(int argc, char *argv[]);
TH1D *GetHistogram(TFile *F, string ToPlot, string Tag, bool Direct);
void HistogramSelfSubtract(TH1D *H);

int main(int argc, char *argv[])
{
   CommandLine CL(argc, argv);

   vector<string> FileNames = CL.GetStringVector("File");
   string OutputFileName    = CL.Get("Output", "Sys.root");

   int NFile = FileNames.size();

   vector<TFile *> Files(NFile);
   for(int iF = 0; iF < NFile; iF++)
      Files[iF] = new TFile(FileNames[iF].c_str());
   TFile OutputFile(OutputFileName.c_str(), "RECREATE");

   // Go through the files and gather histogram names to be combined
   vector<string> Names;
   for(int iF = 0; iF < NFile; iF++)
   {
      TIter Iter(Files[iF]->GetListOfKeys());
      for(TKey *Key = (TKey *)Iter(); Key != nullptr; Key = (TKey *)Iter())
      {
         string Name = Key->ReadObj()->GetName();

         if(Key->ReadObj()->IsA()->InheritsFrom("TH1") == false)   // not histogram, skip
            continue;
         if(Name.rfind("HN_", 0) == 0)   // this is normalization histogram
            continue;

         Names.push_back(Name);
      }
   }
   sort(Names.begin(), Names.end());
   Names.erase(unique(Names.begin(), Names.end()), Names.end());

   for(string Name : Names)
   {
      TH1D *H = nullptr;
      for(int iF = 0; iF < NFile; iF++)
      {
         TH1D *HSingle = (TH1D *)Files[iF]->Get(Name.c_str());
         if(HSingle == nullptr)
            continue;

         if(H == nullptr)
         {
            OutputFile.cd();
            H = (TH1D *)HSingle->Clone(Name.c_str());
         }
         else
         {
            int N = H->GetNbinsX();
            for(int i = 1; i <= N; i++)
            {
               double V1 = H->GetBinContent(i);
               double V2 = HSingle->GetBinContent(i);
               H->SetBinContent(i, sqrt(V1 * V1 + V2 * V2));
            }
         }
      }

      OutputFile.cd();
      H->Write();
   }

   OutputFile.Close();
   for(TFile *F : Files)
   {
      if(F != nullptr)
      {
         F->Close();
         delete F;
      }
   }

   return 0;
}

TH1D *GetHistogram(TFile *F, string ToPlot, string Tag, bool Direct)
{
   TH1D *H  = (TH1D *)F->Get(Form("H%s_%s", ToPlot.c_str(), Tag.c_str()));
   if(H == nullptr)
      return nullptr;
   H = (TH1D *)H->Clone();

   if(Direct == false)
   {
      TH1D *HN = (TH1D *)F->Get(Form("HN_%s", Tag.c_str()));
      if(HN == nullptr)
         return nullptr;
      double Integral = HN->GetBinContent(1);

      H->Scale(1 / Integral);
      for(int i = 1; i <= H->GetNbinsX(); i++)
      {
         double XMin = H->GetXaxis()->GetBinLowEdge(i);
         double XMax = H->GetXaxis()->GetBinUpEdge(i);

         H->SetBinContent(i, H->GetBinContent(i) / (XMax - XMin));
         H->SetBinError(i, H->GetBinError(i) / (XMax - XMin));
      }
   }

   return H;
}

void HistogramSelfSubtract(TH1D *H)
{
   if(H == nullptr)
      return;

   double SumX = 0;
   double SumXY = 0;
   for(int i = 1; i <= H->GetNbinsX(); i++)
   {
      double XMin = H->GetXaxis()->GetBinLowEdge(i);
      double XMax = H->GetXaxis()->GetBinUpEdge(i);
      double Y = H->GetBinContent(i);

      SumX = SumX + (XMax - XMin);
      SumXY = SumXY + (XMax - XMin) * Y;
   }

   double Mean = SumXY / SumX;
   for(int i = 1; i <= H->GetNbinsX(); i++)
      H->SetBinContent(i, H->GetBinContent(i) - Mean);
}


