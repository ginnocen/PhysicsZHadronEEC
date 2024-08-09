#!/bin/bash

for P in 2 3 4
do
   ./ExecuteBasic --Input Curves/MC_PP_PT${P}.root,Curves/MC_PP_PT${P}_PU.root,Curves/MC_PP_PT${P}_PU0.root \
      --Systematics None,None,None \
      --Label "PV = 1","No PV requirement","PU = 0" \
      --Output PUCheckMC_PT${P}.pdf \
      --Color -1,-2,-3 \
      --ExtraText "p_{T} = $P-200 GeV" --DoLogY true \
      --Histogram HDeltaREEC
   
   ./ExecuteBasic --Input Curves/Data_PP_PT${P}.root,Curves/Data_PP_PT${P}_PU.root \
      --Systematics None,None \
      --Label "PV = 1","No PV requirement" \
      --Output PUCheckData_PT${P}.pdf \
      --Color -1,-2,-3 \
      --ExtraText "p_{T} = $P-200 GeV" --DoLogY true \
      --Histogram HDeltaREEC
done


