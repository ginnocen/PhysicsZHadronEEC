#!/bin/bash

for P in 4
do
   ./ExecuteBasic --Input Curves/GenPP_PP_PT${P}.root,Curves/MC_PP_PT${P}.root,Curves/MC_PP_PT${P}_NoResidual.root \
      --Systematics "None","None","None" \
      --Label "PP Gen","PP Reco","PP Reco No Residual" \
      --Output PPNoResidual_PT${P}.pdf \
      --Color -1,-2,-3 \
      --ExtraText "p_{T} = $P-200 GeV" \
      --DoLogY false \
      --Histogram HDeltaREEC
   
   ./ExecuteBasic --Input Curves/GenPP_PP_PT${P}.root,Curves/MC_PP_PT${P}.root,Curves/MC_PP_PT${P}_NoResidual.root \
      --Systematics "None","None","None" \
      --Label "PP Gen","PP Reco","PP Reco No Residual" \
      --Output PPNoResidualLog_PT${P}.pdf \
      --Color -1,-2,-3 \
      --ExtraText "p_{T} = $P-200 GeV" \
      --DoLogY true \
      --Histogram HDeltaREEC
      
   for C in 030 3050 5090
   do
      ./ExecuteBasic --Input Curves/Gen_PP_PT${P}.root,Curves/MC_Centrality${C}_PT${P}.root,Curves/MC_Centrality${C}_PT${P}_NoResidual.root \
         --Systematics "None","None","None" \
         --Label "PP Gen","PbPb ${C}%","PbPb ${C}% No Residual" \
         --Output PbPbNoResidual_Centrality${C}_PT${P}.pdf \
         --Color -4,-5,-6 \
         --ExtraText "p_{T} = $P-200 GeV" \
         --DoLogY false \
         --Histogram HDeltaREEC
      
      ./ExecuteBasic --Input Curves/Gen_PP_PT${P}.root,Curves/MC_Centrality${C}_PT${P}.root,Curves/MC_Centrality${C}_PT${P}_NoResidual.root \
         --Systematics "None","None","None" \
         --Label "PP Gen","PbPb ${C}%","PbPb ${C}% No Residual" \
         --Output PbPbNoResidualLog_Centrality${C}_PT${P}.pdf \
         --Color -4,-5,-6 \
         --ExtraText "p_{T} = $P-200 GeV" \
         --DoLogY true \
         --Histogram HDeltaREEC
   done
done
