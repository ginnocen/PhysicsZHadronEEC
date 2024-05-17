#include <iostream>
#include <vector>
using namespace std;

#include "TCanvas.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TPad.h"
#include "TGaxis.h"
#include "TFile.h"
#include "TLatex.h"
#include "TGraph.h"
#include "TLegend.h"

#include "CommandLine.h"
#include "SetStyle.h"

int main(int argc, char *argv[]);
void SetPad(TPad *P);
void SetWorld(TH2D *H);
void SetAxis(TGaxis *A);
TH1D *GetHistogram(TFile *F, string ToPlot, string Tag, int Color);
TH1D *BuildSystematics(TFile *F, TH1D *H, string ToPlot, string Tag, int Color);
void HistogramSelfSubtract(TH1D *H);
//TH1D *SubtractHistogram(TH1D *H, TH1D *HRef);
void PrintHistogram(TFile *F, string Name);
void PrintHistogram(TH1D *H);

int main(int argc, char *argv[])
{
   vector<int> Colors = GetPrimaryColors();

   CommandLine CL(argc, argv);

   string OutputBase = CL.Get("OutputBase", "Plot");

   vector<string> DataFiles       = CL.GetStringVector("DataFiles", vector<string>{"pp.root","test.root"});
   bool SkipSystematics           = CL.GetBool("SkipSystematics", false);
   vector<string> SystematicFiles = (SkipSystematics == false) ? CL.GetStringVector("SystematicFiles") : vector<string>();
   vector<string> CurveLabels     = CL.GetStringVector("CurveLabels", vector<string>{"pp", "PbPb 0-30%"});
   string ToPlot                  = CL.Get("ToPlot", "DeltaPhi");
   vector<string> Tags            = CL.GetStringVector("Tags", vector<string> {
         "Result", "Result", "Result"
   });
   vector<string> SecondTags      = CL.GetStringVector("SecondTags", vector<string>());
   vector<string> Labels          = CL.GetStringVector("Labels",
   vector<string>
   {
      "1 < p_{T}^{trk} < 2 GeV",
      "2 < p_{T}^{trk} < 5 GeV",
      "5 < p_{T}^{trk} < 10 GeV",
      // "30-90%, 1 < p_{T}^{trk} < 2 GeV",
      // "30-90%, 2 < p_{T}^{trk} < 5 GeV",
      // "30-90%, 5 < p_{T}^{trk} < 10 GeV"
   });

   vector<string> ExtraInfo       = CL.GetStringVector("ExtraInfo",
   vector<string>
   {
      "p_{T}^{Z} > 40 GeV",
      ""
   });

   if(SystematicFiles.size() == 0)
      SkipSystematics = true;

   string PbPbLumi = "1.X nb^{-1}";
   string PPLumi = "3XX pb^{-1}";

   int NFile = DataFiles.size();
   int NColumn = Tags.size();

   double XMin = CL.GetDouble("XMin", 0);
   double XMax = CL.GetDouble("XMax", M_PI);
   double YMin = CL.GetDouble("YMin", -5);
   double YMax = CL.GetDouble("YMax", 10);
   double RMin = CL.GetDouble("RMin", -5);
   double RMax = CL.GetDouble("RMax", 5);

   int XAxisSpacing = 510;
   int YAxisSpacing = 510;
   int RAxisSpacing = 505;

   string XAxisLabel = CL.Get("XAxisLabel", "|#Delta#phi_{trk,Z}|");
   string YAxisLabel = CL.Get("YAxisLabel", "<#DeltaN_{ch}>/event");
   string RAxisLabel = CL.Get("RAxisLabel", "Difference to pp");

   double MarginLeft    = 100;
   double MarginRight   = 50;
   double MarginTop     = 50;
   double MarginBottom  = 100;
   double PadWidth      = 500;
   double PadHeight     = 500;
   double RPadHeight    = 200;

   double CanvasWidth   = MarginLeft + PadWidth * NColumn + MarginRight;
   double CanvasHeight  = MarginTop + PadHeight + RPadHeight + MarginBottom;

   double XMarginLeft   = MarginLeft / CanvasWidth;
   double XMarginRight  = MarginRight / CanvasWidth;
   double XMarginTop    = MarginTop / CanvasHeight;
   double XMarginBottom = MarginBottom / CanvasHeight;
   double XPadWidth     = PadWidth / CanvasWidth;
   double XPadHeight    = PadHeight / CanvasHeight;
   double XRPadHeight   = RPadHeight/ CanvasHeight;

   double LegendLeft    = CL.GetDouble("LegendLeft", 0.08);
   double LegendBottom  = CL.GetDouble("LegendBottom", 0.40);

   // Open input files
   vector<TFile *> File(NFile);
   for(int iF = 0; iF < NFile; iF++)
      File[iF] = new TFile(DataFiles[iF].c_str());
   
   vector<TFile *> SysFile(NFile);
   if(SkipSystematics == false)
   {
      for(int iF = 0; iF < NFile; iF++)
         SysFile[iF] = new TFile(SystematicFiles[iF].c_str());
   }

   // Setup canvas and pads
   TCanvas Canvas("Canvas", "", CanvasWidth, CanvasHeight);

   vector<TPad *> Pad(NColumn);
   vector<TPad *> RPad(NColumn);
   for(int iC = 0; iC < NColumn; iC++)
   {
      Pad[iC] = new TPad(Form("P%d", iC), "",
         XMarginLeft + XPadWidth * iC, XMarginBottom + XRPadHeight,
         XMarginLeft + XPadWidth * (iC + 1), XMarginBottom + XRPadHeight + XPadHeight);
      RPad[iC] = new TPad(Form("RP%d", iC), "",
         XMarginLeft + XPadWidth * iC, XMarginBottom,
         XMarginLeft + XPadWidth * (iC + 1), XMarginBottom + XRPadHeight);

      SetPad(Pad[iC]);
      SetPad(RPad[iC]);
   }

   // Setup world histograms
   vector<TH2D *> HWorld(NColumn);
   vector<TH2D *> HRWorld(NColumn);
   for(int iC = 0; iC < NColumn; iC++)
   {
      HWorld[iC] = new TH2D(Form("HWorld%d", iC), "", 100, XMin, XMax, 100, YMin, YMax);
      HRWorld[iC] = new TH2D(Form("HRWorld%d", iC), "", 100, XMin, XMax, 100, RMin, RMax);
      
      SetWorld(HWorld[iC]);
      SetWorld(HRWorld[iC]);
   }

   // Setup axes
   Canvas.cd();
   vector<TGaxis *> XAxis(NColumn), YAxis(1), RAxis(1);
   for(int iC = 0; iC < NColumn; iC++)
   {
      XAxis[iC] = new TGaxis(XMarginLeft + XPadWidth * iC, XMarginBottom,
                             XMarginLeft + XPadWidth * (iC + 1), XMarginBottom,
                             XMin, XMax, XAxisSpacing, "S");
      XAxis[iC]->SetTickSize(0.03 / XPadWidth /*CanvasWidth / CanvasHeight*/);
      SetAxis(XAxis[iC]);
   }
   YAxis[0] = new TGaxis(XMarginLeft, XMarginBottom + XRPadHeight,
                         XMarginLeft, XMarginBottom + XRPadHeight + XPadHeight,
                         YMin, YMax, YAxisSpacing, "S");
   YAxis[0]->SetTickSize(0.03 / XPadHeight * CanvasHeight / CanvasWidth);
   SetAxis(YAxis[0]);
   RAxis[0] = new TGaxis(XMarginLeft, XMarginBottom,
                         XMarginLeft, XMarginBottom + XRPadHeight,
                         RMin, RMax, RAxisSpacing, "S");
   RAxis[0]->SetTickSize(0.03 / XRPadHeight * CanvasHeight / CanvasWidth);
   SetAxis(RAxis[0]);

   // Setup axis labels
   TLatex Latex;
   Latex.SetTextFont(42);
   Latex.SetTextSize(0.035);
   Latex.SetNDC();

   for(int iC = 0; iC < NColumn; iC++)
   {
      Latex.SetTextAngle(0);
      Latex.SetTextAlign(22);
      Latex.DrawLatex(XMarginLeft + XPadWidth * (iC + 0.5), XMarginBottom * 0.4, XAxisLabel.c_str());
   }
   Latex.SetTextAngle(90);
   Latex.DrawLatex(XMarginLeft * 0.35, XMarginBottom + XRPadHeight + XPadHeight * 0.5, YAxisLabel.c_str());
   Latex.DrawLatex(XMarginLeft * 0.35, XMarginBottom + XRPadHeight * 0.5, RAxisLabel.c_str());

   // Add decorations
   Latex.SetTextAngle(0);
   Latex.SetTextAlign(11);
   Latex.DrawLatex(XMarginLeft, XMarginBottom + XRPadHeight + XPadHeight + 0.01,
      "#font[62]{CMS} #font[52]{Preliminary}");
   Latex.SetTextAlign(31);
   Latex.DrawLatex(XMarginLeft + XPadWidth * NColumn, XMarginBottom + XRPadHeight + XPadHeight + 0.01,
      Form("PbPb (pp) 5.02 TeV %s (%s)", PbPbLumi.c_str(), PPLumi.c_str()));

   // Retrieve histograms
   vector<vector<TH1D *>> HData(NColumn);
   for(int iC = 0; iC < NColumn; iC++)
   {
      HData[iC].resize(NFile);

      for(int iF = 0; iF < NFile; iF++)
      {
         string Tag = Tags[iC];
         if(SecondTags.size() == NColumn && iF == 1)
            Tag = SecondTags[iC];
         //cout <<"Get:" <<File[iF]<<" "<< ToPlot<<" "<< Tag<<" "<< Colors[iF]<<endl;
         HData[iC][iF] = GetHistogram(File[iF], ToPlot, Tag, Colors[iF]);
      }
   }
   
   // Retrieve systematics
   vector<vector<TH1D *>> HDataSys(NColumn);
   if(SkipSystematics == false)
   {
      for(int iC = 0; iC < NColumn; iC++)
      {
         HDataSys[iC].resize(NFile);
         
         for(int iF = 0; iF < NFile; iF++)
         {
            string Tag = Tags[iC];
            if(SecondTags.size() == NColumn && iF == 1)
               Tag = SecondTags[iC];
            HDataSys[iC][iF] = BuildSystematics(SysFile[iF], HData[iC][iF], ToPlot, Tag, Colors[iF]);
         }   
      }

   }

   // Setup legend
   TLegend Legend(LegendLeft, LegendBottom, LegendLeft + 0.40, LegendBottom + NFile * 0.1);
   Legend.SetTextFont(42);
   Legend.SetTextSize(0.035 * CanvasHeight / PadHeight);
   Legend.SetBorderSize(0);
   Legend.SetFillStyle(0);

   // Draw things
   for(int iC = 0; iC < NColumn; iC++)
   {
      Pad[iC]->cd();
      HWorld[iC]->Draw("axis");
      cout <<iC<<endl;
      for(int iF = 0; iF < NFile; iF++)
      {
         if(SkipSystematics == false && HDataSys[iC][iF] != nullptr) HDataSys[iC][iF]->Draw("same e2");
	 HData[iC][iF]->Draw("same");
	 cout <<iF<<endl;
      }

      Latex.SetTextAngle(0);
      Latex.SetTextAlign(33);
      Latex.SetTextSize(0.035 * CanvasHeight / PadHeight);
      Latex.DrawLatex(0.97, 0.97, Labels[iC].c_str());

      if(iC == 0)   // adding extra info!
      {
         for(int i = 0; i < (int)ExtraInfo.size(); i++)
         {
            Latex.SetTextAngle(0);
            Latex.SetTextAlign(11);
            Latex.SetTextSize(0.035 * CanvasHeight / PadHeight);
            Latex.DrawLatex(0.08, 0.85 - i * 0.075, ExtraInfo[i].c_str());
         }
      }
      if(iC == NColumn - 1)   // adding legend!
      {
         for(int iF = 0; iF < NFile; iF++)
            Legend.AddEntry(HData[iC][iF], CurveLabels[iF].c_str(), "pl");
         Legend.Draw();
      }


      // Draw difference
      RPad[iC]->cd();
      HRWorld[iC]->Draw("axis");
      
      for(int iF = 0; iF < NFile; iF++)
      {
         if(SkipSystematics == false && HDataSys[iC][iF] != nullptr) {
            TH1D *hDiffSys = (TH1D*) HDataSys[iC][iF]->Clone(Form("hDiffSys_%d_%d",iC,iF));
   	    hDiffSys->Add(HDataSys[iC][0], -1);
	    hDiffSys->Draw("same e2");
	 }   
	    
         TH1D *hDiff = (TH1D*) HData[iC][iF]->Clone(Form("hDiff_%d_%d",iC,iF));
	 hDiff->Add(HData[iC][0], -1);
	 hDiff->Draw("same");
      }
   }

   // Finally we have the plots
   Canvas.SaveAs((OutputBase + ".pdf").c_str());

   // Close input files
   for(int iF = 0; iF < NFile; iF++)
   {
      if(SkipSystematics == false && SysFile[iF] != nullptr)
      {
         SysFile[iF]->Close();
         delete SysFile[iF];
      }
   }
   for(int iF = 0; iF < NFile; iF++)
   {
      if(File[iF] != nullptr)
      {
         File[iF]->Close();
         delete File[iF];
      }
   }

   return 0;
}

