#include <TFile.h>
#include <TCanvas.h>
#include <TTree.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TCut.h>
#include <TLegend.h>
#include <TNtuple.h>

#include "Tree.C"
#include <iostream>

#include "utilities.h"
#include "ResidualCorrection.h"


//============================================================//
// Define analysis parameters
//============================================================//

class Parameters {
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

  void printParameters() const {
    cout << "zPtCutL: " << zPtCutL << endl;
    cout << "zPtCutH: " << zPtCutH << endl;
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

//============================================================//
// Turn off all branches and only turn on the relevant ones
//============================================================//
void smartBranch(TTree* fChain)
{
   // Turn off all branches
   fChain->SetBranchStatus("*",0);

   // Turn on necessary branches
   fChain->SetBranchStatus("zMass",1);
   fChain->SetBranchStatus("zEta",1);
   fChain->SetBranchStatus("zPhi",1);
   fChain->SetBranchStatus("zPt",1);
   fChain->SetBranchStatus("hiBin",1);
   fChain->SetBranchStatus("trackPt",1);
   fChain->SetBranchStatus("trackMuTagged",1);
   fChain->SetBranchStatus("trackDeta",1);
   fChain->SetBranchStatus("trackDphi",1);
   fChain->SetBranchStatus("NCollWeight",1);
   fChain->SetBranchStatus("trackWeight",1);
   fChain->SetBranchStatus("ZWeight",1);
   fChain->SetBranchStatus("trackResidualWeight",1);
}

//======= eventSelection =====================================//
// Check if the event mass eventSelection criteria
// ZPtCutL < zPt < ZPtCutH
// hiBinL , hiBin < hiBinH
//============================================================//
bool eventSelection(Tree &b, const Parameters& par)
{
   bool foundZ = false;
   if (b.hiBin< par.hiBinL) return 0;
   if (b.hiBin>=par.hiBinH) return 0;
   if (b.zMass->size()==0) return 0;
   if ((*b.zMass)[0]<60) return 0;
   if ((*b.zMass)[0]>120) return 0;
   if (fabs((*b.zEta)[0])>2) return 0;
   if ((*b.zPt)[0]<par.zPtCutL) return 0;
   if ((*b.zPt)[0]>par.zPtCutH) return 0;

   foundZ=1;   
   return foundZ;
}

//======= trackSelection =====================================//
// Check if the track pass selection criteria
// ZPtCutL < zPt < ZPtCutH
// hiBinL , hiBin < hiBinH
//============================================================//
bool trackSelection(Tree &b, Parameters par, int j) {
    if ((*b.trackMuTagged)[j]) return false;  
    if ((*b.trackPt)[j]>par.ptCutH) return false;  
    if ((*b.trackPt)[j]<par.ptCutL) return false;  
    //if (fabs((*b.trackDeta)[j])>4) return false;
    return true;
}




//======= Project 3D histogram ===============================//
// Prepare 3D histogram (track pT, track Eta and track phi)
//============================================================//

void projectTrack3DHistogram(TTree *t, Tree &b, TH3D *h3D, const Parameters& par)
{
    double nZ=0;
    h3D->Sumw2();
    cout <<"Produce 3D histogram of (track pt, track eta and track phi)";
    par.printParameters();
    
    for (unsigned long i=0;i<t->GetEntries();i++) {
       bool foundZ=false;
       t->GetEntry(i);
       if (i%100000==0) cout <<i<<" / "<<t->GetEntries()<<endl;
       
       // check if the event pass the selection criteria
       if (eventSelection(b, par)) {
          // Ncoll wieght
	  nZ+=(b.NCollWeight);
	  for (long j=0;j<b.trackDphi->size();j++) {
             if (!trackSelection(b, par, j)) continue;  
	     // recover track Phi angle
	     double trackPhi = (*b.trackDphi)[j]+(*b.zPhi)[0];  
	     if (trackPhi<0) trackPhi+=2*M_PI;
	     // recover track Eta value
	     double trackEta = (*b.trackDeta)[j]+(*b.zEta)[0];
	     double residualCorr = 1;
	     h3D->Fill((*b.trackPt)[j], trackEta, trackPhi,(b.NCollWeight)*(*b.trackWeight)[j]*residualCorr);
 	  }
       }
    }
    cout <<"done!"<<nZ<<endl;
    h3D->Scale(1./((double) nZ));
}

//======= Data analyzer ======================================//
// Perform analysis
//============================================================//

class DataAnalyzer {
public:
  DataAnalyzer(const char* filename, const char* treename, const char *mytitle = "Data") :
    inf(new TFile(filename)), t((TTree*) inf->Get(treename)), b(t), title(mytitle) {}


  ~DataAnalyzer() {
    deleteHistograms();
    closeFile();
  }

  void analyze3D(Parameters& par) {
    smartBranch(t);
    h3D = project3D(par);
    h3D->SetName(Form("h3D%s_%d_%d",title.c_str(),par.hiBinL,par.hiBinH));
  }

  void writeHistograms(TFile* outf) {
    outf->cd();
    smartWrite(h3D);
  }


  TFile *inf;
  TTree *t;
  Tree b;
  TH3D *h3D=0;
  string title;

  
  TH3D* project3D(const Parameters& par) {

    TH3D* h3D = new TH3D("h3D", "Histogram Title; p_{T} (GeV/c); #eta; #phi", 50,0,10,50,-2.4,2.4,50, 0,2*M_PI);

    projectTrack3DHistogram(t, b, h3D, par);
    return h3D;
  }
  
  private:
  void deleteHistograms() {
    delete h3D;
  }

  void closeFile() {
    inf->Close();
  }
};


//============================================================//
// Efficiency Study
//============================================================//

int efficiencyStudy(double ZptCutL=5, double ZptCutH=200, double ptL=0, double ptH=10)
{
    // Initialize a Parameters object with default values
   Parameters par(ZptCutL, ZptCutH, ptL, ptH);

   double hiBin[5] = {0, 20, 60, 100, 200};
  
   for (unsigned int i=0;i<4;i++) {
      par.hiBinL=hiBin[i];
      par.hiBinH=hiBin[i+1];
      TFile *outf = new TFile(Form("result/output_data_%.1f_%.1f_%.0f_%.0f_%d_%d.root",ptL,ptH,ZptCutL,ZptCutH,par.hiBinL,par.hiBinH),"recreate");
      DataAnalyzer* analyzerMC = new DataAnalyzer("PbPbMC_V9.root", "Tree",Form("MC", hiBin[i], hiBin[i+1]));
      analyzerMC->analyze3D(par);
      cout <<"good"<<endl;
      analyzerMC->writeHistograms(outf);
      delete analyzerMC;

      DataAnalyzer* analyzerMCGen = new DataAnalyzer("PbPbMCGen_V9.root", "Tree",Form("MCGen", hiBin[i], hiBin[i+1]));
      analyzerMCGen->analyze3D(par);
      analyzerMCGen->writeHistograms(outf);
      
      delete analyzerMCGen;
      outf->Close();
   }
   return 1;
}


int efficiencyStudyPP(double ZptCutL=5, double ZptCutH=200, double ptL=0, double ptH=10)
{
    // Initialize a Parameters object with default values
   Parameters par(ZptCutL, ZptCutH, ptL, ptH);
   par.hiBinL=-10;
   par.hiBinH=0;
  
   TFile *outf = new TFile(Form("result/output_dataPP_%.1f_%.1f_%.0f_%.0f.root",ptL,ptH,ZptCutL,ZptCutH),"recreate");
   DataAnalyzer* analyzerMC = new DataAnalyzer("PPMC_v12.root", "Tree","MC");
   analyzerMC->analyze3D(par);
   cout <<"good"<<endl;
   analyzerMC->writeHistograms(outf);
   delete analyzerMC;

   DataAnalyzer* analyzerMCGen = new DataAnalyzer("PPMCGen_v12.root", "Tree","MCGen");
   analyzerMCGen->analyze3D(par);
   analyzerMCGen->writeHistograms(outf);
   
   delete analyzerMCGen;
   outf->Close();
   
   return 1;
}
