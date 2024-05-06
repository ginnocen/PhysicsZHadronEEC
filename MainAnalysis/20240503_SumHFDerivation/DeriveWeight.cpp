#include <iostream>
using namespace std;

#include "TF1.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TCanvas.h"

#include "CommandLine.h"
#include "PlotHelper4.h"

int main(int argc, char *argv[])
{
   TFile FileDY("Input/DYLLMC_Gen.root");
   TFile FileMB("Input/MinBiasMC_Gen.root");

   TTree *TDY = (TTree *)FileDY.Get("Tree");
   TTree *TMB = (TTree *)FileMB.Get("Tree");

   TFile OutputFile("GenWeight_Centrality010.root", "RECREATE");

   int NBins = 200;
   double Bins[201] = {0};
   double Min = 1;
   double Max = 180000;
   for(int i = 0; i <= 200; i++)
      Bins[i] = Min * exp((log(Max) - log(Min)) / 200 * i);

   TH1D HMB("HMB", ";;", NBins, Bins);
   TH1D HDY("HDY", ";;", NBins, Bins);

   TDY->SetAlias("HFShift", "(745.5-117.6*exp(-0.1148*genZPt[0]))");
   TDY->Draw("SignalHF-HFShift>>HDY", "genZPt[0] > 20 && hiBin >= 0 && hiBin < 20");
   TMB->Draw("SignalHF>>HMB", "");

   PdfFileHelper PdfFile("SumHFWeight.pdf");
   PdfFile.AddTextPage("SumHF Weights");

   TCanvas Canvas;
   Canvas.SetLogx();
   Canvas.SetLogy();

   HMB.Draw();
   HDY.Draw("same");

   PdfFile.AddCanvas(Canvas);

   TH1D *HRatio = (TH1D *)HDY.Clone("HRatio");
   HRatio->Divide(&HMB);

   PdfFile.AddPlot(HRatio, "", true, false, false, true);

   PdfFile.AddTimeStampPage();
   PdfFile.Close();

   OutputFile.cd();

   TF1 F("HFShift", "[0]-[1]*exp(-[2]*x)", 0, 1000);
   F.SetParameters(745.5, 117.6, 0.1148);

   HMB.Write();
   HDY.Write();
   HRatio->Write();
   F.Write();

   OutputFile.Close();

   FileDY.Close();
   FileMB.Close();

   return 0;
}







