#include <iostream>
#include <vector>
using namespace std;

#include "TH2D.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TH1D.h"
#include "TLegend.h"
#include "TGraphAsymmErrors.h"
#include "TLatex.h"

#include "SetStyle.h"
#include "CommandLine.h"

int main(int argc, char *argv[]);
void Transcribe(string FileName, string SysFileName, string Histogram,
   TGraphAsymmErrors &G, TGraphAsymmErrors &GSys, bool LogX);

int main(int argc, char *argv[])
{
   vector<int> DefaultColors = GetCVDColors6();

   SetThesisStyle();

   CommandLine CL(argc, argv);

   vector<string> InputFileNames = CL.GetStringVector("Input");
   vector<string> SystematicsFileNames = CL.GetStringVector("Systematics");
   vector<string> Labels = CL.GetStringVector("Label");
   vector<int> Colors = CL.GetIntegerVector("Color");
   vector<string> ExtraTexts = CL.GetStringVector("ExtraText", vector<string>{});
   string OutputFileName = CL.Get("Output");
   string Histogram = CL.Get("Histogram");
   bool DoLogX = CL.GetBool("DoLogX", false);
   bool DoLogY = CL.GetBool("DoLogY", false);
   double WorldYMax = CL.GetDouble("WorldYMax", 0);

   int NFile = InputFileNames.size();

   if(SystematicsFileNames.size() < NFile)
      SystematicsFileNames.insert(SystematicsFileNames.end(), NFile - SystematicsFileNames.size(), "File");
   if(Labels.size() < NFile)
      Labels.insert(Labels.end(), NFile - Labels.size(), "Unlabeled");
   if(Colors.size() < NFile)
      Colors.insert(Colors.end(), NFile - Labels.size(), 0);

   vector<TGraphAsymmErrors> G(NFile);
   vector<TGraphAsymmErrors> GSys(NFile);

   for(int iF = 0; iF < NFile; iF++)
      Transcribe(InputFileNames[iF], SystematicsFileNames[iF], Histogram, G[iF], GSys[iF], DoLogX);

   TCanvas Canvas;

   double YMax = (WorldYMax == 0) ? 5500 : WorldYMax;
   double YMin = -500;
   if(DoLogY == true)
   {
      YMin = 0.5;
      YMax = (WorldYMax == 0) ? 500000 : WorldYMax;
   }
   if(DoLogX == true && DoLogY == false)
   {
      YMin = -500;
      YMax = (WorldYMax == 0) ? 10000 : WorldYMax;
   }

   double XMin = 0;
   double XMax = 4;
   if(DoLogX == true)
      XMin = 0.01;

   TH2D HWorld("HWorld", ";#DeltaR;EEC", 100, XMin, XMax, 100, YMin, YMax);
   HWorld.SetStats(0);
   HWorld.Draw("axis");

   if(DoLogX == true)   Canvas.SetLogx();
   if(DoLogY == true)   Canvas.SetLogy();

   TLegend Legend(0.5, 0.8, 0.75, 0.8 - 0.07 * NFile);
   Legend.SetTextFont(42);
   Legend.SetTextSize(0.035);
   Legend.SetFillStyle(0);
   Legend.SetBorderSize(0);

   for(int iF = 0; iF < NFile; iF++)
   {
      int C = Colors[iF];
      if(C == -1)   C = DefaultColors[0];
      if(C == -2)   C = DefaultColors[1];
      if(C == -3)   C = DefaultColors[2];
      if(C == -4)   C = DefaultColors[3];
      if(C == -5)   C = DefaultColors[4];
      if(C == -6)   C = DefaultColors[5];

      GSys[iF].SetFillColorAlpha(C, 0.25);
      G[iF].SetMarkerColor(C);
      G[iF].SetMarkerStyle(20);
      G[iF].SetLineColor(C);
      G[iF].SetLineWidth(2);
      G[iF].SetFillColorAlpha(C, 0.25);

      GSys[iF].Draw("2");
      G[iF].Draw("p");

      Legend.AddEntry(&G[iF], Labels[iF].c_str(), "plf");
   }

   Legend.Draw();

   TLatex Latex;
   Latex.SetTextFont(42);
   Latex.SetTextSize(0.035);
   Latex.SetTextAlign(11);
   Latex.SetTextAngle(0);
   Latex.SetNDC();
   for(int i = 0; i < (int)ExtraTexts.size(); i++)
      Latex.DrawLatex(0.35, 0.80 - i * 0.07, ExtraTexts[i].c_str());

   Canvas.SaveAs(OutputFileName.c_str());

   return 0;
}

void Transcribe(string FileName, string SysFileName, string Histogram,
   TGraphAsymmErrors &G, TGraphAsymmErrors &GSys, bool LogX)
{
   int NBin = 0;

   TFile File(FileName.c_str());

   TH1D *H = (TH1D *)File.Get(Histogram.c_str());
   NBin = H->GetNbinsX();

   for(int iB = 1; iB <= NBin; iB++)
   {
      G.SetPoint(iB - 1, H->GetBinCenter(iB), H->GetBinContent(iB));
      G.SetPointError(iB - 1, 0, 0, H->GetBinError(iB), H->GetBinError(iB));
      GSys.SetPoint(iB - 1, H->GetBinCenter(iB), H->GetBinContent(iB));
      GSys.SetPointError(iB - 1, 0, 0, 0, 0);
   }

   File.Close();

   if(SysFileName != "None" && SysFileName != "")
   {
      TFile SysFile(SysFileName.c_str());

      TH1D *HSysBase = (TH1D *)SysFile.Get(Form("Total_Base_%s", Histogram.c_str()));
      TH1D *HSysVariant = (TH1D *)SysFile.Get(Form("Total_Variant_%s", Histogram.c_str()));

      if(HSysBase == nullptr || HSysVariant == nullptr)
         cerr << "Warning! Necessary histograms from " << SysFileName
            << " for " << Histogram << " not found." << endl;
      else
      {
         for(int iB = 1; iB <= NBin; iB++)
         {
            double B = HSysBase->GetBinContent(iB);
            double V = HSysVariant->GetBinContent(iB);

            double X = GSys.GetPointX(iB - 1);

            if(LogX == false)
               GSys.SetPointError(iB - 1, 0.01, 0.01, fabs(V - B), fabs(V - B));
            else
               GSys.SetPointError(iB - 1, X * 0.05, X * 0.05, fabs(V - B), fabs(V - B));
         }
      }

      SysFile.Close();
   }
}



