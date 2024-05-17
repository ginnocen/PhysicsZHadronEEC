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
#include "helpMessage.h"   // Print out help message
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
    float scaleFactor;     // Scale factor
    float shift;           // shift in sumHF when doing mb matching
    int MinHiBin;          // Lower limit of hiBin
    int MaxHiBin;          // Upper limit of hiBin
    int nThread;           // Number of Threads
    int nChunk;            // Process the Nth chunk
    bool mix;              // Mix flag
    int nMix;              // Number of mixed events
    TH1D *hShift;
    bool isSelfMixing;     // isSelfMixing flag
    bool isGenZ;           // isGenZ flag
    bool isMuTagged;       // Flag to enable/disable muon tagging requirement
    bool isPUReject;       // Flag to reject PU sample for systemaitcs.
    bool isHiBinUp;        // Flag to do systematics with HiBinUp
    bool isHiBinDown;      // Flag to do systematics with HiBinDown
    bool isPP;             // Flag to check if this is a PP analysis

   void printParameters() const {
       cout << "Input file: " << input << endl;
       cout << "Output file: " << output << endl;
       cout << "Mix File: " << mixFile << endl;
       cout << "MinZPT: " << MinZPT << " GeV/c" << endl;
       cout << "MaxZPT: " << MaxZPT << " GeV/c" << endl;
       cout << "MinTrackPT: " << MinTrackPT << " GeV/c" << endl;
       cout << "MaxTrackPT: " << MaxTrackPT << " GeV/c" << endl;
       cout << "isSelfMixing: " << (isSelfMixing ? "true" : "false") << endl;
       cout << "isGenZ: " << (isGenZ ? "true" : "false") << endl;
       cout << "Scale factor: " << scaleFactor << endl;
       cout << "SumHF shift: " << shift << endl;
       cout << "MinHiBin: " << MinHiBin << endl;
       cout << "MaxHiBin: " << MaxHiBin << endl;
       cout << "Number of Threads: " << nThread << endl;
       cout << "Process the Nth chunk: " << nChunk << endl;
       cout << "Mix flag: " << (mix ? "true" : "false") << endl;
       cout << "Number of mixed events: " << nMix << endl;
       cout << "Muon Tagging Enabled: " << (isMuTagged ? "true" : "false") << endl;
       cout << "PU rejection: " << (isPUReject ? "true" : "false") << endl;
       if (mix) cout << "Event mixing!" << endl;
   }
};


//============================================================//
// Function to check for configuration errors
//============================================================//
bool checkError(const Parameters& par) {
    if (par.isSelfMixing && par.input != par.mixFile) {
        std::cout << "Error! Self-mixing mode but assigned different input and mix files. Please check the macro." << std::endl;
        return true;  // Return true indicates an error was found
    }

    if (par.isHiBinUp && par.isHiBinDown) {
        std::cout << "Error! Cannot do hiBinUp and hiBinDown simultaneously!" << std::endl;
        return true;  // Return true indicates an error was found
    }

    // No errors found
    return false;
}


//======= eventSelection =====================================//
// Check if the event mass eventSelection criteria
// MinZPT < zPt < MaxZPT
// MinHiBin , hiBin < MaxHiBin
//============================================================//
bool eventSelection(ZHadronMessenger *b, const Parameters& par) {
   int effectiveHiBin = par.isHiBinUp ? b->hiBinUp : (par.isHiBinDown ? b->hiBinDown : b->hiBin);

   bool foundZ = false;            
   if (par.isPUReject && b->NVertex!=1) return 0;
   if (effectiveHiBin< par.MinHiBin) return 0;
   if (effectiveHiBin>=par.MaxHiBin) return 0;
   if ((par.isGenZ ? b->genZMass->size() : b->zMass->size())==0) return 0;
   if ((par.isGenZ ? (*b->genZMass)[0] : (*b->zMass)[0])<60) return 0;
   if ((par.isGenZ ? (*b->genZMass)[0] : (*b->zMass)[0])>120) return 0;
   if (fabs((par.isGenZ ? (*b->genZY)[0] : (*b->zY)[0]))>2.4) return 0;
   if ((par.isGenZ ? (*b->genZPt)[0] : (*b->zPt)[0])<par.MinZPT) return 0;
   if ((par.isGenZ ? (*b->genZPt)[0] : (*b->zPt)[0])>par.MaxZPT) return 0;
   foundZ=1;   
   return foundZ;
}

