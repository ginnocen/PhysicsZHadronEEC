#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

#include "CommandLine.h"
#include "Messenger.h"

#include "Helper.h"

int main(int argc, char *argv[])
{
   CommandLine CL(argc, argv);

   string InputFileName  = CL.Get("Input");
   string OutputFileName = CL.Get("Output");
   int NegativeID        = CL.GetInteger("NegativeID", -999999);

   TFile OutputFile(OutputFileName.c_str(), "RECREATE");

   TTree OutputTree("Tree", "");
   ZHadronMessenger MZHadron;
   MZHadron.SetBranch(&OutputTree);

   ifstream in(InputFileName);

   while(in)
   {
      char ch[10240];
      ch[0] = '\0';
      in.getline(ch, 10239, '\n');
      if(ch[0] == '\0')
         continue;

      stringstream str(ch);

      vector<string> list;
      while(str)
      {
         string Temp = "";
         str >> Temp;
         if(Temp != "")
            list.push_back(Temp);
      }

      if(list.size() == 0)
         continue;
      if(list[0][0] == '#')
         continue;
      if(list[0] == "end")
      {
         FillAuxiliaryVariables(MZHadron);
         MZHadron.FillEntry();
         MZHadron.Clear();
         continue;
      }
      if(list.size() == 6)
      {
         double px  = stof(list[0]);
         double py  = stof(list[1]);
         double pz  = stof(list[2]);
         double m   = stof(list[3]);
         int ID     = stoi(list[4]);
         int Status = stoi(list[5]);

         double p   = sqrt(px * px + py * py + pz * pz);
         double eta = 0.5 * log((p + pz) / (p - pz));
         double e   = sqrt(p * p + 0.14 * 0.14);
         double y   = 0.5 * log((e + pz) / (e - pz));

         double phi = 0;
         if(px == 0 && py > 0)        phi = M_PI / 2;
         else if(px == 0 && py < 0)   phi = -M_PI / 2;
         else                         phi = atan(py / px);
         if(px < 0)                   phi = phi + M_PI;
         if(phi > M_PI)               phi = phi - 2 * M_PI;

         double Weight = 0;
         if(Status == 0)
            Weight = 1;
         if(Status == NegativeID)
            Weight = -1;

         MZHadron.trackPt->push_back(sqrt(px * px + py * py));
         MZHadron.trackPDFId->push_back(ID);
         MZHadron.trackEta->push_back(eta);
         MZHadron.trackY->push_back(y);
         MZHadron.trackPhi->push_back(phi);
         MZHadron.trackMuTagged->push_back(false);
         MZHadron.trackMuDR->push_back(false);
         MZHadron.trackWeight->push_back(Weight);
         MZHadron.trackResidualWeight->push_back(1);
         MZHadron.trackCharge->push_back(999);
         MZHadron.subevent->push_back(0);
      }
   }
   if(MZHadron.trackPt->size() > 0)
   {
      FillAuxiliaryVariables(MZHadron);
      MZHadron.FillEntry();
   }

   in.close();

   OutputTree.Write();
   OutputFile.Close();

   return 0;
}





