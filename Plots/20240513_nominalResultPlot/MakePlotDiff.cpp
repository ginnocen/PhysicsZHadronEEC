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
#include "TMath.h"

#include "CommandLine.h"
#include "SetStyle.h"
#include "utilities.h"
#include "usageMessage.h"


double Chi2ToSigma(double chi2, int ndof) {
    // Calculate the p-value from the chi-square value
    double pValue = TMath::Prob(chi2, ndof);
    
    // Convert the p-value to the number of sigma
    double sigma = TMath::Sqrt(2) * TMath::ErfcInverse(pValue);
    
    return sigma;
}


void FilterTH1D(TH1D* hist, double xMin, double xMax) {
    if (!hist) {
        std::cerr << "Invalid histogram pointer!" << std::endl;
        return;
    }

    int nBins = hist->GetNbinsX();
    for (int i = 1; i <= nBins; ++i) {
        double binCenter = hist->GetBinCenter(i);
        if (binCenter < xMin || binCenter > xMax) {
            hist->SetBinContent(i, 0);
            hist->SetBinError(i, 0); // Optionally set the bin error to zero as well
        }
    }
}


// Function to replace all occurrences of a character in a string
std::string replaceHashWithSpace(const std::string &input) {
    std::string output = input;
    std::replace(output.begin(), output.end(), '#', ' ');
    return output;
}

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
   HResult->SetFillColorAlpha(Color, 0.10);
   HResult->SetLineColorAlpha(Color, 0.10);
   HResult->SetMarkerColorAlpha(Color, 0.10);

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

   CommandLine CL(argc, argv);

   string OutputBase = CL.Get("OutputBase", "Plot");

   vector<string> DataFiles       = CL.GetStringVector("DataFiles", vector<string>{"pp.root","test.root"});

   bool SkipSystematics           = CL.GetBool("SkipSystematics", false);
   bool SkipMixFile               = CL.GetBool("SkipMixFile", true);
   bool PlotDiff                  = CL.GetBool("PlotDiff", true);
   int Rebin                      = CL.GetInt("Rebin", 1);
   bool PlotZeroLine		  = CL.GetBool("PlotZeroLine", true);
   
   
   vector<string> SystematicFiles = (SkipSystematics == false) ? CL.GetStringVector("SystematicFiles") : vector<string>();
   vector<string> MixFiles = (SkipMixFile == false) ? CL.GetStringVector("mixFiles") : vector<string>();
   vector<string> CurveLabels     = CL.GetStringVector("CurveLabels", vector<string>{"pp", "PbPb 0-30%"});
   string ToPlot                  = CL.Get("ToPlot", "DeltaPhi");
   vector<int>    lines           = CL.GetIntVector("lines", vector<int>{0,0,1,1,1,1,1});
   vector<int>    reflected       = CL.GetIntVector("reflected", vector<int>{1,0,0,0,0,0,0});
   vector<string> Tags            = CL.GetStringVector("Tags", vector<string> {
         "Result1_2", "Result2_4", "Result4_10"
   });
   vector<string> SecondTags      = CL.GetStringVector("SecondTags", vector<string>());
   vector<string> Labels          = CL.GetStringVector("Labels", vector<string> {
      "1 < p_{T}^{trk} < 2 GeV",
      "2 < p_{T}^{trk} < 4 GeV",
      "4 < p_{T}^{trk} < 10 GeV",
   });
   
   
   vector<int> Colors             = CL.GetIntVector("Colors", GetCVDColors6());   //vector<int>{1179,1180,1181,1182,15,1183,97,0,1184,1185,1186,1187,1188}

   
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

   double XMin      = CL.GetDouble("XMin", 0);
   double XMax      = CL.GetDouble("XMax", M_PI);
   double SolidXMin = CL.GetDouble("SolidXMin", XMin);
   double SolidXMax = CL.GetDouble("SolidXMax", XMax);
   double YMin 	    = CL.GetDouble("YMin", -5);
   double YMax      = CL.GetDouble("YMax", 5);
   double RMin      = CL.GetDouble("RMin", -5);
   double RMax      = CL.GetDouble("RMax", 5);

   int XAxisSpacing = 510;
   int YAxisSpacing = 510;
   int RAxisSpacing = 505;

   string XAxisLabel = CL.Get("XAxisLabel", "|#Delta#phi_{trk,Z}|");
   string YAxisLabel = CL.Get("YAxisLabel", "d<#DeltaN_{ch}>/d#Delta#phi_{trk,Z}");
   string RAxisLabel = CL.Get("RAxisLabel", "PbPb - pp");

   double MarginLeft    = 100;
   double MarginRight   = 50;
   double MarginTop     = CL.GetDouble("MarginTop",50);
   double MarginBottom  = CL.GetDouble("MarginBottom",100);
   double PadWidth      = 400;
   double PadHeight     = 400;
   double RPadHeight    = 170;
   
   if (MarginBottom==0) XAxisLabel="";
   
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

   double LegendLeft    = CL.GetDouble("LegendLeft", 0.03);
   double LegendBottom  = CL.GetDouble("LegendBottom", 0.52);

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
   
   TLine *l = new TLine (XMin,0, XMax,0);
   l->SetLineStyle(1);
   l->SetLineColor(kGray);

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
   Latex.SetTextSize(0.045);
   Latex.SetNDC();
      
   for(int iC = 0; iC < NColumn; iC++) {
      Latex.SetTextAngle(0);
      Latex.SetTextAlign(22);
      Latex.DrawLatex(XMarginLeft + XPadWidth * (iC + 0.5), XMarginBottom * 0.4, XAxisLabel.c_str());
   }
   Latex.SetTextAngle(90);
   Latex.SetTextSize(0.045);
   Latex.DrawLatex(XMarginLeft * 0.35, XMarginBottom + XRPadHeight + XPadHeight * 0.5, YAxisLabel.c_str());
   if (PlotDiff) Latex.DrawLatex(XMarginLeft * 0.35, XMarginBottom + XRPadHeight * 0.5, RAxisLabel.c_str());

   // Add decorations
   Latex.SetTextAngle(0);
   Latex.SetTextAlign(11);
   Latex.DrawLatex(XMarginLeft, XMarginBottom + XRPadHeight + XPadHeight + 0.01,
      "#font[62]{CMS} #font[52]{Preliminary}");
   Latex.SetTextAlign(31);
   Latex.SetTextSize(0.045);
   Latex.DrawLatex(XMarginLeft + XPadWidth * NColumn, XMarginBottom + XRPadHeight + XPadHeight + 0.01,
      Form("PbPb (pp) 5.02 TeV %s (%s)", PbPbLumi.c_str(), PPLumi.c_str()));

   // Retrieve histograms
   vector<vector<TH1D *>> HData(NColumn);
   vector<vector<TH1D *>> HClone(NColumn);
   vector<vector<TH1D *>> HDiffClone(NColumn);
   for(int iC = 0; iC < NColumn; iC++) {
      HData[iC].resize(NFile);
      HClone[iC].resize(NFile);

      for(int iF = 0; iF < NFile; iF++) {
         string Tag = Tags[iC];
         if(SecondTags.size() == NColumn && iF == 1) Tag = SecondTags[iC];
         HData[iC][iF] = GetHistogram(File[iF], ToPlot, Tag, Colors[iF]);
	 HData[iC][iF]->SetMarkerStyle(24);
	 HData[iC][iF]->SetMarkerSize(1.5);
         if (lines[iF]==0) HData[iC][iF]->SetLineColorAlpha(Colors[iF], 0.4); else HData[iC][iF]->SetLineColorAlpha(Colors[iF], 0.8);
         if (lines[iF]==0) HData[iC][iF]->SetMarkerColorAlpha(Colors[iF], 0.4);
         if (lines[iF]>0) HData[iC][iF]->SetLineWidth(2.5);
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
	    HDataSys[iC][iF]->SetMarkerStyle(24);
	    cout <<"Colors["<<iF<<"]: "<<Colors[iF]<<endl;
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
   TLegend Legend(LegendLeft, LegendBottom, LegendLeft + 0.4, LegendBottom + NPair * 0.063);
   Legend.SetTextFont(42);
//   Legend.SetTextSize(0.03 * CanvasHeight / PadHeight);
   Legend.SetTextSize(0.06);
   Legend.SetBorderSize(0);
   Legend.SetFillStyle(0);

   TLegend LegendReflected(LegendLeft, LegendBottom, LegendLeft + 0.4, LegendBottom + NPair * 0.063);
   LegendReflected.SetTextFont(42);
//   LegendReflected.SetTextSize(0.03 * CanvasHeight / PadHeight);
   LegendReflected.SetTextSize(0.06);
   LegendReflected.SetBorderSize(0);
   LegendReflected.SetFillStyle(0);

   
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
      if (PlotZeroLine) l->Draw();
      cout <<iC<<endl;
      for(int iF = NPair-1; iF >=0; iF--) {
         if (lines[iF]>0) {
	    HDataSys[iC][iF]->SetLineStyle(lines[iF]);
	    HData[iC][iF]->SetLineStyle(lines[iF]);
	 }
	 if(SkipSystematics == false && HDataSys[iC][iF] != nullptr && lines[iF]==0) HDataSys[iC][iF]->Draw("same e2");
	 if (lines[iF]<=0) { 
	    HClone[iC][iF] = (TH1D*) HData[iC][iF]->Clone(Form("%s_clone",HData[iC][iF]->GetName()));
	    HClone[iC][iF+NPair] = (TH1D*) HData[iC][iF+NPair]->Clone(Form("%s_clone",HData[iC][iF+NPair]->GetName()));
            HClone[iC][iF]->Draw("same");
	    HData[iC][iF]->SetAxisRange(SolidXMin,SolidXMax,"x");
	    HData[iC][iF]->SetMarkerStyle(20);
	    HData[iC][iF]->SetMarkerColorAlpha(Colors[iF],1);
	    HData[iC][iF]->Draw("same"); 
	 } else if (lines[iF]>0) { 
	 HData[iC][iF]->Draw("hist l same");
         }
      }

      Latex.SetTextAngle(0);
      Latex.SetTextAlign(33);
      Latex.SetTextSize(0.065);
      Latex.DrawLatex(0.97, 0.97, Labels[iC].c_str());

      if(iC == 0) {   // adding extra info!
         for(int i = 0; i < (int)ExtraInfo.size(); i++) {
            Latex.SetTextAngle(0);
            Latex.SetTextAlign(11);
            Latex.SetTextSize(0.065);
            Latex.DrawLatex(0.08, 0.85 - i * 0.075, ExtraInfo[i].c_str());
         }
      }
      if(iC == NColumn - 2) {  // adding legend!
         for(int iF = 0; iF < NPair; iF++) {
	    if (lines[iF]<=0) {
	       LegendReflected.AddEntry(HClone[iC][iF], Form ("%s Reflected",replaceHashWithSpace(CurveLabels[iF]).c_str()), "pl");
            }
	 }
	 LegendReflected.Draw();
      }
      if(iC == NColumn - 1) {  // adding legend!
         for(int iF = 0; iF < NPair; iF++) {
	    if (lines[iF]<=0) {
	       Legend.AddEntry(HData[iC][iF], replaceHashWithSpace(CurveLabels[iF]).c_str(), "pl");
            } else if (lines[iF]>0) {
	       Legend.AddEntry(HData[iC][iF], replaceHashWithSpace(CurveLabels[iF]).c_str(), "l");
            }
	    cout <<iF<<" "<<CurveLabels[iF].c_str()<<" "<<DataFiles[iF].c_str()<<" "<<DataFiles[iF+NPair].c_str()<<endl;
	 }
	 Legend.Draw();
      }

      // Draw difference
      if (PlotDiff) {
         RPad[iC]->cd();
         if (PlotZeroLine) l->Draw();

         HRWorld[iC]->Draw("axis");
         TLine *l = new TLine (XMin,0, XMax,0);
         l->SetLineStyle(1);
         l->SetLineColor(kGray);
         l->Draw();
         for(int iF = NPair-1; iF >=0; iF--) {
            TH1D *h1 = (TH1D*) HData[iC][iF]->Clone("h1"); 
            TH1D *h2 = (TH1D*) HData[iC][0]->Clone("h2"); 
	    //FilterTH1D(h1, SolidXMin, SolidXMax);
	    //FilterTH1D(h2, SolidXMin, SolidXMax);
//	    int ndof = (h1->GetNbinsX())/2. - 1; // Adjust this based on your degrees of freedom calculation
//	    double chi2 = h1->Chi2Test(h2, "WW");
 //           std::cout <<iC<<" "<<CurveLabels[iF]<<" Chi-Square Test Probability: " << chi2 <<" Sigma: " << Chi2ToSigma(chi2 * ndof, ndof)<< std::endl;
            if(SkipSystematics == false && HDataSys[iC][iF] != nullptr) {
               TH1D *hDiffSys = (TH1D*) HDataSys[iC][iF]->Clone(Form("hDiffSys_%d_%d",iC,iF));
   	       hDiffSys->Add(HDataSys[iC][iF+NPair], -1);
	       hDiffSys->SetMarkerSize(0);
	       if (lines[iF]<=0) hDiffSys->Draw("same e2"); else {
	          //hDiffSys->SetMarkerSize(0);
	          //hDiffSys->Draw("e2 hist c same");
	       }
	    }   
	    
            TH1D *hDiff = (TH1D*) HData[iC][iF]->Clone(Form("hDiff_%d_%d",iC,iF));
	    hDiff->Add(HData[iC][iF+NPair], -1);
	    if (lines[iF]==0) {
               TH1D *hDiffClone = (TH1D*) HClone[iC][iF]->Clone(Form("hDiffClone_%d_%d",iC,iF));
   	       hDiffClone->Add(HClone[iC][iF+NPair], -1);
	       hDiffClone->Draw("same");
	       hDiff->Draw("same"); 
         	    } else { 
 	       hDiff->SetMarkerSize(0);
	       hDiff->Draw("hist l same");
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



