#include <vector>
#include <iostream>
using namespace std;

#include "TLatex.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TH2D.h"
#include "TFile.h"
#include "TLegend.h"

#include "SetStyle.h"
#include "CommandLine.h"
#include "PlotHelper4.h"

int main(int argc, char *argv[])
{
   SetThesisStyle();
   vector<int> Color = GetCVDColors6();

   CommandLine CL(argc, argv);

   string InputFileName     = CL.Get("Input");
   vector<string> Item      = CL.GetStringVector("Item");
   vector<string> Histogram = CL.GetStringVector("Histogram");
   vector<string> Label     = CL.GetStringVector("Label");
   string OutputBase        = CL.Get("OutputBase", "Plot");
   vector<string> ExtraText = CL.GetStringVector("ExtraText", vector<string>{});

   TFile File(InputFileName.c_str());

   PdfFileHelper PdfFile(OutputBase + ".pdf");
   PdfFile.AddTextPage("Systematics plots");

   int N = Item.size();

   for(string H : Histogram)
   {
      bool LogX = false;
      if(H.find("Log") != string::npos)
         LogX = true;

      PdfFile.AddTextPage(H);

      TH1D *HNominal = (TH1D *)File.Get(Form("Nominal_%s", H.c_str()));
      HNominal->SetTitle("");
      HNominal->SetStats(0);
      HNominal->SetMarkerStyle(20);
      HNominal->SetMarkerColor(kBlack);
      HNominal->SetLineWidth(2);
      HNominal->SetLineColor(kBlack);
      PdfFile.AddPlot(HNominal, "", false, false, true, LogX);

      vector<TH1D *> HSys(N);
      for(int i = 0; i < N; i++)
      {
         TH1D *HB = (TH1D *)File.Get(Form("%s_Base_%s", Item[i].c_str(), H.c_str()));
         TH1D *HV = (TH1D *)File.Get(Form("%s_Variant_%s", Item[i].c_str(), H.c_str()));

         HSys[i] = (TH1D *)HB->Clone(Form("%s_Sys_%s", Item[i].c_str(), H.c_str()));

         for(int iB = 1; iB <= HSys[i]->GetNbinsX(); iB++)
         {
            HSys[i]->SetBinError(iB, 0);
            HSys[i]->SetBinContent(iB, fabs(HV->GetBinContent(iB) - HB->GetBinContent(iB)));
         }
         HSys[i]->SetStats(0);

         HSys[i]->SetTitle("");
         HSys[i]->SetTitle(Label[i].c_str());
         HSys[i]->SetMarkerStyle(20);
         HSys[i]->SetLineColor(Color[i%6]);
         HSys[i]->SetMarkerColor(Color[i%6]);
         HSys[i]->SetLineWidth(2);

         TCanvas Canvas;
         HB->SetLineColor(kBlue);
         HB->SetMarkerColor(kBlue);
         HB->SetMarkerStyle(20);
         HB->SetStats(0);
         HB->Draw();
         HV->SetLineColor(kRed);
         HV->SetMarkerColor(kRed);
         HV->SetMarkerStyle(20);
         HV->SetStats(0);
         HV->Draw("same");
         if(LogX == true)
            Canvas.SetLogx();
         PdfFile.AddCanvas(Canvas);

         PdfFile.AddPlot(HSys[i], "", false, false, true, LogX);
      }

      TCanvas Canvas;

      TLegend Legend(0.5, 0.85, 0.8, 0.85 - N * 0.05);
      Legend.SetFillStyle(0);
      Legend.SetTextFont(42);
      Legend.SetTextSize(0.035);
      Legend.SetBorderSize(0);

      HSys[0]->SetTitle(0);
      HSys[0]->Draw();
      Legend.AddEntry(HSys[0], Label[0].c_str(), "pl");
      for(int i = 1; i < N; i++)
      {
         Legend.AddEntry(HSys[i], Label[i].c_str(), "pl");
         HSys[i]->SetTitle("");
         HSys[i]->Draw("same");
      }

      if(LogX == true)
         Canvas.SetLogx();

      Legend.Draw();

      TLatex Latex;
      Latex.SetTextFont(42);
      Latex.SetTextAlign(11);
      Latex.SetTextAngle(0);
      Latex.SetTextSize(0.035);
      Latex.SetNDC();
      for(int i = 0; i < (int)ExtraText.size(); i++)
         Latex.DrawLatex(0.20, 0.80 - i * 0.05, ExtraText[i].c_str());

      Canvas.SaveAs((OutputBase + "__" + H + ".pdf").c_str());

      PdfFile.AddCanvas(Canvas);
   }

   PdfFile.AddTimeStampPage();
   PdfFile.Close();

   File.Close();

   return 0;
}




