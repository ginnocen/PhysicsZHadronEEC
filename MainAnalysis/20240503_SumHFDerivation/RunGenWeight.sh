#!/bin/bash

GenHFShift="(745.5-117.6*exp(-0.1148*genZPt[0]))"

./Execute --DY Input/DYLLMC_Gen.root --MB Input/MinBiasMC_Gen.root \
   --Output GenWeight_Centrality010.root \
   --PDF GenWeight_Centrality010.pdf \
   --HFShift "$GenHFShift" \
   --Selection "(genZPt[0] > 20 && hiBin >= 0 && hiBin < 20)"

./Execute --DY Input/DYLLMC_Gen.root --MB Input/MinBiasMC_Gen.root \
   --Output GenWeight_Centrality1030.root \
   --PDF GenWeight_Centrality1030.pdf \
   --HFShift "$GenHFShift" \
   --Selection "(genZPt[0] > 20 && hiBin >= 20 && hiBin < 60)"

./Execute --DY Input/DYLLMC_Gen.root --MB Input/MinBiasMC_Gen.root \
   --Output GenWeight_Centrality3050.root \
   --PDF GenWeight_Centrality3050.pdf \
   --HFShift "$GenHFShift" \
   --Selection "(genZPt[0] > 20 && hiBin >= 60 && hiBin < 100)"
