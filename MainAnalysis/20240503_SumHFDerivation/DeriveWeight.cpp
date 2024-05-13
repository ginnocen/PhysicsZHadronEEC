#include <iostream>
using namespace std;

#include "TF1.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TCanvas.h"

#include "CommandLine.h"
#include "PlotHelper4.h"
#include "SetStyle.h"

int main(int argc, char *argv[])
{
   SetThesisStyle();

   CommandLine CL(argc, argv);

   string FileNameDY = CL.Get("DY", "Input/DYLLMC_Gen.root");
   string FileNameMB = CL.Get("MB", "Input/MinBiasMC_Gen.root");
   string OutputFileName = CL.Get("Output", "GenWeight_Centrality010.root");
   string PDFFileName = CL.Get("PDF", "SumHFWeight.pdf");

   string HFShift = CL.Get("HFShift", "(745.5-117.6*exp(-0.1148*genZPt[0]))");
   string Selection = CL.Get("Selection", "(genZPt[0] > 20 && hiBin >= 0 && hiBin < 20)");

   TFile FileDY(FileNameDY.c_str());
   TFile FileMB(FileNameMB.c_str());

   TTree *TDY = (TTree *)FileDY.Get("Tree");
   TTree *TMB = (TTree *)FileMB.Get("Tree");

   TFile OutputFile(OutputFileName.c_str(), "RECREATE");

   int NBins = 200;
   double Bins[201] = {0};
   double Min = 1;
   double Max = 180000;
   for(int i = 0; i <= 200; i++)
      Bins[i] = Min * exp((log(Max) - log(Min)) / 200 * i);

   TH1D HMB("HMB", ";;", NBins, Bins);
   TH1D HDY("HDY", ";;", NBins, Bins);

   HMB.SetStats(0);
   HDY.SetStats(0);

   TDY->SetAlias("HFShift", HFShift.c_str());
   TDY->Draw("SignalHF-HFShift>>HDY", Selection.c_str());
   TMB->Draw("SignalHF>>HMB", "");
   
   TH1D *HRatio = (TH1D *)HDY.Clone("HRatio");
   HRatio->Divide(&HMB);

   HMB.SetLineColor(kBlue);
   HDY.SetLineColor(kRed);
   HMB.SetLineWidth(2);
   HDY.SetLineWidth(2);

   PdfFileHelper PdfFile(PDFFileName);
   PdfFile.AddTextPage("SumHF Weights");

   PdfFile.AddTextPage({"HF Shift = \"" + HFShift + "\"", "Selection = \"" + Selection + "\""});

   TCanvas Canvas;
   Canvas.SetLogx();
   Canvas.SetLogy();

   HMB.Draw();
   HDY.Draw("same");

   PdfFile.AddCanvas(Canvas);

   PdfFile.AddPlot(HRatio, "", true, false, false, true);

   PdfFile.AddTimeStampPage();
   PdfFile.Close();

   OutputFile.cd();

   TF1 F("HFShift", "[0]-[1]*exp(-[2]*x)", 0, 1000);
   // F.SetParameters(745.5, 117.6, 0.1148);
   F.SetParameters(763.1539360425459-82.36975543861207, 101.2995115409047, 0.09666472578946463);

   HMB.Write();
   HDY.Write();
   HRatio->Write();
   F.Write();

   OutputFile.Close();

   FileDY.Close();
   FileMB.Close();

   return 0;
}







