#include <iostream>
#include <vector>
using namespace std;

#include "TFile.h"
#include "TH1D.h"
#include "TLegend.h"

#include "CommandLine.h"
#include "PlotHelper4.h"
#include "SetStyle.h"

int main(int argc, char *argv[])
{
   vector<int> Colors = GetCVDColors6();

   SetThesisStyle();

   CommandLine CL(argc, argv);

   vector<string> InputFileNames = CL.GetStringVector("Input");
   int N = InputFileNames.size();

   vector<TFile *> InputFiles(N);
   for(int i = 0; i < N; i++)
      InputFiles[i] = new TFile(InputFileNames[i].c_str());

   vector<TH1D *> H1(N);
   vector<TH2D *> H2(N);
   for(int i = 0; i < N; i++)
   {
      H1[i] = (TH1D *)InputFiles[i]->Get("HPhi");
      H2[i] = (TH2D *)InputFiles[i]->Get("HEtaPhi");

      TH1D *HCount = (TH1D *)InputFiles[i]->Get("HCount");
      
      H1[i]->Scale(1 / HCount->GetBinContent(1));
      H2[i]->Scale(1 / HCount->GetBinContent(1));
   }

   PdfFileHelper PdfFile("Plots.pdf");
   PdfFile.AddTextPage("Juicy plots");

   for(int i = 0; i < N; i++)
   {
      PdfFile.AddPlot(H1[i]);
      PdfFile.AddTextPage(Form("Sum of all entries = %.2f", H1[i]->Integral()));
      cout << InputFileNames[i] << " " << H1[i]->Integral() << endl;
      PdfFile.AddPlot(H2[i], "colz");
   }

   TCanvas Canvas;

   TH2D HWorld("HWorld", ";#Delta#phi;", 100, -M_PI, M_PI, 100, 0, 12000);
   HWorld.SetStats(0);
   HWorld.Draw();

   TLegend Legend(0.2, 0.85, 0.5, 0.60);
   Legend.SetBorderSize(0);
   Legend.SetFillStyle(0);
   Legend.SetTextFont(42);
   Legend.SetTextSize(0.035);

   for(int i = 0; i < N; i++)
   {
      H1[i]->SetLineWidth(2);
      H1[i]->SetLineColor(Colors[i]);
      H1[i]->Draw("hist same");
      Legend.AddEntry(H1[i], Form("%s (%.0f)", InputFileNames[i].c_str(), H1[i]->Integral()), "l");
   }

   Legend.Draw();

   PdfFile.AddCanvas(Canvas);

   Canvas.SetLogy();
   
   TH2D HWorld2("HWorld2", ";#Delta#phi;", 100, -M_PI, M_PI, 100, 1, 500000);
   HWorld2.SetStats(0);
   HWorld2.Draw();
   for(int i = 0; i < N; i++)
      H1[i]->Draw("hist same");
   Legend.Draw();
   
   PdfFile.AddCanvas(Canvas);

   PdfFile.AddTimeStampPage();
   PdfFile.Close();

   for(int i = 0; i < N; i++)
   {
      InputFiles[i]->Close();
      delete InputFiles[i];
   }

   return 0;
}




