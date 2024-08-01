#!/bin/bash

for P in 2 3 4
do
   for C in 030 3050 5090
   do
      ./ExecuteBasic --Input Curves/Gen_PP_PT${P}.root,Curves/Gen_Centrality${C}_PT${P}.root,Curves/Gen_SBxSB_Centrality${C}_PT${P}.root,Curves/Gen_SBxB_Centrality${C}_PT${P}.root,Curves/Gen_BxBSame_Centrality${C}_PT${P}.root,Curves/Gen_BxBDiff_Centrality${C}_PT${P}.root \
         --Systematics "None","None","None","None","None","None" \
         --Label PP,"PbPb ${C}%","(S+B)x(S+B)","(S+B)xB","BxB same","BxB diff" \
         --Output GenClosure_Centrality${C}_PT${P}.pdf \
         --Color -1,-2,-3,-4,-5,-6 \
         --ExtraText "p_{T} > $P GeV" \
         --DoLog true \
         --Histogram HDeltaREEC
   
      ./ExecuteBasic --Input Curves/Gen_PP_PT${P}.root,Curves/MC_Centrality${C}_PT${P}.root,Curves/MC_SBxSB_Centrality${C}_PT${P}.root,Curves/MC_SBxB_Centrality${C}_PT${P}.root,Curves/MC_BxBSame_Centrality${C}_PT${P}.root,Curves/MC_BxBDiff_Centrality${C}_PT${P}.root \
         --Systematics "None","None","None","None","None","None" \
         --Label PP,"PbPb ${C}%","(S+B)x(S+B)","(S+B)xB","BxB same","BxB diff" \
         --Output Closure_Centrality${C}_PT${P}.pdf \
         --Color -1,-2,-3,-4,-5,-6 \
         --ExtraText "p_{T} > $P GeV" \
         --DoLog true \
         --Histogram HDeltaREEC
   done
done
