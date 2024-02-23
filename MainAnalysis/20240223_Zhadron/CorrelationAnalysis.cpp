#include <TFile.h>
#include <TCanvas.h>
#include <TTree.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TCut.h>
#include <TLegend.h>
#include <TNtuple.h>

//#include "Tree.C"
#include <iostream>

using namespace std;

#include "utilities.h"
#include "Messenger.h"
#include "CommandLine.h"



//============================================================//
// Define analysis parameters
//============================================================//

class Parameters 
{
  public:
  Parameters(double zPtCutL, double zPtCutH, double ptCutL, double ptCutH,
             int hiBinL = 0, int hiBinH = 200, bool mix = false, bool isGen = false,
             double scaleFactor = 1.0)
    : zPtCutL(zPtCutL), zPtCutH(zPtCutH), ptCutL(ptCutL), ptCutH(ptCutH),
      hiBinL(hiBinL), hiBinH(hiBinH), mix(mix), isGen(isGen), scaleFactor(scaleFactor) {}

  double zPtCutL;      // Lower limit of zpt
  double zPtCutH;      // Upper limit of zpt
  double ptCutL;       // Lower limit of pt
  double ptCutH;       // Upper limit of pt
  int hiBinL;          // Lower limit of hiBin
  int hiBinH;          // Upper limit of hiBin
  bool mix;            // Mix flag
  bool isGen;          // isGen flag
  double scaleFactor;  // Scale factor

  void printParameters() const 
  {
    cout << "zPtCutL: " << zPtCutL << endl;
    cout << "zPtCfutH: " << zPtCutH << endl;
    cout << "ptCutL: " << ptCutL << endl;
    cout << "ptCutH: " << ptCutH << endl;
    cout << "hiBinL: " << hiBinL << endl;
    cout << "hiBinH: " << hiBinH << endl;
    cout << "mix: " << mix << endl;
    cout << "isGen: " << isGen << endl;
    if (mix) cout <<"Event mixing!"<<endl;
    cout << "scaleFactor: " << scaleFactor << endl;
  }
};


//======= eventSelection =====================================//
// Check if the event mass eventSelection criteria
// ZPtCutL < zPt < ZPtCutH
// hiBinL , hiBin < hiBinH
//============================================================//
bool eventSelection(ZHadronMessenger *b, const Parameters& par)
{
   bool foundZ = false;
   if (b->hiBin< par.hiBinL) return 0;
   if (b->hiBin>=par.hiBinH) return 0;
   if (b->zMass->size()==0) return 0;
   if ((*b->zMass)[0]<60) return 0;
   if ((*b->zMass)[0]>120) return 0;
   if (fabs((*b->zEta)[0])>2) return 0;
   if ((*b->zPt)[0]<par.zPtCutL) return 0;
   if ((*b->zPt)[0]>par.zPtCutH) return 0;

   foundZ=1;   
   return foundZ;
}

//======= trackSelection =====================================//
// Check if the track pass selection criteria
// ZPtCutL < zPt < ZPtCutH
// hiBinL , hiBin < hiBinH
//============================================================//
bool trackSelection(ZHadronMessenger *b, Parameters par, int j) 
{
    //if ((*b->trackMuTagged)[j]) return false;  
    if ((*b->trackPt)[j]>par.ptCutH) return false;  
    if ((*b->trackPt)[j]<par.ptCutL) return false;  
    return true;
}


