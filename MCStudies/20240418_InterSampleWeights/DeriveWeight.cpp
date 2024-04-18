#include <iostream>
#include <vector>
using namespace std;

#include "TCanvas.h"
#include "TH1D.h"
#include "TF1.h"

#include "SetStyle.h"
#include "CommandLine.h"
#include "PlotHelper4.h"
#include "Messenger.h"

int main(int argc, char *argv[]);
double GetZWeight(double PT);

int main(int argc, char *argv[])
{
   vector<int> Colors = GetCVDColors6();

   SetThesisStyle();

   CommandLine CL(argc, argv);

   string PPFileName = CL.Get("PP");
   string AAFileName = CL.Get("AA");
   string OutputFileName = CL.Get("Output", "Result.pdf");

   TFile PPFile(PPFileName.c_str());
   TFile AAFile(AAFileName.c_str());

   ZHadronMessenger MPP(PPFile, "Tree");
   ZHadronMessenger MAA(AAFile, "Tree");

   double Bins[101];
   double Min = 1;
   double Max = 150;
   for(int i = 0; i <= 100; i++)
      Bins[i] = Min * exp((log(Max) - log(Min)) / 100 * i);

   TH1D HPPPT("HPPPT", "PP;PT", 100, Bins);
   TH1D HPPPTW("HPPPTW", "PP;PT", 100, Bins);
   TH1D HPPPTWR("HPPPTWR", "PP;PT", 100, Bins);
   TH1D HAAPT("HAAPT", "AA;PT", 100, Bins);
   TH1D HAAPTR("HAAPTR", "AA;PT", 100, Bins);
   TH1D HPPY("HPPY", "PP;Y", 100, -2.5, 2.5);
   TH1D HPPYW("HPPYW", "PP;Y", 100, -2.5, 2.5);
   TH1D HPPYWR("HPPYWR", "PP;Y", 100, -2.5, 2.5);
   TH1D HAAY("HAAY", "AA;Y", 100, -2.5, 2.5);
   TH1D HAAYR("HAAYR", "AA;Y", 100, -2.5, 2.5);

   int EntryCount;

   EntryCount = MPP.GetEntries();
   for(int iE = 0; iE < EntryCount; iE++)
   {
      MPP.GetEntry(iE);

      if(MPP.GoodGenZ == true)
      {
         double ZWeight = GetZWeight(MPP.genZPt->at(0));
         HPPPT.Fill(MPP.genZPt->at(0), MPP.EventWeight);
         HPPPTW.Fill(MPP.genZPt->at(0), MPP.EventWeight * ZWeight);
         HPPY.Fill(MPP.genZY->at(0), MPP.EventWeight);
         HPPYW.Fill(MPP.genZY->at(0), MPP.EventWeight * ZWeight);
      }
      if(MPP.GoodRecoZ == true)
      {
         double ZWeight = GetZWeight(MPP.zPt->at(0));
         HPPPTWR.Fill(MPP.zPt->at(0), MPP.EventWeight * ZWeight);
         HPPYWR.Fill(MPP.zY->at(0), MPP.EventWeight * ZWeight);
      }
   }
   
   EntryCount = MAA.GetEntries();
   for(int iE = 0; iE < EntryCount; iE++)
   {
      MAA.GetEntry(iE);
      if(MAA.GoodGenZ == true)
      {
         HAAPT.Fill(MAA.genZPt->at(0), MAA.EventWeight);
         HAAY.Fill(MAA.genZY->at(0), MAA.EventWeight);
      }
      if(MAA.GoodRecoZ == true)
      {
         HAAPTR.Fill(MAA.zPt->at(0), MAA.EventWeight);
         HAAYR.Fill(MAA.zY->at(0), MAA.EventWeight);
      }
   }

   HPPPT.SetMarkerColor(Colors[0]);
   HPPPT.SetLineColor(Colors[0]);
   HAAPT.SetMarkerColor(Colors[1]);
   HAAPT.SetLineColor(Colors[1]);
   HPPPTW.SetMarkerColor(Colors[2]);
   HPPPTW.SetLineColor(Colors[2]);
   HPPPTWR.SetMarkerColor(Colors[2]);
   HPPPTWR.SetLineColor(Colors[2]);
   HAAPTR.SetMarkerColor(Colors[1]);
   HAAPTR.SetLineColor(Colors[1]);
   HPPY.SetMarkerColor(Colors[0]);
   HPPY.SetLineColor(Colors[0]);
   HAAY.SetMarkerColor(Colors[1]);
   HAAY.SetLineColor(Colors[1]);
   HPPYW.SetMarkerColor(Colors[2]);
   HPPYW.SetLineColor(Colors[2]);
   HAAYR.SetMarkerColor(Colors[1]);
   HAAYR.SetLineColor(Colors[1]);
   HPPYWR.SetMarkerColor(Colors[2]);
   HPPYWR.SetLineColor(Colors[2]);

   PdfFileHelper PdfFile(OutputFileName);
   PdfFile.AddTextPage("Result");

   PdfFile.AddTextPage("Raw Distributions");

   PdfFile.AddPlot(HPPPT, "", true);
   PdfFile.AddPlot(HAAPT, "", true);
   PdfFile.AddPlot(HPPY, "", true);
   PdfFile.AddPlot(HAAY, "", true);

   TCanvas Canvas;
   Canvas.SetLogy();

   Canvas.SetLogx(true);
   HPPPT.Draw();
   HAAPT.Draw("same");
   PdfFile.AddCanvas(Canvas);

   Canvas.SetLogx(false);
   HPPY.Draw();
   HAAY.Draw("same");
   PdfFile.AddCanvas(Canvas);

   PdfFile.AddTextPage("Derivations");

   TH1D *HRatio = (TH1D *)HAAPT.Clone("HRatio");
   HRatio->Divide(&HPPPT);

   TF1 F("F", "pol3");
   HRatio->Fit(&F);

   HRatio->SetMinimum(0.5);
   HRatio->SetMaximum(1.5);

   HRatio->SetStats(0);
   PdfFile.AddPlot(HRatio, "", false, false, false, true);
   
   PdfFile.AddTextPage({
      Form("P0 = %f #pm %f", F.GetParameter(0), F.GetParError(0)),
      Form("P1 = %f #pm %f", F.GetParameter(1), F.GetParError(1)),
      Form("P2 = %f #pm %f", F.GetParameter(2), F.GetParError(2)),
      Form("P3 = %f #pm %f", F.GetParameter(3), F.GetParError(3))
   });

   PdfFile.AddTextPage("Corrected Distributions");

   Canvas.SetLogx(true);
   HPPPTW.Draw();
   HAAPT.Draw("same");
   PdfFile.AddCanvas(Canvas);

   Canvas.SetLogx(false);
   HPPYW.Draw();
   HAAY.Draw("same");
   PdfFile.AddCanvas(Canvas);
  
   PdfFile.AddTextPage("Apply same weight to reco");

   Canvas.SetLogx(true);
   HPPPTWR.DrawNormalized();
   HAAPTR.DrawNormalized("same");
   PdfFile.AddCanvas(Canvas);

   Canvas.SetLogx(false);
   HPPYWR.DrawNormalized();
   HAAYR.DrawNormalized("same");
   PdfFile.AddCanvas(Canvas);

   PdfFile.AddTimeStampPage();
   PdfFile.Close();

   AAFile.Close();
   PPFile.Close();

   return 0;
}

double GetZWeight(double PT)
{
   if(PT < 1)
      PT = 1;
   if(PT > 150)
      PT = 150;

   double P0 =     0.985164;
   double P1 =  -0.00116944;
   double P2 =  5.26231e-05;
   double P3 = -2.21173e-07;

   double Weight = P0 + P1 * PT + P2 * PT * PT + P3 * PT * PT * PT;

   return Weight;
}



