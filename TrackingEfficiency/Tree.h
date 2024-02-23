//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed May 10 10:50:41 2023 by ROOT version 6.22/00
// from TTree Tree/Tree for ZHadron analysis, V8
// found on file: sample/HISingleMuon_V8.root
//////////////////////////////////////////////////////////

#ifndef Tree_h
#define Tree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "vector"
#include "vector"
#include "vector"

class Tree {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           run;
   Long64_t        event;
   Int_t           lumi;
   Int_t           hiBin;
   Float_t         hiHF;
   Float_t         SignalHF;
   Float_t         BackgroundHF;
   Float_t         SignalVZ;
   Float_t         NCollWeight;
   Float_t         ZWeight;
   Int_t           NVertex;
   Float_t         VX;
   Float_t         VY;
   Float_t         VZ;
   Float_t         VXError;
   Float_t         VYError;
   Float_t         VZError;
   Int_t           NPU;
   vector<double>  *zMass;
   vector<double>  *zEta;
   vector<double>  *zPhi;
   vector<double>  *zPt;
   vector<double>  *genZMass;
   vector<double>  *genZEta;
   vector<double>  *genZPhi;
   vector<double>  *genZPt;
   vector<double>  *trackPt;
   vector<double>  *trackDeta;
   vector<double>  *trackDphi;
   vector<double>  *trackPDFId;
   vector<bool>    *trackMuTagged;
   vector<double>  *trackWeight;
   vector<double>  *trackResidualWeight;
   vector<int>     *subevent;
   vector<double>  *jetPt;
   vector<double>  *jetDeta;
   vector<double>  *jetDphi;
   vector<double>  *jetRefPt;
   vector<double>  *jetRefDeta;
   vector<double>  *jetRefDphi;
   vector<bool>    *jetMuTagged;
   vector<double>  *genJetPt;
   vector<double>  *genJetEta;
   vector<double>  *genJetPhi;
   Double_t        maxOppositeDEta;
   Double_t        maxOppositeDPhi;
   Double_t        maxDEta;
   Double_t        maxDPhi;
   Double_t        maxOppositeWTADEta;
   Double_t        maxOppositeWTADPhi;
   Double_t        maxMoreOppositeWTADEta;
   Double_t        maxMoreOppositeWTADPhi;
   Double_t        maxOppositeCSWTADEta;
   Double_t        maxOppositeCSWTADPhi;
   Double_t        maxOppositeChargedWTADEta;
   Double_t        maxOppositeChargedWTADPhi;
   Double_t        maxMoreOppositeChargedWTADEta;
   Double_t        maxMoreOppositeChargedWTADPhi;
   Double_t        maxOppositeHardChargedWTADEta;
   Double_t        maxOppositeHardChargedWTADPhi;
   Double_t        maxOppositeJet12Pt;
   Double_t        maxOppositeJet12DEta;
   Double_t        maxOppositeJet12DPhi;
   Double_t        maxOppositeJet34Pt;
   Double_t        maxOppositeJet34DEta;
   Double_t        maxOppositeJet34DPhi;
   Double_t        maxOppositeJet56Pt;
   Double_t        maxOppositeJet56DEta;
   Double_t        maxOppositeJet56DPhi;
   Double_t        maxOppositeJet78Pt;
   Double_t        maxOppositeJet78DEta;
   Double_t        maxOppositeJet78DPhi;
   vector<double>  *muEta1;
   vector<double>  *muEta2;
   vector<double>  *muPhi1;
   vector<double>  *muPhi2;
   vector<double>  *muPt1;
   vector<double>  *muPt2;
   vector<double>  *genMuPt1;
   vector<double>  *genMuPt2;
   vector<double>  *genMuEta1;
   vector<double>  *genMuEta2;
   vector<double>  *genMuPhi1;
   vector<double>  *genMuPhi2;
   vector<double>  *muDeta;
   vector<double>  *muDphi;
   vector<double>  *muDR;
   vector<double>  *muDphiS;
   vector<double>  *genMuDeta;
   vector<double>  *genMuDphi;
   vector<double>  *genMuDR;
   vector<double>  *genMuDphiS;

