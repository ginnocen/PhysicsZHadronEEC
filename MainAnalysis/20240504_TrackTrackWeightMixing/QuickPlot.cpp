#include <iostream>
#include <vector>
using namespace std;

#include "TFile.h"
#include "TH1D.h"
#include "TLegend.h"

#include "CommandLine.h"
#include "PlotHelper4.h"
#include "SetStyle.h"

int main(int argc, char *argv[]);
void DivideByBinWidth(TH1D *H);
double Integral(TH1D *H);

int main(int argc, char *argv[])
{
   vector<int> Colors = GetCVDColors8();

   SetThesisStyle();

   CommandLine CL(argc, argv);

   vector<string> InputFileNames = CL.GetStringVector("Input");
   vector<string> Labels = CL.GetStringVector("Labels", InputFileNames);
   vector<double> Coefficients = CL.GetDoubleVector("Coefficients");
   vector<string> Histograms = CL.GetStringVector("Histograms");
   vector<double> NormalizationOverride = CL.GetDoubleVector("Normalization", vector<double>{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1});
   vector<bool> SelfMixing = CL.GetBoolVector("SelfMixing", vector<bool>{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1});
   vector<int> Rebin = CL.GetIntVector("Rebin", vector<int>{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1});
   string OutputFileName = CL.Get("Output");
   bool IncludeCombined = CL.GetBool("IncludeCombined", true);
   bool IncludeScaleUncertainty = CL.GetBool("IncludeScaleUncertainty", false);

   int N = InputFileNames.size();

   Coefficients.insert(Coefficients.end(), 1, N);
   Rebin.insert(Rebin.end(), 1, N);
   Labels.insert(Labels.end(), N, "");

   vector<TFile *> InputFiles(N);
   for(int i = 0; i < N; i++)
      InputFiles[i] = new TFile(InputFileNames[i].c_str());

   vector<TH2D *> H2(N);

   PdfFileHelper PdfFile(OutputFileName);
   PdfFile.AddTextPage("Good plots");

   vector<double> MixingScale;
   for(int i = 0; i < N; i++)
   {
      TH1D *HCount = (TH1D *)InputFiles[i]->Get("HCount");
      TH1D *HZ = (TH1D *)InputFiles[i]->Get("HZPT");
      MixingScale.push_back(1);
      // if(SelfMixing[i] == true)
      //    MixingScale.push_back(1);
      // else
      //    MixingScale.push_back(1 - HZ->Integral() / HCount->GetBinContent(1));
   }

   for(string H : Histograms)
   {
      PdfFile.AddTextPage(H);

      vector<TH1D *> H1(N);
      vector<double> N1(N);
      for(int i = 0; i < N; i++)
      {
         H1[i] = (TH1D *)InputFiles[i]->Get(H.c_str());
         H1[i]->Rebin(Rebin[i]);
         TH1D *HCount = (TH1D *)InputFiles[i]->Get("HCount");
         H1[i]->Scale(MixingScale[i] / HCount->GetBinContent(1));
         if(NormalizationOverride[i] > 0)
         {
            cout << "Normalization override!" << endl;
            cout << "Before: " << H1[i]->Integral(0, H1[i]->GetNbinsX() + 1) << endl;
            cout << "Target: " << NormalizationOverride[i] << endl;
            H1[i]->Scale(NormalizationOverride[i] / H1[i]->Integral(0, H1[i]->GetNbinsX() + 1));
            cout << "After: " << H1[i]->Integral(0, H1[i]->GetNbinsX() + 1) << endl;
         }
         DivideByBinWidth(H1[i]);
         N1[i] = HCount->GetBinContent(1);

         cout << "Histogram " << H << ", index " << i << ", integral = " << HCount->GetBinContent(1) << endl;

         // H2[i] = (TH2D *)InputFiles[i]->Get("HEtaPhi");
         // H2[i]->Scale(1 / HCount->GetBinContent(1));
      }

      for(int i = 0; i < N; i++)
      {
         H1[i]->SetTitle(Form("Sum of all entries = %.2f", Integral(H1[i])));
         H1[i]->SetStats(0);
         PdfFile.AddPlot(H1[i], "hist error", false, false, true, false);
         H1[i]->SetTitle("");
         cout << InputFileNames[i] << " " << Integral(H1[i]) << endl;
         // PdfFile.AddPlot(H2[i], "colz");
      }

      double WorldMax = 0;
      double WorldMin = 9999999;
      for(int i = 0; i < N; i++)
      {
         if(H1[i]->GetMaximum() == H1[i]->GetMaximum())
            WorldMax = max(WorldMax, H1[i]->GetMaximum());
         if(H1[i]->GetMinimum() == H1[i]->GetMinimum())
            WorldMin = min(WorldMin, H1[i]->GetMinimum());
      }
      
      TCanvas Canvas;
      if(H.find("Log") != string::npos)
         Canvas.SetLogx();

      double XMin = H1[0]->GetXaxis()->GetBinLowEdge(1);
      double XMax = H1[0]->GetXaxis()->GetBinUpEdge(H1[0]->GetNbinsX());

      TH2D HWorld("HWorld", ";;", 100, XMin, XMax, 100, WorldMax * -0.5, WorldMax * 2.5);
      HWorld.GetXaxis()->SetTitle(H1[0]->GetXaxis()->GetTitle());
      HWorld.SetStats(0);
      HWorld.Draw();

      TLegend Legend(0.2, 0.85, 0.5, 0.60);
      Legend.SetBorderSize(0);
      Legend.SetFillStyle(0);
      Legend.SetTextFont(42);
      Legend.SetTextSize(0.035);
      
      TLegend Legend2(0.2, 0.85, 0.5, 0.60);
      Legend2.SetBorderSize(0);
      Legend2.SetFillStyle(0);
      Legend2.SetTextFont(42);
      Legend2.SetTextSize(0.035);

      for(int i = 0; i < N; i++)
      {
         H1[i]->SetLineWidth(2);
         H1[i]->SetLineColor(Colors[i]);
         H1[i]->Draw("hist same");
         if(IncludeCombined == true)
         {
            Legend.AddEntry(H1[i], Form("%s (%.3f) [%.0g]", InputFileNames[i].c_str(), Integral(H1[i]), Coefficients[i]), "l");
            Legend2.AddEntry(H1[i], Form("%s [%.0g]", Labels[i].c_str(), Coefficients[i]), "l");
         }
         else
         {
            Legend.AddEntry(H1[i], Form("%s (%.3f)", InputFileNames[i].c_str(), Integral(H1[i])), "l");
            Legend2.AddEntry(H1[i], Form("%s", Labels[i].c_str()), "l");
         }
      }

      TH1D *HDiff = (TH1D *)H1[0]->Clone("HDiff");
      HDiff->SetLineWidth(1);
      // HDiff->SetLineColor(Colors[N]);
      HDiff->SetLineColor(kBlack);
      // HDiff->SetLineStyle(kDashed);
      HDiff->Scale(Coefficients[0]);
      for(int i = 1; i < N; i++)
         HDiff->Add(H1[i], Coefficients[i]);
      if(IncludeCombined == true)
      {
         Legend.AddEntry(HDiff, Form("Combined (%.3f)", Integral(HDiff)), "l");
         Legend2.AddEntry(HDiff, "Combined", "l");
      }
      
      TH1D *HDiffUp = (TH1D *)HDiff->Clone("HDiffUp");
      TH1D *HDiffDown = (TH1D *)HDiff->Clone("HDiffDown");
      HDiffUp->SetLineColor(kRed);
      HDiffDown->SetLineColor(kRed);
      for(int iB = 1; iB <= HDiff->GetNbinsX(); iB++)
      {
         double E2 = 0;
         for(int i = 0; i < N; i++)
            E2 = E2 + H1[i]->GetBinContent(iB) * H1[i]->GetBinContent(iB) / N1[i] * Coefficients[i] * Coefficients[i];
         HDiffUp->SetBinContent(iB, HDiff->GetBinContent(iB) + sqrt(E2));
         HDiffDown->SetBinContent(iB, HDiff->GetBinContent(iB) - sqrt(E2));
         // HDiff->SetBinError(iB, sqrt(E2));
      }

      HDiff->SetStats(0);
      if(IncludeScaleUncertainty == true)
      {
         cout << "!" << endl;
         HDiffUp->Draw("hist same");
         HDiffDown->Draw("hist same");
      }
      HDiff->Draw("hist same");
      // HDiffUp->Draw("hist same");
      // HDiffDown->Draw("hist same");

      Legend.Draw();

      PdfFile.AddCanvas(Canvas);

      Canvas.SetLogy();

      // WorldMax = max(WorldMax, HDiff->GetMaximum());
      // WorldMin = min(WorldMin, HDiff->GetMinimum());
      if(WorldMin <= 0)
         WorldMin = WorldMax * 0.00001;

      TH2D HWorld2("HWorld2", ";;", 100, XMin, XMax, 100, WorldMin / 50, WorldMax * 5000);
      HWorld2.GetXaxis()->SetTitle(H1[0]->GetXaxis()->GetTitle());
      HWorld2.SetStats(0);
      HWorld2.Draw();
      for(int i = 0; i < N; i++)
         H1[i]->Draw("hist same");
      if(IncludeCombined == true)
         HDiff->Draw("hist same");
      Legend.Draw();

      PdfFile.AddCanvas(Canvas);

      PdfFile.AddPlot(HDiff);
      PdfFile.AddPlot(HDiffUp);
      PdfFile.AddPlot(HDiffDown);

      HWorld2.Draw();
      for(int i = 0; i < N; i++)
         H1[i]->Draw("hist same");
      if(IncludeCombined == true)
      {
         if(IncludeScaleUncertainty == true)
         {
            HDiffUp->Draw("hist same");
            HDiffDown->Draw("hist same");
         }
         HDiff->Draw("hist same");
         // HDiffUp->Draw("hist same");
         // HDiffDown->Draw("hist same");
      }
      Legend2.Draw();

      Canvas.SaveAs(Form("%s_%s.pdf", OutputFileName.c_str(), H.c_str()));
      Canvas.SaveAs(Form("%s_%s.C", OutputFileName.c_str(), H.c_str()));

      PdfFile.AddCanvas(Canvas);

      HDiff->SetLineStyle(kSolid);
      HDiff->SetMarkerStyle(20);
      HDiff->SetMarkerColor(HDiff->GetLineColor());
      // PdfFile.AddPlot(HDiff, "hist");
      PdfFile.AddPlot(HDiff, "");
      
      TH2D HWorld3("HWorld3", ";;", 100, XMin, XMax, 100, 0, 2);
      HWorld3.GetXaxis()->SetTitle(H1[0]->GetXaxis()->GetTitle());
      HWorld3.GetYaxis()->SetTitle(Form("Ratio to %s", InputFileNames[0].c_str()));
      HWorld3.SetStats(0);
      HWorld3.Draw();
      for(int i = 0; i < N; i++)
      {
         TH1D *HRatio = (TH1D *)H1[i]->Clone(Form("HRatio%d", i));
         HRatio->SetMarkerStyle(20);
         HRatio->SetMarkerColor(HRatio->GetLineColor());
         HRatio->Divide(H1[0]);
         HRatio->Draw("same");
      }
      TH1D *HDiffRatio = (TH1D *)HDiff->Clone("HDiffRatio");
      HDiffRatio->Divide(H1[0]);
      HDiffRatio->Draw("hist same");

      Legend.Draw();
      Canvas.SetLogy(false);
      PdfFile.AddCanvas(Canvas);
   }

   PdfFile.AddTimeStampPage();
   PdfFile.Close();

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



