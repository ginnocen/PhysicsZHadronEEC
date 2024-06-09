#include <iostream>
using namespace std;

#include "CommandLine.h"
#include "Messenger.h"

#include "Helper.h"

int main(int argc, char *argv[])
{
   CommandLine CL(argc, argv);

   string InputFileName  = CL.Get("Input");
   string OutputFileName = CL.Get("Output");
   int NegativeID        = CL.GetInteger("NegativeID", 2);

   TFile InputFile(InputFileName.c_str());

   TTree *InputTree = (TTree *)InputFile.Get("t");

   float weight, cross_section;
   float Zmass, Zpt, Zeta, Zphi;
   vector<float>   *px = nullptr;
   vector<float>   *py = nullptr;
   vector<float>   *pz = nullptr;
   vector<float>   *mass = nullptr;
   vector<float>   *pt = nullptr;
   vector<float>   *eta = nullptr;
   vector<float>   *phi = nullptr;
   vector<int>     *pdg_id = nullptr;
   vector<int>     *label = nullptr;

   InputTree->SetBranchAddress("weight", &weight);
   InputTree->SetBranchAddress("cross_section", &cross_section);
   InputTree->SetBranchAddress("px", &px);
   InputTree->SetBranchAddress("py", &py);
   InputTree->SetBranchAddress("pz", &pz);
   InputTree->SetBranchAddress("mass", &mass);
   InputTree->SetBranchAddress("pt", &pt);
   InputTree->SetBranchAddress("eta", &eta);
   InputTree->SetBranchAddress("phi", &phi);
   InputTree->SetBranchAddress("pdg_id", &pdg_id);
   InputTree->SetBranchAddress("label", &label);
   InputTree->SetBranchAddress("Zmass", &Zmass);
   InputTree->SetBranchAddress("Zpt", &Zpt);
   InputTree->SetBranchAddress("Zeta", &Zeta);
   InputTree->SetBranchAddress("Zphi", &Zphi);

   TFile OutputFile(OutputFileName.c_str(), "RECREATE");

   TTree OutputTree("Tree", "");
   ZHadronMessenger MZHadron;
   MZHadron.SetBranch(&OutputTree);

   int EntryCount = InputTree->GetEntries();
   for(int iE = 0; iE < EntryCount; iE++)
   {
      InputTree->GetEntry(iE);

      // There is also "cross section", not sure what it is
      MZHadron.EventWeight = weight;

      int NParticle = px->size();
      for(int i = 0; i < NParticle; i++)
      {
         double PX = px->at(i);
         double PY = py->at(i);
         double PZ = pz->at(i);
         double Mass = mass->at(i);
         double E = sqrt(PX * PX + PY * PY + PZ * PZ * Mass * Mass);
         int ID = pdg_id->at(i);
         double Eta = eta->at(i);
         double Y = 0.5 * log((E + PZ) / (E - PZ));
         double Phi = phi->at(i);
         int Status = label->at(i);

         // -2 is initial particle
         // 0 is positive particles
         // 1 is positive particles
         // 2 is negative particles
     
         double Weight = 0;
         if(Status == 0 || Status == 1)
            Weight = 1;
         if(Status == NegativeID)
            Weight = -1;
  
         MZHadron.trackPt->push_back(sqrt(PX * PX + PY * PY));
         MZHadron.trackPDFId->push_back(ID);
         MZHadron.trackEta->push_back(Eta);
         MZHadron.trackY->push_back(Y);
         MZHadron.trackPhi->push_back(Phi);
         MZHadron.trackMuTagged->push_back(false);
         MZHadron.trackMuDR->push_back(0);
         MZHadron.trackWeight->push_back(Weight);
         MZHadron.trackResidualWeight->push_back(1);
         MZHadron.trackCharge->push_back(999);
         MZHadron.subevent->push_back(0);

         // Add in the stored Z as the first entry
         double Zpz = Zpt * sinh(Zeta);
         double Zp  = Zpt * cosh(Zeta);
         double Ze  = sqrt(Zp * Zp + Zmass * Zmass);
         double Zy  = 0.5 * log((Ze + Zpz) / (Ze - Zpz));

         MZHadron.genZMass->push_back(Zmass);
         MZHadron.genZPt->push_back  (Zpt);
         MZHadron.genZPhi->push_back (Zphi);
         MZHadron.genZEta->push_back (Zeta);
         MZHadron.genZY->push_back   (Zy);

         MZHadron.genMuPt1->push_back(-1);
         MZHadron.genMuPt2->push_back(-1);
         MZHadron.genMuEta1->push_back(-1);
         MZHadron.genMuEta2->push_back(-1);
         MZHadron.genMuPhi1->push_back(-1);
         MZHadron.genMuPhi2->push_back(-1);

         MZHadron.genMuDeta->push_back(-1);
         MZHadron.genMuDphi->push_back(-1);
         MZHadron.genMuDR->push_back(-1);
         MZHadron.genMuDphiS->push_back(-1);

      }

      FillAuxiliaryVariables(MZHadron);
      MZHadron.FillEntry();
   }

   OutputTree.Write();
   OutputFile.Close();

   InputFile.Close();

   return 0;
}










