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
    Parameters( float MinZPT, float MaxZPT, float MinTrackPT, float MaxTrackPT, int MinHiBin = 0, int MaxHiBin = 200, bool mix = false, float scaleFactor = 1.0, float nMix = 1)
    : MinZPT(MinZPT), MaxZPT(MaxZPT), MinTrackPT(MinTrackPT), MaxTrackPT(MaxTrackPT), MinHiBin(MinHiBin), MaxHiBin(MaxHiBin), mix(mix), scaleFactor(scaleFactor), nMix(nMix) {}

    string input;          // Input file name
    string output;         // Output file name
    string mixFile;        // Mix File name
    float MinZPT;          // Lower limit of Z pt
    float MaxZPT;          // Upper limit of Z pt
    float MinTrackPT;      // Lower limit of track pt
    float MaxTrackPT;      // Upper limit of track pt
    bool isSelfMixing;     // isSelfMixing flag
    float scaleFactor;     // Scale factor
    int MinHiBin;          // Lower limit of hiBin
    int MaxHiBin;          // Upper limit of hiBin
    int nThread;           // Number of Threads
    int nChunk;            // Process the Nth chunk
    bool mix;              // Mix flag
    int nMix;              // Number of mixed events

    void printParameters() const {
        cout << "Input file: " << input << endl;
        cout << "Output: " << output << endl;
        cout << "MixFile: " << mixFile << endl;
        cout << "MinZPT: " << MinZPT << endl;
        cout << "MaxZPT: " << MaxZPT << endl;
        cout << "MinTrackPT: " << MinTrackPT << endl;
        cout << "MaxTrackPT: " << MaxTrackPT << endl;
        cout << "MinHiBin: " << MinHiBin << endl;
        cout << "MaxHiBin: " << MaxHiBin << endl;
        cout << "mix: " << mix << endl;
        cout << "isSelfMixing: " << isSelfMixing << endl;
        cout << "nChunk/nThread: " << nChunk << "/" << nThread << endl;

        if (mix) cout << "Event mixing!" << endl;
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

float getDphi(ZHadronMessenger *MZSignal, ZHadronMessenger *MMix, TH2D *h, const Parameters& par) {

    float nZ = 0;
    h->Sumw2();
    par.printParameters();
    unsigned long nEntry = MZSignal->GetEntries() * par.scaleFactor;
    unsigned long iStart = nEntry * (par.nChunk - 1) / par.nThread;
    unsigned long iEnd = nEntry * par.nChunk / par.nThread;
    unsigned int targetMix = ((par.nMix - 1) * par.mix + 1);

    ProgressBar Bar(cout, iEnd - iStart);
    Bar.SetStyle(1);

    for (unsigned long i = iStart; i < iEnd; i++) {
       bool foundZ = false;
       MZSignal->GetEntry(i);
       if (i % 100000 == 0) {
          Bar.Update(i - iStart);
          Bar.Print();
       }

       // Check if the event passes the selection criteria
       if (eventSelection(MZSignal, par)) {
          float zPhi = (*MZSignal->zPhi)[0];
          float zEta = (*MZSignal->zEta)[0];    

          float mix_i = i;

          // Find a mixed event
          for (unsigned int nMix = 0; nMix < targetMix; nMix++) {
             bool foundMix = false;
             if (par.mix) {
                while (!foundMix) {
                   mix_i = (mix_i + 1);
                   if (mix_i >= MMix->GetEntries()) mix_i = 0;
                   if (i == mix_i) break;
                   MMix->GetEntry(mix_i);
                   if (eventSelection(MMix, par)) foundMix = true;
                }
             }

             if (!foundMix && par.mix) {
                cout << "Cannot find a mixed event!!! Event = " << i << endl;
                break;
             }

             nZ += (MZSignal->NCollWeight);  // Ncoll reweighting at the event level.
             for (unsigned long j = 0; j < (par.mix ? MMix->trackPhi->size() : MZSignal->trackPhi->size()); j++) {
                if (!trackSelection((par.mix ? MMix : MZSignal), par, j)) continue;
                float trackDphi  = par.mix ? DeltaPhi((*MMix->trackPhi)[j], zPhi) : DeltaPhi((*MZSignal->trackPhi)[j], zPhi);
                float trackDphi2 = par.mix ? DeltaPhi(zPhi, (*MMix->trackPhi)[j]) : DeltaPhi(zPhi, (*MZSignal->trackPhi)[j]);
                float trackDeta  = par.mix ? fabs((*MMix->trackEta)[j] - zEta) : fabs((*MZSignal->trackEta)[j] - zEta);
                float weight = par.mix ? (MMix->NCollWeight) * (*MMix->trackWeight)[j] * (MMix->ZWeight) : (MZSignal->NCollWeight) * (*MZSignal->trackWeight)[j] * (MZSignal->ZWeight);

                h->Fill(trackDeta, trackDphi, weight);
                h->Fill(-trackDeta, trackDphi, weight);
                h->Fill(trackDeta, trackDphi2, weight);
                h->Fill(-trackDeta, trackDphi2, weight);
             }
          }
       }
    }
    cout << "done" << nZ << endl;
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
int main(int argc, char *argv[])
{
   // Read command line
   CommandLine CL(argc, argv);

   // File names
   string Input      = CL.Get("Input",   "sample/HISingleMuon.root");                 // Input file
   string MixFile    = CL.Get("MixFile", "sample/HISingleMuon.root");               // Input Mix file
   string Output     = CL.Get("Output",  "output.root");                             // Output file

   bool IsSelfMixing = CL.GetBool("IsSelfMixing", true);  // Determine if the analysis is self-mixing      
   bool IsData       = CL.GetBool("IsData", false);       // Determines whether the analysis is being run on actual data.
   bool IsPP         = CL.GetBool("IsPP", false);         // Flag to indicate if the analysis is for Proton-Proton collisions.
   float Fraction   = CL.GetDouble("Fraction", 1.00);    // Fraction of event processed in the sample
   float MinZPT     = CL.GetDouble("MinZPT", 40);        // Minimum Z particle transverse momentum threshold for event selection.
   float MinTrackPT = CL.GetDouble("MinTrackPT", 1);     // Minimum track transverse momentum threshold for track selection.
   float MaxZPT     = CL.GetDouble("MaxZPT", 120);       // Maximum Z particle transverse momentum threshold for event selection.
   float MaxTrackPT = CL.GetDouble("MaxTrackPT", 2);     // Maximum track transverse momentum threshold for track selection.
   int MaxHiBin      = CL.GetInt("MaxHiBin", 0);          // Maximum hiBin value for event selection. hiBin is a variable often used in heavy-ion collision experiments.
   int MinHiBin      = CL.GetInt("MinHiBin", 200);        // Minimum hiBin value for event selection.
   int nThread       = CL.GetInt("nThread", 1);           // The number of threads to be used for parallel processing.
   int nChunk        = CL.GetInt("nChunk", 1);            // Specifies which chunk (segment) of the data to process, used in parallel processing.
   int nMix          = CL.GetInt("nMix", 10);             // Number of mixed events to be considered in the analysis.

   // Parameter sets
   Parameters par(MinZPT, MaxZPT, MinTrackPT, MaxTrackPT, MaxHiBin, MinHiBin);
   par.isSelfMixing = IsSelfMixing;
   par.mix = 0;
   par.scaleFactor = Fraction;
   par.input = Input;
   par.mixFile = MixFile;
   par.output = Output;
   par.nThread = nThread;
   par.nChunk = nChunk;
   par.nMix = nMix;
   
   if (IsSelfMixing && Input!=MixFile) {
      cout <<"Error! Self-mixing mode but assigned different input and mix files. Please check the macro."<<endl;
      return -1;
   }

   TCanvas *c = new TCanvas("c", "", 800, 800);

   // Analyze Data
   DataAnalyzer analyzer(par.input.c_str(), par.mixFile.c_str(), "Data");
   analyzer.analyze(par);

   TFile *outf = new TFile(par.output.c_str(), "recreate");
   analyzer.writeHistograms(outf);

   cout << "done!" << outf->GetName() << endl;
}
