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

   string ReferenceFileName = CL.Get("Reference", "Root/Data.root");
   string VariationFileName = CL.Get("Variation", "Root/DataHiBinUp.root");
   string OutputFileName    = CL.Get("Output", "Sys.root");

   bool DirectMode          = CL.GetBool("DirectMode", false);
   string Scheme            = CL.Get("Scheme", "Difference");
   double Inflation         = CL.GetDouble("Inflation", 1.00);

   bool DoSubstitute        = CL.GetBool("DoSubstitute", false);
   string SubstituteFrom    = DoSubstitute ? CL.Get("SubstituteFrom") : "";
   string SubstituteTo      = DoSubstitute ? CL.Get("SubstituteTo") : "";
   if(SubstituteFrom == "")
      DoSubstitute = false;

   TFile VariationFile(VariationFileName.c_str());
   TFile ReferenceFile(ReferenceFileName.c_str());
   TFile OutputFile(OutputFileName.c_str(), "RECREATE");

   // Go through the referencefile!
   TIter Iter(ReferenceFile.GetListOfKeys());
   for(TKey *Key = (TKey *)Iter(); Key != nullptr; Key = (TKey *)Iter())
   {
      string Name = Key->ReadObj()->GetName();

      if(Key->ReadObj()->IsA()->InheritsFrom("TH1") == false)   // not histogram, skip
         continue;
      if(Name.rfind("HN_", 0) == 0)   // this is normalization histogram
         continue;

      int Split = Name.find("_");
      if(Split == string::npos)
         continue;

      string ToPlot = Name.substr(1, Split - 1);
      string VariationTag = Name.substr(Split + 1);
      string ReferenceTag = VariationTag;
      if(DoSubstitute == true)
      {
         int Location = ReferenceTag.find(SubstituteFrom);
         if(Location != string::npos)
            ReferenceTag.replace(Location, SubstituteFrom.size(), SubstituteTo);
      }

      // cout << ToPlot << endl;
      // cout << " " << ReferenceTag << endl;
      // cout << " " << VariationTag << endl;

      TH1D *HReference = GetHistogram(&ReferenceFile, ToPlot, ReferenceTag, DirectMode);
      TH1D *HVariation = GetHistogram(&VariationFile, ToPlot, VariationTag, DirectMode);
      
      // cout << HReference << " " << HVariation << endl;

      if(HReference == nullptr || HVariation == nullptr)   // WTF
         continue;

      if(DirectMode == false)
      {
         HistogramSelfSubtract(HReference);
         HistogramSelfSubtract(HVariation);
      }

      TH1D *HSystematics = (TH1D *)HVariation->Clone();
      int NBin = HSystematics->GetNbinsX();

      if(Scheme == "Envelope" || Scheme == "Maximum")
      {
         double Max = 0;
         for(int i = 1; i <= NBin; i++)
         {
            double Diff = fabs(HReference->GetBinContent(i) - HVariation->GetBinContent(i));
            if(Diff > Max)
               Max = Diff;
         }
         for(int i = 1; i <= NBin; i++)
            HSystematics->SetBinContent(i, Max);
      }
      else if(Scheme == "RMS")
      {
         double Sum0 = 0, Sum1 = 0, Sum2 = 0;
         for(int i = 1; i <= NBin; i++)
         {
            double XL = HReference->GetXaxis()->GetBinLowEdge(i);
            double XR = HReference->GetXaxis()->GetBinUpEdge(i);
            double Y = HReference->GetBinContent(i) - HVariation->GetBinContent(i);
            Sum0 = Sum0 + (XR - XL);
            Sum1 = Sum1 + (XR - XL) * Y;
            Sum2 = Sum2 + (XR - XL) * Y * Y;
         }
         double RMS = sqrt((Sum2 - Sum1 * Sum1 * Sum0) / Sum0);
         for(int i = 1; i <= NBin; i++)
            HSystematics->SetBinContent(i, RMS);
      }
      else if(Scheme == "Mean")
      {
         double Sum0 = 0, Sum1 = 0;
         for(int i = 1; i <= NBin; i++)
         {
            double XL = HReference->GetXaxis()->GetBinLowEdge(i);
            double XR = HReference->GetXaxis()->GetBinUpEdge(i);
            double Y = HReference->GetBinContent(i) - HVariation->GetBinContent(i);
            Sum0 = Sum0 + (XR - XL);
            Sum1 = Sum1 + (XR - XL) * Y;
         }
         double Mean = Sum1 / Sum0;
         for(int i = 1; i <= NBin; i++)
            HSystematics->SetBinContent(i, Mean);
      }
      else if(Scheme == "Difference")
      {
         for(int i = 1; i <= NBin; i++)
            HSystematics->SetBinContent(i, fabs(HReference->GetBinContent(i) - HVariation->GetBinContent(i)));
      }
      else if(Scheme == "DirectMaximum")
      {
         for(int i = 1; i <= NBin; i++)
            HSystematics->SetBinContent(i, max(HReference->GetBinContent(i), HVariation->GetBinContent(i)));
      }
      else if(Scheme == "DirectMean")
      {
         for(int i = 1; i <= NBin; i++)
            HSystematics->SetBinContent(i, (HReference->GetBinContent(i) + HVariation->GetBinContent(i)) / 2);
      }
      else if(Scheme == "DirectSum")
      {
         for(int i = 1; i <= NBin; i++)
            HSystematics->SetBinContent(i, (HReference->GetBinContent(i) + HVariation->GetBinContent(i)));
      }
      else if(Scheme == "DirectQuadraticSum")
      {
         for(int i = 1; i <= NBin; i++)
         {
            double V1 = HReference->GetBinContent(i);
            double V2 = HVariation->GetBinContent(i);
            HSystematics->SetBinContent(i, sqrt(V1 * V1 + V2 * V2));
         }
      }
      else
         cerr << "Note: scheme \"" << Scheme << "\" not supported yet." << endl;

      for(int i = 1; i <= NBin; i++)
         HSystematics->SetBinContent(i, HSystematics->GetBinContent(i) * Inflation);

      OutputFile.cd();
      HSystematics->Write();
   }

   OutputFile.Close();
   VariationFile.Close();
   ReferenceFile.Close();

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


