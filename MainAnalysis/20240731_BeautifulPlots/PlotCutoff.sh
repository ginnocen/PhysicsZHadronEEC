#!/bin/bash

for P in 4
do
   for C in 030 3050 5090
   do
      for U in "" to20 to100 to200
      do
         ./ExecuteBasic --Input Curves/Gen_PP_PT${P}${U}.root,Curves/GenPP_PP_PT${P}${U}.root,Curves/MC_PP_PT${P}${U}.root,Curves/MC_Centrality${C}_PT${P}${U}.root \
            --Systematics "None","None","None","None" \
            --Label "DYLL subevent 0","pp gen","pp Reco","PbPb ${C}%" \
            --Output MCCutoff_Centrality${C}_PT${P}${U}.pdf \
            --Color -1,-2,-3,-4 \
            --DoLogY true \
            --ExtraText "p_{T} : 4${U} GeV" \
            --Histogram HDeltaREEC
      done
   done
done