   // List of branches
   TBranch        *b_run;   //!
   TBranch        *b_event;   //!
   TBranch        *b_lumi;   //!
   TBranch        *b_hiBin;   //!
   TBranch        *b_hiHF;   //!
   TBranch        *b_SignalHF;   //!
   TBranch        *b_BackgroundHF;   //!
   TBranch        *b_SignalVZ;   //!
   TBranch        *b_NCollWeight;   //!
   TBranch        *b_ZWeight;   //!
   TBranch        *b_NVertex;   //!
   TBranch        *b_VX;   //!
   TBranch        *b_VY;   //!
   TBranch        *b_VZ;   //!
   TBranch        *b_VXError;   //!
   TBranch        *b_VYError;   //!
   TBranch        *b_VZError;   //!
   TBranch        *b_NPU;   //!
   TBranch        *b_zMass;   //!
   TBranch        *b_zEta;   //!
   TBranch        *b_zPhi;   //!
   TBranch        *b_zPt;   //!
   TBranch        *b_genZMass;   //!
   TBranch        *b_genZEta;   //!
   TBranch        *b_genZPhi;   //!
   TBranch        *b_genZPt;   //!
   TBranch        *b_trackPt;   //!
   TBranch        *b_trackDeta;   //!
   TBranch        *b_trackDphi;   //!
   TBranch        *b_trackPDFId;   //!
   TBranch        *b_trackMuTagged;   //!
   TBranch        *b_trackWeight;   //!
   TBranch        *b_trackResidualWeight;   //!
   TBranch        *b_subevent;   //!
   TBranch        *b_jetPt;   //!
   TBranch        *b_jetDeta;   //!
   TBranch        *b_jetDphi;   //!
   TBranch        *b_jetRefPt;   //!
   TBranch        *b_jetRefDeta;   //!
   TBranch        *b_jetRefDphi;   //!
   TBranch        *b_jetMuTagged;   //!
   TBranch        *b_genJetPt;   //!
   TBranch        *b_genJetEta;   //!
   TBranch        *b_genJetPhi;   //!
   TBranch        *b_maxOppositeDEta;   //!
   TBranch        *b_maxOppositeDPhi;   //!
   TBranch        *b_maxDEta;   //!
   TBranch        *b_maxDPhi;   //!
   TBranch        *b_maxOppositeWTADEta;   //!
   TBranch        *b_maxOppositeWTADPhi;   //!
   TBranch        *b_maxMoreOppositeWTADEta;   //!
   TBranch        *b_maxMoreOppositeWTADPhi;   //!
   TBranch        *b_maxOppositeCSWTADEta;   //!
   TBranch        *b_maxOppositeCSWTADPhi;   //!
   TBranch        *b_maxOppositeChargedWTADEta;   //!
   TBranch        *b_maxOppositeChargedWTADPhi;   //!
   TBranch        *b_maxMoreOppositeChargedWTADEta;   //!
   TBranch        *b_maxMoreOppositeChargedWTADPhi;   //!
   TBranch        *b_maxOppositeHardChargedWTADEta;   //!
   TBranch        *b_maxOppositeHardChargedWTADPhi;   //!
   TBranch        *b_maxOppositeJet12Pt;   //!
   TBranch        *b_maxOppositeJet12DEta;   //!
   TBranch        *b_maxOppositeJet12DPhi;   //!
   TBranch        *b_maxOppositeJet34Pt;   //!
   TBranch        *b_maxOppositeJet34DEta;   //!
   TBranch        *b_maxOppositeJet34DPhi;   //!
   TBranch        *b_maxOppositeJet56Pt;   //!
   TBranch        *b_maxOppositeJet56DEta;   //!
   TBranch        *b_maxOppositeJet56DPhi;   //!
   TBranch        *b_maxOppositeJet78Pt;   //!
   TBranch        *b_maxOppositeJet78DEta;   //!
   TBranch        *b_maxOppositeJet78DPhi;   //!
   TBranch        *b_muEta1;   //!
   TBranch        *b_muEta2;   //!
   TBranch        *b_muPhi1;   //!
   TBranch        *b_muPhi2;   //!
   TBranch        *b_muPt1;   //!
   TBranch        *b_muPt2;   //!
   TBranch        *b_genMuPt1;   //!
   TBranch        *b_genMuPt2;   //!
   TBranch        *b_genMuEta1;   //!
   TBranch        *b_genMuEta2;   //!
   TBranch        *b_genMuPhi1;   //!
   TBranch        *b_genMuPhi2;   //!
   TBranch        *b_muDeta;   //!
   TBranch        *b_muDphi;   //!
   TBranch        *b_muDR;   //!
   TBranch        *b_muDphiS;   //!
   TBranch        *b_genMuDeta;   //!
   TBranch        *b_genMuDphi;   //!
   TBranch        *b_genMuDR;   //!
   TBranch        *b_genMuDphiS;   //!

