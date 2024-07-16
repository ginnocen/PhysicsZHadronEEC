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
#include "utilities.h"
#include "usageMessage.h"

void NormalizeHistogram(TH1D *h, TH1D *h2) {
    if (!h || !h2) {
        cerr << "Error: One or both of the histograms are null." << endl;
        return;
    }

    double integral_h = h->Integral();
    double integral_h2 = h2->Integral();

    if (integral_h == 0 || integral_h2 == 0) {
        cerr << "Error: One or both of the histograms have zero integral." << endl;
        return;
    }

    double shift = (integral_h2 - integral_h) / h->GetNbinsX();
    cout <<"shift="<<shift<<endl;

    for (int i = 1; i <= h->GetNbinsX(); ++i) {
        h->SetBinContent(i, h->GetBinContent(i) + shift);
    }
}

int main(int argc, char *argv[]);

TH1D *BuildSystematics(TFile *F, TH1D *H, string ToPlot, string Tag, int Color) {
   static int ID = 0;
   ID = ID + 1;
   TH1D *HSys = (TH1D *)(F->Get(Form("%s_%s", ToPlot.c_str(), Tag.c_str())));
   HSys = (TH1D*)HSys->Clone(Form("HSysValue%d",ID));
   if(HSys == nullptr)
      return nullptr;

   TH1D *HResult = (TH1D *)H->Clone(Form("HSys%d", ID));
   for(int i = 1; i <= H->GetNbinsX(); i++)
      HResult->SetBinError(i, HSys->GetBinContent(i));
   HResult->SetFillColorAlpha(Color, 0.25);

   return HResult;
}

