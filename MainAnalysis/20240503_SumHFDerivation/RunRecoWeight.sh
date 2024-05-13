#!/bin/bash

RecoHFShift="(763.1539360425459-101.2995115409047*exp(-0.09666472578946463*zPt[0])-82.36975543861207)"

./Execute --DY Input/DYLLMC_Reco.root --MB Input/MinBiasMC_Reco.root \
   --Output RecoWeight_Centrality010.root \
   --PDF RecoWeight_Centrality010.pdf \
   --HFShift "$RecoHFShift" \
   --Selection "(zPt[0] > 20 && hiBin >= 0 && hiBin < 20)"

./Execute --DY Input/DYLLMC_Reco.root --MB Input/MinBiasMC_Reco.root \
   --Output RecoWeight_Centrality1030.root \
   --PDF RecoWeight_Centrality1030.pdf \
   --HFShift "$RecoHFShift" \
   --Selection "(zPt[0] > 20 && hiBin >= 20 && hiBin < 60)"

./Execute --DY Input/DYLLMC_Reco.root --MB Input/MinBiasMC_Reco.root \
   --Output RecoWeight_Centrality3050.root \
   --PDF RecoWeight_Centrality3050.pdf \
   --HFShift "$RecoHFShift" \
   --Selection "(zPt[0] > 20 && hiBin >= 60 && hiBin < 100)"

./Execute --DY Input/DYLLMC_Reco.root --MB Input/MinBiasMC_Reco.root \
   --Output RecoWeight_Centrality030.root \
   --PDF RecoWeight_Centrality030.pdf \
   --HFShift "$RecoHFShift" \
   --Selection "(zPt[0] > 20 && hiBin >= 0 && hiBin < 60)"
