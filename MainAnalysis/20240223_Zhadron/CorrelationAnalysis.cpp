#include <TFile.h>
#include <TCanvas.h>
#include <TTree.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TCut.h>
#include <TLegend.h>
#include <TNtuple.h>

#include <iostream>

using namespace std;

#include "utilities.h"     // Yen-Jie's random utility functions
#include "Messenger.h"     // Yi's Messengers for reading data files
#include "CommandLine.h"   // Yi's Commandline bundle
#include "ProgressBar.h"   // Yi's fish progress bar


//============================================================//
// Define analysis parameters
//============================================================//

class Parameters {
  public:
  Parameters(double MinZPT, double MaxZPT, double MinTrackPT, double MaxTrackPT,
             int MinHiBin = 0, int MaxHiBin = 200, bool mix = false, bool isGen = false,
             double scaleFactor = 1.0, double nMix = 1)
    : MinZPT(MinZPT), MaxZPT(MaxZPT), MinTrackPT(MinTrackPT), MaxTrackPT(MaxTrackPT),
      MinHiBin(MinHiBin), MaxHiBin(MaxHiBin), mix(mix), isGen(isGen), scaleFactor(scaleFactor), nMix(nMix) {}

  double MinZPT;        // Lower limit of zpt
  double MaxZPT;        // Upper limit of zpt
  double MinTrackPT;    // Lower limit of pt
  double MaxTrackPT;    // Upper limit of pt
  int MinHiBin;         // Lower limit of hiBin
  int MaxHiBin;         // Upper limit of hiBin
  bool isGen;           // isGen flag
  double scaleFactor;   // Scale factor
  string input;         // Input file name
  string output;        // Output file name
  string mixFile;       // Mix File name
  int nThread;          // Number of Threads
  int nChunk;           // Process the Nth chunk
  bool mix;             // Mix flag
  int nMix;             // Number of mixed events

  void printParameters() const {
    cout << "MinZPT: " << MinZPT << endl;
    cout << "zPtCfutH: " << MaxZPT << endl;
    cout << "MinTrackPT: " << MinTrackPT << endl;
    cout << "MaxTrackPT: " << MaxTrackPT << endl;
    cout << "MinHiBin: " << MinHiBin << endl;
    cout << "MaxHiBin: " << MaxHiBin << endl;
    cout << "mix: " << mix << endl;
    cout << "isGen: " << isGen << endl;
    cout << "input: " << input << endl;
    cout << "output: " << output << endl;
    cout << "nChunk/nThread: " << nChunk << "/" << nThread << endl;
    if (mix) cout <<"Event mixing!"<<endl;
    cout << "scaleFactor: " << scaleFactor << endl;
  }
};


//======= eventSelection =====================================//
// Check if the event mass eventSelection criteria
// MinZPT < zPt < MaxZPT
// MinHiBin , hiBin < MaxHiBin
//============================================================//
bool eventSelection(ZHadronMessenger *b, const Parameters& par) {
   bool foundZ = false;
   if (b->hiBin< par.MinHiBin) return 0;
   if (b->hiBin>=par.MaxHiBin) return 0;
   if (b->zMass->size()==0) return 0;
   if ((*b->zMass)[0]<60) return 0;
   if ((*b->zMass)[0]>120) return 0;
   if (fabs((*b->zEta)[0])>2) return 0;
   if ((*b->zPt)[0]<par.MinZPT) return 0;
   if ((*b->zPt)[0]>par.MaxZPT) return 0;

   foundZ=1;   
   return foundZ;
}

//======= trackSelection =====================================//
// Check if the track pass selection criteria
// MinZPT < zPt < MaxZPT
// MinHiBin , hiBin < MaxHiBin
//============================================================//
bool trackSelection(ZHadronMessenger *b, Parameters par, int j) {
    //if ((*b->trackMuTagged)[j]) return false;  
    if ((*b->trackPt)[j]>par.MaxTrackPT) return false;  
    if ((*b->trackPt)[j]<par.MinTrackPT) return false;  
    return true;
}