int main(int argc, char *argv[]) {


   // ========================================================================================================
   // Setting from input command
   // ========================================================================================================
   if (argc > 1 && string(argv[1]) == "--help") {
       PrintUsage();
       return 0;
   }
   vector<int> Colors = GetCVDColors6();

   CommandLine CL(argc, argv);

   string OutputBase = CL.Get("OutputBase", "Plot");

   vector<string> DataFiles       = CL.GetStringVector("DataFiles", vector<string>{"pp.root","test.root"});

   bool SkipSystematics           = CL.GetBool("SkipSystematics", false);
   bool SkipMixFile               = CL.GetBool("SkipMixFile", true);
   bool PlotDiff                  = CL.GetBool("PlotDiff", true);
   int Rebin                      = CL.GetInt("Rebin", 1);
   
   
   vector<string> SystematicFiles = (SkipSystematics == false) ? CL.GetStringVector("SystematicFiles") : vector<string>();
   vector<string> MixFiles = (SkipMixFile == false) ? CL.GetStringVector("mixFiles") : vector<string>();
   vector<string> CurveLabels     = CL.GetStringVector("CurveLabels", vector<string>{"pp", "PbPb 0-30%"});
   string ToPlot                  = CL.Get("ToPlot", "DeltaPhi");
   vector<int>    lines           = CL.GetIntVector("lines", vector<int>{0,0,1,1,1,1,1});
   vector<string> Tags            = CL.GetStringVector("Tags", vector<string> {
         "Result1_2", "Result2_4", "Result4_10"
   });
   vector<string> SecondTags      = CL.GetStringVector("SecondTags", vector<string>());
   vector<string> Labels          = CL.GetStringVector("Labels", vector<string> {
      "1 < p_{T}^{trk} < 2 GeV",
      "2 < p_{T}^{trk} < 4 GeV",
      "4 < p_{T}^{trk} < 10 GeV",
   });

   vector<string> ExtraInfo       = CL.GetStringVector("ExtraInfo", vector<string> {
      "p_{T}^{Z} > 40 GeV",
      ""
   });

   if(SystematicFiles.size() == 0) SkipSystematics = true;

   string PbPbLumi = "1.67 nb^{-1}";
   string PPLumi = "301 pb^{-1}";

   int NFile = DataFiles.size();
   int NPair = DataFiles.size()/2.;
   int NColumn = Tags.size();
   cout <<"NFile: "<<NFile<<" NPair"<<NPair<<endl;

   double XMin = CL.GetDouble("XMin", 0);
   double XMax = CL.GetDouble("XMax", M_PI);
   double YMin = CL.GetDouble("YMin", -5);
   double YMax = CL.GetDouble("YMax", 5);
   double RMin = CL.GetDouble("RMin", -5);
   double RMax = CL.GetDouble("RMax", 5);

   int XAxisSpacing = 510;
   int YAxisSpacing = 510;
   int RAxisSpacing = 505;

   string XAxisLabel = CL.Get("XAxisLabel", "|#Delta#phi_{trk,Z}|");
   string YAxisLabel = CL.Get("YAxisLabel", "d<#DeltaN_{ch}>/d#Delta#phi_{trk,Z}");
   string RAxisLabel = CL.Get("RAxisLabel", "PbPb - pp");

   double MarginLeft    = 100;
   double MarginRight   = 50;
   double MarginTop     = 50;
   double MarginBottom  = 100;
   double PadWidth      = 500;
   double PadHeight     = 500;
   double RPadHeight    = 200;
   
   if (!PlotDiff) {
      RPadHeight = 0;
      PadHeight = 500;
   }
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
   double LegendBottom  = CL.GetDouble("LegendBottom", 0.50);

   // ========================================================================================================
   // Open input files
   // ========================================================================================================
   vector<TFile *> File(NFile);
   for(int iF = 0; iF < NFile; iF++) File[iF] = new TFile(DataFiles[iF].c_str());
   
   vector<TFile *> SysFile(NFile);
   if(SkipSystematics == false) {
      for(int iF = 0; iF < NFile; iF++)
         SysFile[iF] = new TFile(SystematicFiles[iF].c_str());
   }

  vector<TFile *> MixFile(NFile);
   if(SkipMixFile == false) {
      for(int iF = 0; iF < NFile; iF++){
         MixFile[iF] = new TFile(MixFiles[iF].c_str());
	 cout <<MixFiles[iF].c_str()<<endl;}
   }

   // ========================================================================================================
   // Setup canvas and pads
   // ========================================================================================================
   TCanvas Canvas("Canvas", "", CanvasWidth, CanvasHeight);

   vector<TPad *> Pad(NColumn);
   vector<TPad *> RPad(NColumn);
   for(int iC = 0; iC < NColumn; iC++) {
      Pad[iC] = new TPad(Form("P%d", iC), "",
         XMarginLeft + XPadWidth * iC, XMarginBottom + XRPadHeight,
         XMarginLeft + XPadWidth * (iC + 1), XMarginBottom + XRPadHeight + XPadHeight);
      if (PlotDiff) RPad[iC] = new TPad(Form("RP%d", iC), "",
         XMarginLeft + XPadWidth * iC, XMarginBottom,
         XMarginLeft + XPadWidth * (iC + 1), XMarginBottom + XRPadHeight);

      SetPad(Pad[iC]);
      if (PlotDiff) SetPad(RPad[iC]);
   }

   // ========================================================================================================
   // Setup world histograms
   // ========================================================================================================
   vector<TH2D *> HWorld(NColumn);
   vector<TH2D *> HRWorld(NColumn);
   cout <<"XMin: "<<XMin<<" "<<"XMax: "<<XMax<<endl;
   for(int iC = 0; iC < NColumn; iC++) {
      HWorld[iC] = new TH2D(Form("HWorld%d", iC), "", 100, XMin, XMax, 100, YMin, YMax);
      HRWorld[iC] = new TH2D(Form("HRWorld%d", iC), "", 100, XMin, XMax, 100, RMin, RMax);
      
      SetWorld(HWorld[iC]);
      if (PlotDiff) SetWorld(HRWorld[iC]);
   }

   // ========================================================================================================
   // Setup axes
   // ========================================================================================================
   Canvas.cd();
   vector<TGaxis *> XAxis(NColumn), YAxis(1), RAxis(1);
   for(int iC = 0; iC < NColumn; iC++) {
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
   if (PlotDiff) {
      RAxis[0] = new TGaxis(XMarginLeft, XMarginBottom,
                         XMarginLeft, XMarginBottom + XRPadHeight,
                         RMin, RMax, RAxisSpacing, "S");
      RAxis[0]->SetTickSize(0.03 / XRPadHeight * CanvasHeight / CanvasWidth);
      SetAxis(RAxis[0]);
   }
   
   // ========================================================================================================
   // Setup axis labels
   // ========================================================================================================
   TLatex Latex;
   Latex.SetTextFont(42);
   Latex.SetTextSize(0.035);
   Latex.SetNDC();

   for(int iC = 0; iC < NColumn; iC++) {
      Latex.SetTextAngle(0);
      Latex.SetTextAlign(22);
      Latex.DrawLatex(XMarginLeft + XPadWidth * (iC + 0.5), XMarginBottom * 0.4, XAxisLabel.c_str());
   }
   Latex.SetTextAngle(90);
   Latex.DrawLatex(XMarginLeft * 0.35, XMarginBottom + XRPadHeight + XPadHeight * 0.5, YAxisLabel.c_str());
   if (PlotDiff) Latex.DrawLatex(XMarginLeft * 0.35, XMarginBottom + XRPadHeight * 0.5, RAxisLabel.c_str());

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
   for(int iC = 0; iC < NColumn; iC++) {
      HData[iC].resize(NFile);

      for(int iF = 0; iF < NFile; iF++) {
         string Tag = Tags[iC];
         if(SecondTags.size() == NColumn && iF == 1) Tag = SecondTags[iC];
         HData[iC][iF] = GetHistogram(File[iF], ToPlot, Tag, Colors[iF]);
      }
   }
   
   // Retrieve systematics
   vector<vector<TH1D *>> HDataSys(NColumn);
   if(SkipSystematics == false) {
      for(int iC = 0; iC < NColumn; iC++) {
         HDataSys[iC].resize(NFile);
         for(int iF = 0; iF < NFile; iF++) {
            string Tag = Tags[iC];
            if(SecondTags.size() == NColumn && iF == 1)
               Tag = SecondTags[iC];
            HDataSys[iC][iF] = BuildSystematics(SysFile[iF], HData[iC][iF], ToPlot, Tag, Colors[iF]);
         }   
      }
   }
   
  // Retrieve mix files
   vector<vector<TH1D *>> HMix(NColumn);
   if(SkipMixFile == false) {
      for(int iC = 0; iC < NColumn; iC++) {
         HMix[iC].resize(NFile);
         for(int iF = 0; iF < NFile; iF++) {
            string Tag = Tags[iC];
            if(SecondTags.size() == NColumn && iF == 1)
               Tag = SecondTags[iC];
            HMix[iC][iF] = GetHistogram(MixFile[iF], ToPlot, Tag, Colors[iF]);
	    NormalizeHistogram(HData[iC][iF],HMix[iC][iF]);
	    NormalizeHistogram(HDataSys[iC][iF],HMix[iC][iF]);
         }   
      }
   }

   // Setup   

   // Setup legend
   TLegend Legend(LegendLeft, LegendBottom, LegendLeft + 0.40, LegendBottom + NPair * 0.08);
   Legend.SetTextFont(42);
   Legend.SetTextSize(0.035 * CanvasHeight / PadHeight);
   Legend.SetBorderSize(0);
   Legend.SetFillStyle(0);
   
   for(int iC = 0; iC < NColumn; iC++) {
      for(int iF = 0; iF < NFile; iF++) {
         if (Rebin!=1) {
            HData[iC][iF]->Rebin(Rebin);
            HData[iC][iF]->Scale(1./Rebin);
            HDataSys[iC][iF]->Rebin(Rebin);
            HDataSys[iC][iF]->Scale(1./Rebin);
         }
      }	   
   }   

   // Draw things
   for(int iC = 0; iC < NColumn; iC++) {
      Pad[iC]->cd();
      HWorld[iC]->Draw("axis");
      cout <<iC<<endl;
      for(int iF = 0; iF < NPair; iF++) {
         if (lines[iF]!=0) {
	    HDataSys[iC][iF]->SetLineStyle(lines[iF]);
	    HData[iC][iF]->SetLineStyle(lines[iF]);
	 }
	 if(SkipSystematics == false && HDataSys[iC][iF] != nullptr && lines[iF]==0) HDataSys[iC][iF]->Draw("same e2");
	 if (lines[iF]==0) HData[iC][iF]->Draw("same"); else HData[iC][iF]->Draw("hist c same");
         cout <<iF<<" "<<(lines[iF]==0)<<endl;
      }

      Latex.SetTextAngle(0);
      Latex.SetTextAlign(33);
      Latex.SetTextSize(0.035 * CanvasHeight / PadHeight);
      Latex.DrawLatex(0.97, 0.97, Labels[iC].c_str());

      if(iC == 0) {   // adding extra info!
         for(int i = 0; i < (int)ExtraInfo.size(); i++) {
            Latex.SetTextAngle(0);
            Latex.SetTextAlign(11);
            Latex.SetTextSize(0.035 * CanvasHeight / PadHeight);
            Latex.DrawLatex(0.08, 0.85 - i * 0.075, ExtraInfo[i].c_str());
         }
      }
      if(iC == NColumn - 1) {  // adding legend!
         for(int iF = 0; iF < NPair; iF++) {
	    if (lines[iF]==0) {
	       Legend.AddEntry(HData[iC][iF], CurveLabels[iF].c_str(), "pl");
            } else {
	       Legend.AddEntry(HData[iC][iF], CurveLabels[iF].c_str(), "l");
            }
	    cout <<iF<<" "<<CurveLabels[iF].c_str()<<" "<<DataFiles[iF].c_str()<<" "<<DataFiles[iF+NPair].c_str()<<endl;
	 }
	 Legend.Draw();
      }

      // Draw difference
      if (PlotDiff) {
         RPad[iC]->cd();
         HRWorld[iC]->Draw("axis");
      
         for(int iF = 0; iF < NPair; iF++) {
            if(SkipSystematics == false && HDataSys[iC][iF] != nullptr) {
               TH1D *hDiffSys = (TH1D*) HDataSys[iC][iF]->Clone(Form("hDiffSys_%d_%d",iC,iF));
   	       hDiffSys->Add(HDataSys[iC][iF+NPair], -1);
	       if (lines[iF]==0) hDiffSys->Draw("same e2"); else {
	          //hDiffSys->SetMarkerSize(0);
	          //hDiffSys->Draw("e2 hist c same");
	       }
	    }   
	    
            TH1D *hDiff = (TH1D*) HData[iC][iF]->Clone(Form("hDiff_%d_%d",iC,iF));
	    hDiff->Add(HData[iC][iF+NPair], -1);
	    if (lines[iF]==0) hDiff->Draw("same"); else { 
 	       hDiff->SetMarkerSize(0);
	       hDiff->Draw("hist c same");
            }
         }
      }	 
   }

   // Finally we have the plots
   Canvas.SaveAs((OutputBase + ".pdf").c_str());

   // Close input files
   for(int iF = 0; iF < NFile; iF++) {
      if(SkipSystematics == false && SysFile[iF] != nullptr) {
         SysFile[iF]->Close();
         delete SysFile[iF];
      }
   }
   for(int iF = 0; iF < NFile; iF++) {
      if(File[iF] != nullptr)
      {
         File[iF]->Close();
         delete File[iF];
      }
   }

   return 0;
}



