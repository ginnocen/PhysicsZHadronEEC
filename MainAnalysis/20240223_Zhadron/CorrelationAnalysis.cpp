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
    bool isGenZ;           // isGenZ flag
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
	cout << "nMix: "<< nMix << endl;
        cout << "isSelfMixing: " << isSelfMixing << endl;
	cout << "isGenZ: " << isGenZ << endl;
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
   if ((par.isGenZ ? b->genZMass->size() : b->zMass->size())==0) return 0;
   if ((par.isGenZ ? (*b->genZMass)[0] : (*b->zMass)[0])<60) return 0;
   if ((par.isGenZ ? (*b->genZMass)[0] : (*b->zMass)[0])>120) return 0;
   if (fabs((par.isGenZ ? (*b->genZEta)[0] : (*b->zEta)[0]))>2) return 0;
   if ((par.isGenZ ? (*b->genZPt)[0] : (*b->zPt)[0])<par.MinZPT) return 0;
   if ((par.isGenZ ? (*b->genZPt)[0] : (*b->zPt)[0])>par.MaxZPT) return 0;

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


bool matching(ZHadronMessenger *a, ZHadronMessenger *b) {
    if (fabs(a->hiHF-b->hiHF-97.13)<10) return 1;
    return 0;
}

//============================================================//
// Z hadron dphi calculation
//============================================================//
float getDphi(ZHadronMessenger *MZSignal, ZHadronMessenger *MMix, TH2D *h, TH2D *hSub0, const Parameters& par) {
    float nZ = 0;
    h->Sumw2();
    par.printParameters();
    unsigned long nEntry = MZSignal->GetEntries() * par.scaleFactor;
    unsigned long iStart = nEntry * (par.nChunk - 1) / par.nThread;
    unsigned long iEnd = nEntry * par.nChunk / par.nThread;
    unsigned int targetMix = ((par.nMix - 1) * par.mix + 1);

    ProgressBar Bar(cout, iEnd - iStart);
    Bar.SetStyle(1);
    unsigned long mix_i = iStart;
    unsigned long mixstart_i = mix_i;
          
    for (unsigned long i = iStart; i < iEnd; i++) {
       MZSignal->GetEntry(i);
       if (i % 100000 == 0) {
          Bar.Update(i - iStart);
          Bar.Print();
       }

       // Check if the event passes the selection criteria
       if (eventSelection(MZSignal, par)) {
          // Find a mixed event
	  float zEta = (par.isGenZ ? (*MZSignal->genZEta)[0] : (*MZSignal->zEta)[0]);
	  float zPhi = (par.isGenZ ? (*MZSignal->genZPhi)[0] : (*MZSignal->zPhi)[0]);
	  
          for (unsigned int nMix = 0; nMix < targetMix; nMix++) {
             bool foundMix = false;
	     mixstart_i = mix_i;
             if (par.mix) {
                while (!foundMix) {
                   mix_i = (mix_i + 1);
                   if (mix_i >= MMix->GetEntries()) mix_i = 0;
                   if (mixstart_i == mix_i) break;
                   MMix->GetEntry(mix_i);
                   if ((eventSelection(MMix, par)&&par.isSelfMixing)||(matching(MZSignal,MMix)&&!par.isSelfMixing)) foundMix = true;
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
                float weight = par.mix ? (MZSignal->NCollWeight) * (*MMix->trackWeight)[j] * (MMix->ZWeight) : (MZSignal->NCollWeight) * (*MZSignal->trackWeight)[j] * (MZSignal->ZWeight);
                h->Fill( trackDeta, trackDphi , weight);
                h->Fill(-trackDeta, trackDphi , weight);
                h->Fill( trackDeta, trackDphi2, weight);
                h->Fill(-trackDeta, trackDphi2, weight);
		
		if (!par.mix && (*MZSignal->subevent)[j]==0) {
                   hSub0->Fill( trackDeta, trackDphi , weight);
                   hSub0->Fill(-trackDeta, trackDphi , weight);
                   hSub0->Fill( trackDeta, trackDphi2, weight);
                   hSub0->Fill(-trackDeta, trackDphi2, weight);
		}
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
    inf->Close();
  }

  void analyze(Parameters& par) {
    // First histogram with mix=false
    par.mix = false;
    h = new TH2D(Form("h%s", title.c_str()), "", 20, -4, 4, 20, -M_PI/2, 3*M_PI/2);
    hSub0 = new TH2D(Form("hSub0%s", title.c_str()), "", 20, -4, 4, 20, -M_PI/2, 3*M_PI/2);
    hNZ = new TH1D(Form("hNZ%s", title.c_str()),"",1,0,1);
    hNZ->SetBinContent(1,getDphi(MZHadron, MMix, h, hSub0, par));

    // Second histogram with mix=true
    par.mix = true;
    hMix = new TH2D(Form("hMix%s", title.c_str()), "", 20, -4, 4, 20, -M_PI/2, 3*M_PI/2);
    hNZMix = new TH1D(Form("hNZMix%s", title.c_str()),"",1,0,1);
    hNZMix->SetBinContent(1,getDphi(MZHadron,MMix, hMix, 0, par));
  }
  
  void writeHistograms(TFile* outf) {
    outf->cd();
    smartWrite(h);
    smartWrite(hSub0);
    smartWrite(hMix);
    smartWrite(hNZ);
    smartWrite(hNZMix);
  }

  TFile *inf;
  TFile *mixFile;
  ZHadronMessenger *MZHadron, *MMix;
  TH2D *h=0, *hSub0=0, *hMix=0;
  TH1D *hNZ, *hNZMix;
  string title;
  
  private:
  void deleteHistograms() {
    delete h, hSub0, hMix, hNZ, hNZMix;
  }
};

//============================================================//
// Main analysis
//============================================================//
int main(int argc, char *argv[])
{
   // Read command line
   CommandLine CL(argc, argv);

   // Parameter sets
   float MinZPT     = CL.GetDouble("MinZPT", 40);         // Minimum Z particle transverse momentum threshold for event selection.
   float MinTrackPT = CL.GetDouble("MinTrackPT", 1);      // Minimum track transverse momentum threshold for track selection.
   float MaxZPT     = CL.GetDouble("MaxZPT", 120);        // Maximum Z particle transverse momentum threshold for event selection.
   float MaxTrackPT = CL.GetDouble("MaxTrackPT", 2);      // Maximum track transverse momentum threshold for track selection.
   int MaxHiBin      = CL.GetInt("MaxHiBin", 0);          // Maximum hiBin value for event selection. hiBin is a variable often used in heavy-ion collision experiments.
   int MinHiBin      = CL.GetInt("MinHiBin", 200);        // Minimum hiBin value for event selection.

   Parameters par(MinZPT, MaxZPT, MinTrackPT, MaxTrackPT, MaxHiBin, MinHiBin);
   par.input         = CL.Get("Input",   "sample/HISingleMuon.root");                // Input file
   par.mixFile       = CL.Get("MixFile", "sample/HISingleMuon.root");                // Input Mix file
   par.output        = CL.Get("Output",  "output.root");                             // Output file
   par.isSelfMixing  = CL.GetBool("IsSelfMixing", true);  // Determine if the analysis is self-mixing
   par.isGenZ        = CL.GetBool("IsGenZ", false);       // Determine if the analysis is using Gen level Z     
   bool IsData       = CL.GetBool("IsData", false);       // Determines whether the analysis is being run on actual data.
   bool IsPP         = CL.GetBool("IsPP", false);         // Flag to indicate if the analysis is for Proton-Proton collisions.
   par.scaleFactor   = CL.GetDouble("Fraction", 1.00);    // Fraction of event processed in the sample
   par.nThread       = CL.GetInt("nThread", 1);           // The number of threads to be used for parallel processing.
   par.nChunk        = CL.GetInt("nChunk", 1);            // Specifies which chunk (segment) of the data to process, used in parallel processing.
   par.nMix          = CL.GetInt("nMix", 10);             // Number of mixed events to be considered in the analysis.
   par.mix = 0;
   
   if (par.isSelfMixing && par.input!=par.mixFile) {
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