//============================================================//
// Z hadron dphi calculation
//============================================================//
double getDphi(ZHadronMessenger *b, ZHadronMessenger *MMix, TH2D *h, const Parameters& par) {

    double nZ=0;
    h->Sumw2();
    par.printParameters();
    unsigned long nEntry = b->GetEntries()*par.scaleFactor;
    unsigned long iStart = nEntry*(par.nChunk-1)/par.nThread;
    unsigned long iEnd = nEntry*par.nChunk/par.nThread;
    ProgressBar Bar(cout, iEnd-iStart);
    Bar.SetStyle(1);

    for (unsigned long i=iStart;i<iEnd;i++) {
       bool foundZ=false;
       b->GetEntry(i);
       if (i%30000==0){
          Bar.Update(i-iStart);
          Bar.Print();
       }
       
       // check if the event pass the selection criteria
       if (eventSelection(b, par)) {
          double zPhi = (*b->zPhi)[0];
          double zEta = (*b->zEta)[0];	  

	  double mix_i = i;
	  
	  // find a mixed event
	  for (unsigned int nMix=0;nMix<((par.nMix-1)*par.mix+1);nMix++) {
	     bool foundMix = 0;
	     if (par.mix) {
	        while (foundMix==0) {
		
	           mix_i = (mix_i+1);
		   if (mix_i>=MMix->GetEntries()) mix_i=0;
		   if (i==mix_i)  break;
		   MMix->GetEntry(mix_i);
		   if (eventSelection(MMix, par)) foundMix=1;
	        }
	     }
	  
	     if (foundMix==0&&par.mix) {
	        cout <<"Can not find a mixed event!!! Event = "<<i<<endl;
	        break;
	     }
	     
	     nZ+=(b->NCollWeight);    // Ncoll reweighting in the event level.
	     for (unsigned long j=0;j<(par.mix ? MMix->trackPhi->size(): b->trackPhi->size());j++) {
                if (!trackSelection((par.mix ? MMix: b), par, j)) continue;  
                double trackDphi  = par.mix ? DeltaPhi((*MMix->trackPhi)[j], zPhi) : DeltaPhi((*b->trackPhi)[j], zPhi);
		double trackDphi2 = par.mix ? DeltaPhi(zPhi,(*MMix->trackPhi)[j]) : DeltaPhi(zPhi, (*b->trackPhi)[j]);
	        double trackDeta  = par.mix ? fabs((*MMix->trackEta)[j]- zEta) : fabs((*b->trackEta)[j]- zEta);
		double weight = par.mix ? (MMix->NCollWeight)*(*MMix->trackWeight)[j]*(MMix->ZWeight) : (b->NCollWeight)*(*b->trackWeight)[j]*(b->ZWeight); //(*b->trackResidualWeight)[j]*
		
		h->Fill(trackDeta,trackDphi,weight);
 	        h->Fill(-trackDeta,trackDphi,weight);
 	        h->Fill(trackDeta,trackDphi2,weight);
 	        h->Fill(-trackDeta,trackDphi2,weight);
	     }
          }
       }
    }
    cout <<"done"<<nZ<<endl;
    return nZ;
}


class DataAnalyzer {
public:
  DataAnalyzer(const char* filename, const char* mixFilename, const char *mytitle = "Data") :
     inf(new TFile(filename)), MZHadron(new ZHadronMessenger(*inf,string("Tree"))), mixFile(new TFile(mixFilename)), MMix(new ZHadronMessenger(*mixFile,string("Tree"))), title(mytitle) {}

  ~DataAnalyzer() {
    deleteHistograms();
    closeFile();
  }