void SetPad(TPad *P)
{
   if(P == nullptr)
      return;
   P->SetLeftMargin(0);
   P->SetTopMargin(0);
   P->SetRightMargin(0);
   P->SetBottomMargin(0);
   P->SetTickx(false);
   P->SetTicky(false);
   P->Draw();
}

void SetWorld(TH2D *H)
{
   if(H == nullptr)
      return;
   H->SetStats(0);
   H->GetXaxis()->SetTickLength(0);
   H->GetYaxis()->SetTickLength(0);
}

void SetAxis(TGaxis *A)
{
   if(A == nullptr)
      return;
   A->SetLabelFont(42);
   A->SetLabelSize(0.035);
   A->SetMaxDigits(6);
   A->SetMoreLogLabels();
   A->Draw();
}

TH1D *GetHistogram(TFile *F, string ToPlot, string Tag, int Color)
{
   TH1D *H  = (TH1D *)F->Get(Form("%s_%s", ToPlot.c_str(), Tag.c_str()));
   if(H == nullptr)
      return nullptr;

   static int ID = 0;
   ID = ID + 1;
   H = (TH1D *)H->Clone(Form("H%d", ID));

   double Integral = 1;//HN->GetBinContent(1);

   H->Scale(1 / Integral);
   for(int i = 1; i <= H->GetNbinsX(); i++)
   {
      double XMin = H->GetXaxis()->GetBinLowEdge(i);
      double XMax = H->GetXaxis()->GetBinUpEdge(i);

      H->SetBinContent(i, H->GetBinContent(i) / (XMax - XMin));
      H->SetBinError(i, H->GetBinError(i) / (XMax - XMin));
   }

   H->SetStats(0);
   H->SetMarkerStyle(20);
   H->SetLineWidth(2);
   H->SetMarkerSize(2);
   
   H->SetMarkerColor(Color);
   H->SetLineColor(Color);
   
   return H;
}