//======= trackSelection =====================================//
// Check if the track pass selection criteria
// MinZPT < zPt < MaxZPT &&  MinHiBin , hiBin < MaxHiBin
//============================================================//
bool trackSelection(ZHadronMessenger *b, Parameters par, int j) {
    if (par.isMuTagged && (*b->trackMuTagged)[j]) return false; 
    if ((*b->trackPt)[j]>par.MaxTrackPT) return false;  
    if ((*b->trackPt)[j]<par.MinTrackPT) return false;
    return true;
}

// ======= Define mixed event matching criteria
bool matching(ZHadronMessenger *a, ZHadronMessenger *b, double shift) {

// 1036 is the maxima of SignalHF in pythia
//   double shift = 1018.0541;
//   double shift = //1268.69;
//    if (a->hiHF<97.13&&b->hiHF<97.13) return 1;
//    if (a->SignalHF<shift*1.05&&b->SignalHF<shift*1.05) return 1;
//    if ((b->SignalHF/(a->SignalHF-shift))<1.02&&b->SignalHF/(a->SignalHF-shift)>0.97) return 1;
    if (int((a->SignalHF-shift)/180)==int((b->SignalHF)/180)) return 1;
//    if (fabs(b->SignalHF-a->SignalHF+shift)<300) return 1;
    return 0;
}