//============================================================//
// Z hadron dphi calculation
//============================================================//
void getDphi(ZHadronMessenger *b, TH2D *h, const Parameters& par)
{
    double nZ=0;
    h->Sumw2();
    par.printParameters();
    
    for (unsigned long i=0;i<b->GetEntries()*par.scaleFactor;i++) {
       bool foundZ=false;
       b->GetEntry(i);
       if (i%1000==0) cout <<i<<" / "<<b->GetEntries()<<endl;
       
       // check if the event pass the selection criteria
       if (eventSelection(b, par)) {
          double zPhi = (*b->zPhi)[0];
          double zEta = (*b->zEta)[0];	  

	  double mix_i = i;
	  for (unsigned int nMix=0;nMix<(9*par.mix+1);nMix++) {
	    //cout <<"mix"<<endl;
   	     bool foundMix = 0;
	     nZ+=(b->NCollWeight);    // Ncoll reweighting in the event level.
	     if (par.mix) {
	        while (foundMix==0) {
		
	           mix_i = (mix_i+1);
		   if (mix_i>=b->GetEntries()) mix_i=0;
		   if (i==mix_i)  break;
		   //cout <<mix_i<<endl;
	           b->GetEntry(mix_i);
		   if (eventSelection(b, par)) foundMix=1;
	        }
	     }
	  
	     if (foundMix==0&&par.mix){
	        cout <<"Can not find a mixed event!!! Event = "<<i<<endl;
	        break;
	     }
	  
	     for (unsigned long j=0;j<b->trackPhi->size();j++) {
                if (!trackSelection(b, par, j)) continue;  
                double trackPhi = (*b->trackPhi)[j];
	        double trackEta = (*b->trackEta)[j];
		double trackDphi = DeltaPhi(trackPhi, zPhi);
	        double trackDeta = fabs(trackEta- zEta);
		double weight = (b->NCollWeight)*(*b->trackWeight)[j]*(b->ZWeight); //(*b->trackResidualWeight)[j]*
		
		h->Fill(trackDeta,trackDphi,weight);
 	        h->Fill(-trackDeta,trackDphi,weight);
 	        h->Fill(trackDeta,-trackDphi,weight);
 	        h->Fill(-trackDeta,-trackDphi,weight);
	     }
          }
       }
    }
    cout <<"done"<<nZ<<endl;
    h->Scale(1./((double) nZ));
}


class DataAnalyzer {
public:
  DataAnalyzer(const char* filename, const char* treename, const char *mytitle = "Data") :
     inf(new TFile(filename)), MZHadron(new ZHadronMessenger(*inf,string("Tree"))), title(mytitle) {}


  ~DataAnalyzer() {
    deleteHistograms();
    closeFile();
  }
  
  void analyze(Parameters& par) {
    par.mix=false;
    hCut2 = project2D(par);
    hCut2->SetName(Form("h%s_2",title.c_str()));
    par.mix=true;
    hCut2mix = project2D(par);
    hCut2mix->SetName(Form("hmix%s_2",title.c_str()));
    hCut2mix->Draw();
    hDiff2D = (TH2D*) hCut2->Clone(Form("hDiff2D%s",title.c_str()));
    hDiff2D->Add(hCut2mix,-1);
    hDiff = (TH1D*) hDiff2D->ProjectionY(Form("hDiff%s",title.c_str()));
    
  }
  
  void writeHistograms(TFile* outf) {
    outf->cd();
    smartWrite(hCut1);
    smartWrite(hCut2);
    smartWrite(hCut2mix);
    smartWrite(hDiff);
    smartWrite(hDiff2D);
  }

  TH2D* getCut1() { return hCut1; }
  TH2D* getCut2() { return hCut2; }


  TFile *inf;
  ZHadronMessenger *MZHadron;
  TH2D *hCut1=0, *hCut2=0;
  TH2D *hCut2mix=0;
  TH2D *hDiff2D=0;
  TH1D *hDiff=0;
  string title;

  TH2D* project2D(const Parameters& par) {
    TH2D *h = new TH2D("h", "", 20,-4,4, 20, -M_PI, M_PI);
    cout <<"getDPhi!"<<endl;
    getDphi(MZHadron, h, par);
    return h;
  }
  
  private:
  void deleteHistograms() {
    delete hCut1;
    delete hCut2;
    delete hCut2mix;
    delete hDiff2D;
    delete hDiff;
  }

  void closeFile() {
    inf->Close();
  }
};



//============================================================//
// Main analysis
//============================================================//

