#include <iostream>
#include <vector>
using namespace std;

#include "TFile.h"
#include "TH1D.h"

#include "CommandLine.h"
#include "PlotHelper4.h"
#include "SetStyle.h"

int main(int argc, char *argv[])
{
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
      cout << H1[i]->Integral() << endl;
      PdfFile.AddPlot(H2[i], "colz");
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