//============================================================//
// Z hadron dphi calculation
//============================================================//
float getDphi(ZHadronMessenger *MZSignal, ZHadronMessenger *MMix, ZHadronMessenger *MMixEvt, TH2D *h, TH2D *hSub0, const Parameters& par, TNtuple *nt=0) {
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
    int deltaI = (iEnd-iStart)/100+1;              
    for (unsigned long i = iStart; i < iEnd; i++) {
       MZSignal->GetEntry(i);
       if (i % deltaI == 0) {
          Bar.Update(i - iStart);
          Bar.Print();
       }

       // Check if the event passes the selection criteria
       if (eventSelection(MZSignal, par)) {
          // Find a mixed akeevent
	  float zY = (par.isGenZ ? (*MZSignal->genZY)[0] : (*MZSignal->zY)[0]);
	  float zPhi = (par.isGenZ ? (*MZSignal->genZPhi)[0] : (*MZSignal->zPhi)[0]);
	  
          for (unsigned int nMix = 0; nMix < targetMix; nMix++) {
             bool foundMix = false;
	     mixstart_i = mix_i;
             if (par.mix) {
                while (!foundMix) {
                   mix_i = (mix_i + 1);
                   if (mix_i >= MMixEvt->GetEntries()) mix_i = 0;
                   if (mixstart_i == mix_i) break;
                   MMixEvt->GetEntry(mix_i);
                   if (par.isSelfMixing) {
		      if (eventSelection(MMixEvt, par)&&par.isSelfMixing&&i!=mix_i) foundMix = true;
                   } else {
		      if (matching(MZSignal,MMixEvt,par.shift)&&!par.isSelfMixing) foundMix = true;

		   }
		}
             }
             if (!foundMix && par.mix) {
                cout << "Cannot find a mixed event!!! Event = " << i <<" "<< MZSignal->SignalHF<< endl;
                break;
             }
	     MMix->GetEntry(mix_i);
             nZ += 1;//(MZSignal->NCollWeight);  // Ncoll reweighting at the event level.
             for (unsigned long j = 0; j < (par.mix ? MMix->trackPhi->size() : MZSignal->trackPhi->size()); j++) {
                if (!trackSelection((par.mix ? MMix : MZSignal), par, j)) continue;
                float trackDphi  = par.mix ? DeltaPhi((*MMix->trackPhi)[j], zPhi) : DeltaPhi((*MZSignal->trackPhi)[j], zPhi);
                float trackDphi2 = par.mix ? DeltaPhi(zPhi, (*MMix->trackPhi)[j]) : DeltaPhi(zPhi, (*MZSignal->trackPhi)[j]);
                float trackDeta  = par.mix ? fabs((*MMix->trackEta)[j] - zY) : fabs((*MZSignal->trackEta)[j] - zY);
                //float weight = par.mix ? (MMix->NCollWeight) * (*MMix->trackWeight)[j] * (MMix->ZWeight) : (MZSignal->NCollWeight) * (*MZSignal->trackWeight)[j] * (MZSignal->ZWeight);
                float weight = (par.mix&&par.isSelfMixing) ? (MMix->ZWeight) : (MZSignal->ZWeight);
		weight*=(par.mix ? (*MMix->trackWeight)[j]*(*MMix->trackResidualWeight)[j] : (*MZSignal->trackWeight)[j]*(*MZSignal->trackResidualWeight)[j]);
                h->Fill( trackDeta, trackDphi , weight);
                h->Fill(-trackDeta, trackDphi , weight);
                h->Fill( trackDeta, trackDphi2, weight);
                h->Fill(-trackDeta, trackDphi2, weight);
		
		if (!par.mix && (*MZSignal->subevent)[j]==0) {
		   //cout <<"Event:"<<i<<" "<<trackDeta<<" "<<trackDphi2<<" "<<(*MZSignal->trackPt)[j]<<weight<<endl;
                   hSub0->Fill( trackDeta, trackDphi , weight);
                   hSub0->Fill(-trackDeta, trackDphi , weight);
                   hSub0->Fill( trackDeta, trackDphi2, weight);
                   hSub0->Fill(-trackDeta, trackDphi2, weight);
		}
             }
             if (nt!=0) nt->Fill((*MZSignal->zPt)[0],MZSignal->trackPhi->size(),MZSignal->hiBin,MZSignal->SignalHF,MMix->trackPhi->size(),MMix->hiBin,MMix->SignalHF,nMix);
	  }
       }
    }
    cout << "done" << nZ << endl;
    return nZ;
}

class DataAnalyzer {
public:
  DataAnalyzer(const char* filename, const char* mixFilename, const char* outFilename, const char *mytitle = "Data") :
     inf(new TFile(filename)), MZHadron(new ZHadronMessenger(*inf,string("Tree"))), mixFile(new TFile(mixFilename)), mixFileClone(new TFile(mixFilename)), MMix(new ZHadronMessenger(*mixFile,string("Tree"))), MMixEvt(new ZHadronMessenger(*mixFileClone,string("Tree"),true)), title(mytitle), outf(new TFile(outFilename, "recreate"))  {
     outf->cd();
     ntDiagnose = new TNtuple ("ntDiagnose","","zPt:nTrk:hiBin:SignalHF:nTrkMix:hiBinMix:SignalHFMix:nMix");     
  }

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
    hNZ->SetBinContent(1,getDphi(MZHadron, MMix, MMixEvt, h, hSub0, par));               // Dphi analysis