  void analyze(Parameters& par) {
    // First histogram with mix=false
    par.mix = false;
    h = new TH2D(Form("h%s", title.c_str()), "", 20, -4, 4, 20, -M_PI/2, 3*M_PI/2);
    hNZ = new TH1D(Form("hNZ%s", title.c_str()),"",1,0,1);
    hNZ->SetBinContent(1,getDphi(MZHadron, MMix, h, par));

    // Second histogram with mix=true
    par.mix = true;
    hMix = new TH2D(Form("hMix%s", title.c_str()), "", 20, -4, 4, 20, -M_PI/2, 3*M_PI/2);
    hNZMix = new TH1D(Form("hNZMix%s", title.c_str()),"",1,0,1);
    hNZMix->SetBinContent(1,getDphi(MZHadron,MMix, hMix, par));
  }
  
  void writeHistograms(TFile* outf) {
    outf->cd();
    smartWrite(h);
    smartWrite(hMix);
    smartWrite(hNZ);
    smartWrite(hNZMix);
  }

  TFile *inf;
  TFile *mixFile;
  ZHadronMessenger *MZHadron;
  ZHadronMessenger *MMix;
  TH2D *h=0;
  TH2D *hMix=0;
  TH1D *hNZ;
  TH1D *hNZMix;
  string title;
  
  private:
  void deleteHistograms() {
    delete h;
    delete hMix;
    delete hNZ;
    delete hNZMix;
  }

  void closeFile() {
    inf->Close();
  }
};



//============================================================//
// Main analysis
//============================================================//

int compareDataMCLoop(Parameters &par)
{
   TCanvas *c = new TCanvas("c", "", 800, 800);

   // Analyze Data
   DataAnalyzer analyzerData(par.input.c_str(), par.mixFile.c_str(), "Data");
   analyzerData.analyze(par);


   TFile *outf = new TFile(par.output.c_str(), "recreate");
   analyzerData.writeHistograms(outf);

   cout << "done!" << endl;
   cout << outf->GetName() << endl;
   return 1;
}


int main(int argc, char *argv[])
{

   // Read command line
   CommandLine CL(argc, argv);

   // File names
   string Input      = CL.Get("Input",   "sample/HISingleMuon.root");                 // Input file
   string MixFile    = CL.Get("MixFile", "sample/HISingleMuon.root");               // Input Mix file
   string Output     = CL.Get("Output",  "output.root");                             // Output file

   bool IsData       = CL.GetBool("IsData", false);       // Determines whether the analysis is being run on actual data.
   bool IsPP         = CL.GetBool("IsPP", false);         // Flag to indicate if the analysis is for Proton-Proton collisions.
   double Fraction   = CL.GetDouble("Fraction", 1.00);    // Fraction of event processed in the sample
   double MinZPT     = CL.GetDouble("MinZPT", 40);        // Minimum Z particle transverse momentum threshold for event selection.
   double MinTrackPT = CL.GetDouble("MinTrackPT", 1);     // Minimum track transverse momentum threshold for track selection.
   double MaxZPT     = CL.GetDouble("MaxZPT", 120);       // Maximum Z particle transverse momentum threshold for event selection.
   double MaxTrackPT = CL.GetDouble("MaxTrackPT", 2);     // Maximum track transverse momentum threshold for track selection.
   int MaxHiBin      = CL.GetInt("MaxHiBin", 0);          // Maximum hiBin value for event selection. hiBin is a variable often used in heavy-ion collision experiments.
   int MinHiBin      = CL.GetInt("MinHiBin", 200);        // Minimum hiBin value for event selection.
   int nThread       = CL.GetInt("nThread", 1);           // The number of threads to be used for parallel processing.
   int nChunk        = CL.GetInt("nChunk", 1);            // Specifies which chunk (segment) of the data to process, used in parallel processing.
   int nMix          = CL.GetInt("nMix", 10);             // Number of mixed events to be considered in the analysis.

   // Parameter sets
   Parameters par(MinZPT, MaxZPT, MinTrackPT, MaxTrackPT, MaxHiBin, MinHiBin);
   par.isGen = 0;
   par.mix = 0;
   par.scaleFactor = Fraction;
   par.input = Input;
   par.mixFile = MixFile;
   par.output = Output;
   par.nThread = nThread;
   par.nChunk = nChunk;
   par.nMix = nMix;

   compareDataMCLoop(par);
   
}