TH1D *BuildSystematics(TFile *F, TH1D *H, string ToPlot, string Tag, int Color)
{
   static int ID = 0;
   ID = ID + 1;
   TH1D *HSys = (TH1D *)(F->Get(Form("%s_%s", ToPlot.c_str(), Tag.c_str())));
   HSys = (TH1D*)HSys->Clone(Form("HSysValue%d",ID));
   if(HSys == nullptr)
      return nullptr;

   for(int i = 1; i <= H->GetNbinsX(); i++)
   {
      double XMin = H->GetXaxis()->GetBinLowEdge(i);
      double XMax = H->GetXaxis()->GetBinUpEdge(i);

      HSys->SetBinContent(i, HSys->GetBinContent(i) / (XMax - XMin));
      HSys->SetBinError(i, HSys->GetBinError(i) / (XMax - XMin));
   }
   
   TH1D *HResult = (TH1D *)H->Clone(Form("HSys%d", ID));
   for(int i = 1; i <= H->GetNbinsX(); i++)
      HResult->SetBinError(i, HSys->GetBinContent(i));
   HResult->SetFillColorAlpha(Color, 0.25);

   return HResult;
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

TH1D *SubtractHistogram(TH1D *H, TH1D *HRef)
{
   int N = H->GetNbinsX();

   static int ID = 0;
   ID = ID + 1;
   TH1D *HDiff = (TH1D *)H->Clone(Form("HDiff%d", ID));

   for(int i = 1; i <= N; i++)
   {
      HDiff->SetBinContent(i, H->GetBinContent(i) - HRef->GetBinContent(i));
      HDiff->SetBinError(i, H->GetBinError(i));
   }

   return HDiff;
}

void PrintHistogram(TFile *F, string Name)
{
   if(F == nullptr)
      return;
   TH1D *H = (TH1D *)F->Get(Name.c_str());
   if(H == nullptr)
      return;

   cout << F->GetName() << " " << Name << endl;
   PrintHistogram(H);
}

void PrintHistogram(TH1D *H)
{
   if(H == nullptr)
      return;
   for(int i = 1; i <= H->GetNbinsX(); i++)
      cout << i << " " << H->GetBinContent(i) << endl;
}