    // Second histogram with mix=true
    par.mix = true;
    hMix = new TH2D(Form("hMix%s", title.c_str()), "", 20, -4, 4, 20, -M_PI/2, 3*M_PI/2);
    hNZMix = new TH1D(Form("hNZMix%s", title.c_str()),"",1,0,1);
    hNZMix->SetBinContent(1,getDphi(MZHadron,MMix, MMixEvt, hMix, 0, par, ntDiagnose));  // Dphi analysis with mixing
  }
  
  void writeHistograms(TFile* outf) {
    outf->cd();
    smartWrite(h);
    smartWrite(hSub0);
    smartWrite(hMix);
    smartWrite(hNZ);
    smartWrite(hNZMix);
    smartWrite(ntDiagnose);
  }

  TFile *inf, *mixFile, *mixFileClone, *outf;
  TNtuple *ntDiagnose;
  TH1D *hNZ, *hNZMix;
  TH2D *h=0, *hSub0=0, *hMix=0;
  ZHadronMessenger *MZHadron, *MMix, *MMixEvt;
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
   if (printHelpMessage(argc, argv)) return 0;

   CommandLine CL(argc, argv);
   float MinZPT      = CL.GetDouble("MinZPT", 40);         // Minimum Z particle transverse momentum threshold for event selection.
   float MaxZPT      = CL.GetDouble("MaxZPT", 200);        // Maximum Z particle transverse momentum threshold for event selection.
   float MinTrackPT  = CL.GetDouble("MinTrackPT", 1);      // Minimum track transverse momentum threshold for track selection.
   float MaxTrackPT  = CL.GetDouble("MaxTrackPT", 2);      // Maximum track transverse momentum threshold for track selection.
   int   MinHiBin    = CL.GetInt   ("MinHiBin", 0);        // Minimum hiBin value for event selection.
   int   MaxHiBin    = CL.GetInt   ("MaxHiBin", 200);      // Maximum hiBin value for event selection.
   bool  IsData      = CL.GetBool  ("IsData", false);      // Determines whether the analysis is being run on actual data.
   bool  IsPP        = CL.GetBool  ("IsPP", false);        // Flag to indicate if the analysis is for Proton-Proton collisions.
   cout <<MinTrackPT<<" "<<MaxTrackPT<<endl;
   if (IsPP) {
      MinHiBin=-2;
      MaxHiBin=0;
   }

   Parameters par(MinZPT, MaxZPT, MinTrackPT, MaxTrackPT, MinHiBin, MaxHiBin);
   par.input         = CL.Get      ("Input",   "mergedSample/HISingleMuon-v5.root");            // Input file
   par.mixFile       = CL.Get      ("MixFile", "mergedSample/HISingleMuon-v5.root");            // Input Mix file
   par.output        = CL.Get      ("Output",  "output.root");                             	// Output file
   par.isSelfMixing  = CL.GetBool  ("IsSelfMixing", true); // Determine if the analysis is self-mixing
   par.isGenZ        = CL.GetBool  ("IsGenZ", false);      // Determine if the analysis is using Gen level Z     
   par.isPUReject    = CL.GetBool  ("IsPUReject", false);  // Flag to reject PU sample for systemaitcs.
   par.isMuTagged    = CL.GetBool  ("IsMuTagged", true);   // Default is true
   par.isHiBinUp     = CL.GetBool  ("IsHiBinUp", false);   // Default is false
   par.isHiBinDown   = CL.GetBool  ("IsHiBinDown", false); // Default is false
   par.scaleFactor   = CL.GetDouble("Fraction", 1.00);     // Fraction of event processed in the sample
   par.nThread       = CL.GetInt   ("nThread", 1);         // The number of threads to be used for parallel processing.
   par.nChunk        = CL.GetInt   ("nChunk", 1);          // Specifies which chunk (segment) of the data to process, used in parallel processing.
   par.nMix          = CL.GetInt   ("nMix", 10);           // Number of mixed events to be considered in the analysis.
   par.shift         = CL.GetDouble("Shift", 971.74);       // Shift of sumHF in MB matching
   par.mix = 0;
   par.isPP = IsPP;
   
   if (checkError(par)) return -1;
          
   // Analyze Data
   DataAnalyzer analyzer(par.input.c_str(), par.mixFile.c_str(), par.output.c_str(), "Data");
   analyzer.analyze(par);
   analyzer.writeHistograms(analyzer.outf);
   cout << "done!" << analyzer.outf->GetName() << endl;
}
