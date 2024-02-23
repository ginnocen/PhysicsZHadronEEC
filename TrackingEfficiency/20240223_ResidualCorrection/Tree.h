//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Feb 23 10:57:25 2024 by ROOT version 6.24/06
// from TTree Tree/Tree for ZHadron EEC analysis
// found on file: Skim_RECO.root
//////////////////////////////////////////////////////////

#ifndef Tree_h
#define Tree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
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
   Int_t           hiBinUp;
   Int_t           hiBinDown;
   Float_t         hiHF;
   Float_t         SignalHF;
   Float_t         BackgroundHF;
   Float_t         SignalVZ;
   Float_t         NCollWeight;
   Float_t         ZWeight;
   Float_t         VZWeight;
   Float_t         ExtraZWeight[12];
   Int_t           NVertex;
   Float_t         VX;
   Float_t         VY;
   Float_t         VZ;
   Float_t         VXError;
   Float_t         VYError;
   Float_t         VZError;
   Int_t           NPU;
   vector<float>   *zMass;
   vector<float>   *zEta;
   vector<float>   *zPhi;
   vector<float>   *zPt;
   vector<float>   *genZMass;
   vector<float>   *genZEta;
   vector<float>   *genZPhi;
   vector<float>   *genZPt;
   vector<float>   *trackPt;
   vector<float>   *trackPDFId;
   vector<float>   *trackPhi;
   vector<float>   *trackEta;
   vector<float>   *trackWeight;
   vector<int>     *trackCharge;
   vector<int>     *subevent;
   vector<float>   *muEta1;
   vector<float>   *muEta2;
   vector<float>   *muPhi1;
   vector<float>   *muPhi2;
   vector<float>   *muPt1;
   vector<float>   *muPt2;
   vector<float>   *genMuPt1;
   vector<float>   *genMuPt2;
   vector<float>   *genMuEta1;
   vector<float>   *genMuEta2;
   vector<float>   *genMuPhi1;
   vector<float>   *genMuPhi2;
   vector<float>   *muDeta;
   vector<float>   *muDphi;
   vector<float>   *muDR;
   vector<float>   *muDphiS;
   vector<float>   *genMuDeta;
   vector<float>   *genMuDphi;
   vector<float>   *genMuDR;
   vector<float>   *genMuDphiS;

   // List of branches
   TBranch        *b_run;   //!
   TBranch        *b_event;   //!
   TBranch        *b_lumi;   //!
   TBranch        *b_hiBin;   //!
   TBranch        *b_hiBinUp;   //!
   TBranch        *b_hiBinDown;   //!
   TBranch        *b_hiHF;   //!
   TBranch        *b_SignalHF;   //!
   TBranch        *b_BackgroundHF;   //!
   TBranch        *b_SignalVZ;   //!
   TBranch        *b_NCollWeight;   //!
   TBranch        *b_ZWeight;   //!
   TBranch        *b_VZWeight;   //!
   TBranch        *b_ExtraZWeight;   //!
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
   TBranch        *b_trackPDFId;   //!
   TBranch        *b_trackPhi;   //!
   TBranch        *b_trackEta;   //!
   TBranch        *b_trackWeight;   //!
   TBranch        *b_trackCharge;   //!
   TBranch        *b_subevent;   //!
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
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("Skim_RECO.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("Skim_RECO.root");
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
   trackPDFId = 0;
   trackPhi = 0;
   trackEta = 0;
   trackWeight = 0;
   trackCharge = 0;
   subevent = 0;
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
   fChain->SetBranchAddress("hiBinUp", &hiBinUp, &b_hiBinUp);
   fChain->SetBranchAddress("hiBinDown", &hiBinDown, &b_hiBinDown);
   fChain->SetBranchAddress("hiHF", &hiHF, &b_hiHF);
   fChain->SetBranchAddress("SignalHF", &SignalHF, &b_SignalHF);
   fChain->SetBranchAddress("BackgroundHF", &BackgroundHF, &b_BackgroundHF);
   fChain->SetBranchAddress("SignalVZ", &SignalVZ, &b_SignalVZ);
   fChain->SetBranchAddress("NCollWeight", &NCollWeight, &b_NCollWeight);
   fChain->SetBranchAddress("ZWeight", &ZWeight, &b_ZWeight);
   fChain->SetBranchAddress("VZWeight", &VZWeight, &b_VZWeight);
   fChain->SetBranchAddress("ExtraZWeight", ExtraZWeight, &b_ExtraZWeight);
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
   fChain->SetBranchAddress("trackPDFId", &trackPDFId, &b_trackPDFId);
   fChain->SetBranchAddress("trackPhi", &trackPhi, &b_trackPhi);
   fChain->SetBranchAddress("trackEta", &trackEta, &b_trackEta);
   fChain->SetBranchAddress("trackWeight", &trackWeight, &b_trackWeight);
   fChain->SetBranchAddress("trackCharge", &trackCharge, &b_trackCharge);
   fChain->SetBranchAddress("subevent", &subevent, &b_subevent);
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