int compareDataMCLoop(double ZptCutL=20, double ZptCutH=300, double ptL=0.5, double ptH=2, double hiBinL=0, double hiBinH=60)
{
   TCanvas *c = new TCanvas("c", "", 800, 800);

   // Initialize a Parameters object with default values
   Parameters par(ZptCutL, ZptCutH, ptL, ptH, hiBinL, hiBinH);
   par.isGen = 0;
   par.mix = 0;
   par.scaleFactor = 1;

   // Initialize a Parameters object with default values for pp
   Parameters parPP(ZptCutL, ZptCutH, ptL, ptH, -10, 10);
   parPP.isGen = 0;
   parPP.mix = 0;
   parPP.scaleFactor = 1;

   // Analyze Data
   DataAnalyzer analyzerData("sample/Skim_RECO.root", "Tree", "Data");
   analyzerData.analyze(par);

   par.scaleFactor = 1;

   // Analyze MC
   DataAnalyzer analyzerMC("sample/Skim_RECO.root", "Tree", "MC");
   analyzerMC.analyze(parPP);

   // Analyze MC Gen level
   DataAnalyzer analyzerMCGen("sample/Skim_GEN.root", "Tree", "MCGen");
   par.isGen = 1;
   analyzerMCGen.analyze(parPP);

   // Analyze pp Data
   DataAnalyzer analyzerPP("sample/Skim_RECO.root", "Tree", "ppData");
   analyzerPP.analyze(parPP);

   TFile *outf = new TFile(Form("result/output_data_%.1f_%.1f_%.0f_%.0f_%.0f_%.0f.root", ptL, ptH, ZptCutL, ZptCutH, hiBinL, hiBinH), "recreate");
   analyzerData.writeHistograms(outf);
   analyzerMC.writeHistograms(outf);
   analyzerMCGen.writeHistograms(outf);
   analyzerPP.writeHistograms(outf);

   cout << "done!" << endl;
   

   // Plotting
   outf->cd();
   analyzerMC.hDiff->SetLineColor(2);
   analyzerMC.hDiff->SetMarkerColor(2);
   analyzerMCGen.hDiff->SetLineColor(4);
   analyzerMCGen.hDiff->SetMarkerColor(4);
   analyzerPP.hDiff->SetLineColor(6);
   analyzerPP.hDiff->SetMarkerColor(6);

   HistogramRangeChecker checker;
   checker.checkHistogramRange(analyzerMC.hDiff);
   cout <<checker.getMinValue()<<" "<<checker.getMaxValue()<<endl;
   checker.checkHistogramRange(analyzerMCGen.hDiff);
   cout <<checker.getMinValue()<<" "<<checker.getMaxValue()<<endl;
   checker.checkHistogramRange(analyzerData.hDiff);
   cout <<checker.getMinValue()<<" "<<checker.getMaxValue()<<endl;
   checker.checkHistogramRange(analyzerPP.hDiff);
   cout <<checker.getMinValue()<<" "<<checker.getMaxValue()<<endl;
   


   analyzerMC.hDiff->SetAxisRange(checker.getMinValue() - (checker.getMaxValue() - checker.getMinValue()) * 0.1,
                                  checker.getMaxValue() + (checker.getMaxValue() - checker.getMinValue()) * 0.1, "Y");
   analyzerMC.hDiff->SetAxisRange(0, 3.2, "X");

   TLegend *leg = new TLegend(0.5, 0.7, 0.9, 0.9);
   leg->SetBorderSize(0);
   leg->AddEntry(analyzerData.hDiff, Form("Data %.0f-%.0f%%", hiBinL / 2., hiBinH / 2.), "pl");
   leg->AddEntry(analyzerMC.hDiff, "MC", "pl");
   leg->AddEntry(analyzerMCGen.hDiff, "MC Gen", "pl");
   leg->AddEntry(analyzerPP.hDiff, "pp Data", "pl");
   leg->AddEntry(analyzerMC.hDiff, Form("%.1f<Track p_{T}<%.1f", ptL, ptH), "");
   leg->AddEntry(analyzerMC.hDiff, Form("%.0f<Z p_{T}<%.0f", ZptCutL, ZptCutH), "");

   analyzerMC.hDiff->SetXTitle("#Delta#phi(Z,track)");
   analyzerMC.hDiff->SetYTitle("#Delta N_{ch} / Event");
   analyzerMC.hDiff->Draw();
   analyzerMCGen.hDiff->Draw("hist same");
   analyzerData.hDiff->Draw("same");
   analyzerPP.hDiff->Draw("same");
   leg->Draw();

   c->SaveAs(Form("result/Z_%.1f_%.1f_%.0f_%.0f_%.0f_%.0f.gif", ptL, ptH, ZptCutL, ZptCutH, hiBinL, hiBinH));
   c->SaveAs(Form("result/Z_%.1f_%.1f_%.0f_%.0f_%.0f_%.0f.pdf", ptL, ptH, ZptCutL, ZptCutH, hiBinL, hiBinH));
   c->Write();

   cout << outf->GetName() << endl;
   return 1;
}


int main(int argc, char *argv[])
{
   CommandLine CL(argc, argv);

   string Default = "DYJetsToLL_MLL-50_TuneCP5_HydjetDrumMB_5p02TeV-amcatnloFXFX-pythia8_merged.root";
   vector<string> Input = CL.GetStringVector("Input", {Default});
   string Output        = CL.Get("Output", "output.root");

   //Zhadron(Input, Output);
   compareDataMCLoop();
}