   Tree(TTree *tree=0);
   virtual ~Tree();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef Tree_cxx
Tree::Tree(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("sample/HISingleMuon_V8.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("sample/HISingleMuon_V8.root");
      }
      f->GetObject("Tree",tree);

   }
   Init(tree);
}

Tree::~Tree()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t Tree::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t Tree::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void Tree::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   zMass = 0;
   zEta = 0;
   zPhi = 0;
   zPt = 0;
   genZMass = 0;
   genZEta = 0;
   genZPhi = 0;
   genZPt = 0;
   trackPt = 0;
   trackDeta = 0;
   trackDphi = 0;
   trackPDFId = 0;
   trackMuTagged = 0;
   trackWeight = 0;
   trackResidualWeight = 0;
   subevent = 0;
   jetPt = 0;
   jetDeta = 0;
   jetDphi = 0;
   jetRefPt = 0;
   jetRefDeta = 0;
   jetRefDphi = 0;
   jetMuTagged = 0;
   genJetPt = 0;
   genJetEta = 0;
   genJetPhi = 0;
   muEta1 = 0;
   muEta2 = 0;
   muPhi1 = 0;
   muPhi2 = 0;
   muPt1 = 0;
   muPt2 = 0;
   genMuPt1 = 0;
   genMuPt2 = 0;
   genMuEta1 = 0;
   genMuEta2 = 0;
   genMuPhi1 = 0;
   genMuPhi2 = 0;
   muDeta = 0;
   muDphi = 0;
   muDR = 0;
   muDphiS = 0;
   genMuDeta = 0;
   genMuDphi = 0;
   genMuDR = 0;
   genMuDphiS = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("run", &run, &b_run);
   fChain->SetBranchAddress("event", &event, &b_event);
   fChain->SetBranchAddress("lumi", &lumi, &b_lumi);
   fChain->SetBranchAddress("hiBin", &hiBin, &b_hiBin);
   fChain->SetBranchAddress("hiHF", &hiHF, &b_hiHF);
   fChain->SetBranchAddress("SignalHF", &SignalHF, &b_SignalHF);
   fChain->SetBranchAddress("BackgroundHF", &BackgroundHF, &b_BackgroundHF);
   fChain->SetBranchAddress("SignalVZ", &SignalVZ, &b_SignalVZ);
   fChain->SetBranchAddress("NCollWeight", &NCollWeight, &b_NCollWeight);
   fChain->SetBranchAddress("ZWeight", &ZWeight, &b_ZWeight);
   fChain->SetBranchAddress("NVertex", &NVertex, &b_NVertex);
   fChain->SetBranchAddress("VX", &VX, &b_VX);
   fChain->SetBranchAddress("VY", &VY, &b_VY);
   fChain->SetBranchAddress("VZ", &VZ, &b_VZ);
   fChain->SetBranchAddress("VXError", &VXError, &b_VXError);
   fChain->SetBranchAddress("VYError", &VYError, &b_VYError);
   fChain->SetBranchAddress("VZError", &VZError, &b_VZError);
   fChain->SetBranchAddress("NPU", &NPU, &b_NPU);
   fChain->SetBranchAddress("zMass", &zMass, &b_zMass);
   fChain->SetBranchAddress("zEta", &zEta, &b_zEta);
   fChain->SetBranchAddress("zPhi", &zPhi, &b_zPhi);
   fChain->SetBranchAddress("zPt", &zPt, &b_zPt);
   fChain->SetBranchAddress("genZMass", &genZMass, &b_genZMass);
   fChain->SetBranchAddress("genZEta", &genZEta, &b_genZEta);
   fChain->SetBranchAddress("genZPhi", &genZPhi, &b_genZPhi);
   fChain->SetBranchAddress("genZPt", &genZPt, &b_genZPt);
   fChain->SetBranchAddress("trackPt", &trackPt, &b_trackPt);
   fChain->SetBranchAddress("trackDeta", &trackDeta, &b_trackDeta);
   fChain->SetBranchAddress("trackDphi", &trackDphi, &b_trackDphi);
   fChain->SetBranchAddress("trackPDFId", &trackPDFId, &b_trackPDFId);
   fChain->SetBranchAddress("trackMuTagged", &trackMuTagged, &b_trackMuTagged);
   fChain->SetBranchAddress("trackWeight", &trackWeight, &b_trackWeight);
   fChain->SetBranchAddress("trackResidualWeight", &trackResidualWeight, &b_trackResidualWeight);
   fChain->SetBranchAddress("subevent", &subevent, &b_subevent);
   fChain->SetBranchAddress("jetPt", &jetPt, &b_jetPt);
   fChain->SetBranchAddress("jetDeta", &jetDeta, &b_jetDeta);
   fChain->SetBranchAddress("jetDphi", &jetDphi, &b_jetDphi);
   fChain->SetBranchAddress("jetRefPt", &jetRefPt, &b_jetRefPt);
   fChain->SetBranchAddress("jetRefDeta", &jetRefDeta, &b_jetRefDeta);
   fChain->SetBranchAddress("jetRefDphi", &jetRefDphi, &b_jetRefDphi);
   fChain->SetBranchAddress("jetMuTagged", &jetMuTagged, &b_jetMuTagged);
   fChain->SetBranchAddress("genJetPt", &genJetPt, &b_genJetPt);
   fChain->SetBranchAddress("genJetEta", &genJetEta, &b_genJetEta);
   fChain->SetBranchAddress("genJetPhi", &genJetPhi, &b_genJetPhi);
   fChain->SetBranchAddress("maxOppositeDEta", &maxOppositeDEta, &b_maxOppositeDEta);
   fChain->SetBranchAddress("maxOppositeDPhi", &maxOppositeDPhi, &b_maxOppositeDPhi);
   fChain->SetBranchAddress("maxDEta", &maxDEta, &b_maxDEta);
   fChain->SetBranchAddress("maxDPhi", &maxDPhi, &b_maxDPhi);
   fChain->SetBranchAddress("maxOppositeWTADEta", &maxOppositeWTADEta, &b_maxOppositeWTADEta);
   fChain->SetBranchAddress("maxOppositeWTADPhi", &maxOppositeWTADPhi, &b_maxOppositeWTADPhi);
   fChain->SetBranchAddress("maxMoreOppositeWTADEta", &maxMoreOppositeWTADEta, &b_maxMoreOppositeWTADEta);
   fChain->SetBranchAddress("maxMoreOppositeWTADPhi", &maxMoreOppositeWTADPhi, &b_maxMoreOppositeWTADPhi);
   fChain->SetBranchAddress("maxOppositeCSWTADEta", &maxOppositeCSWTADEta, &b_maxOppositeCSWTADEta);
   fChain->SetBranchAddress("maxOppositeCSWTADPhi", &maxOppositeCSWTADPhi, &b_maxOppositeCSWTADPhi);
   fChain->SetBranchAddress("maxOppositeChargedWTADEta", &maxOppositeChargedWTADEta, &b_maxOppositeChargedWTADEta);
   fChain->SetBranchAddress("maxOppositeChargedWTADPhi", &maxOppositeChargedWTADPhi, &b_maxOppositeChargedWTADPhi);
   fChain->SetBranchAddress("maxMoreOppositeChargedWTADEta", &maxMoreOppositeChargedWTADEta, &b_maxMoreOppositeChargedWTADEta);
   fChain->SetBranchAddress("maxMoreOppositeChargedWTADPhi", &maxMoreOppositeChargedWTADPhi, &b_maxMoreOppositeChargedWTADPhi);
   fChain->SetBranchAddress("maxOppositeHardChargedWTADEta", &maxOppositeHardChargedWTADEta, &b_maxOppositeHardChargedWTADEta);
   fChain->SetBranchAddress("maxOppositeHardChargedWTADPhi", &maxOppositeHardChargedWTADPhi, &b_maxOppositeHardChargedWTADPhi);
   fChain->SetBranchAddress("maxOppositeJet12Pt", &maxOppositeJet12Pt, &b_maxOppositeJet12Pt);
   fChain->SetBranchAddress("maxOppositeJet12DEta", &maxOppositeJet12DEta, &b_maxOppositeJet12DEta);
   fChain->SetBranchAddress("maxOppositeJet12DPhi", &maxOppositeJet12DPhi, &b_maxOppositeJet12DPhi);
   fChain->SetBranchAddress("maxOppositeJet34Pt", &maxOppositeJet34Pt, &b_maxOppositeJet34Pt);
   fChain->SetBranchAddress("maxOppositeJet34DEta", &maxOppositeJet34DEta, &b_maxOppositeJet34DEta);
   fChain->SetBranchAddress("maxOppositeJet34DPhi", &maxOppositeJet34DPhi, &b_maxOppositeJet34DPhi);
   fChain->SetBranchAddress("maxOppositeJet56Pt", &maxOppositeJet56Pt, &b_maxOppositeJet56Pt);
   fChain->SetBranchAddress("maxOppositeJet56DEta", &maxOppositeJet56DEta, &b_maxOppositeJet56DEta);
   fChain->SetBranchAddress("maxOppositeJet56DPhi", &maxOppositeJet56DPhi, &b_maxOppositeJet56DPhi);
   fChain->SetBranchAddress("maxOppositeJet78Pt", &maxOppositeJet78Pt, &b_maxOppositeJet78Pt);
   fChain->SetBranchAddress("maxOppositeJet78DEta", &maxOppositeJet78DEta, &b_maxOppositeJet78DEta);
   fChain->SetBranchAddress("maxOppositeJet78DPhi", &maxOppositeJet78DPhi, &b_maxOppositeJet78DPhi);
   fChain->SetBranchAddress("muEta1", &muEta1, &b_muEta1);
   fChain->SetBranchAddress("muEta2", &muEta2, &b_muEta2);
   fChain->SetBranchAddress("muPhi1", &muPhi1, &b_muPhi1);
   fChain->SetBranchAddress("muPhi2", &muPhi2, &b_muPhi2);
   fChain->SetBranchAddress("muPt1", &muPt1, &b_muPt1);
   fChain->SetBranchAddress("muPt2", &muPt2, &b_muPt2);
   fChain->SetBranchAddress("genMuPt1", &genMuPt1, &b_genMuPt1);
   fChain->SetBranchAddress("genMuPt2", &genMuPt2, &b_genMuPt2);
   fChain->SetBranchAddress("genMuEta1", &genMuEta1, &b_genMuEta1);
   fChain->SetBranchAddress("genMuEta2", &genMuEta2, &b_genMuEta2);
   fChain->SetBranchAddress("genMuPhi1", &genMuPhi1, &b_genMuPhi1);
   fChain->SetBranchAddress("genMuPhi2", &genMuPhi2, &b_genMuPhi2);
   fChain->SetBranchAddress("muDeta", &muDeta, &b_muDeta);
   fChain->SetBranchAddress("muDphi", &muDphi, &b_muDphi);
   fChain->SetBranchAddress("muDR", &muDR, &b_muDR);
   fChain->SetBranchAddress("muDphiS", &muDphiS, &b_muDphiS);
   fChain->SetBranchAddress("genMuDeta", &genMuDeta, &b_genMuDeta);
   fChain->SetBranchAddress("genMuDphi", &genMuDphi, &b_genMuDphi);
   fChain->SetBranchAddress("genMuDR", &genMuDR, &b_genMuDR);
   fChain->SetBranchAddress("genMuDphiS", &genMuDphiS, &b_genMuDphiS);
   Notify();
}

Bool_t Tree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void Tree::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t Tree::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef Tree_cxx
