#!/bin/bash

for P in 2 3 4
do
   for C in 030 3050 5090
   do
      ./ExecuteBasic --Input Curves/Data_PP_PT${P}.root,Curves/Data_Centrality${C}_PT${P}.root,Curves/Data_SBxSB_Centrality${C}_PT${P}.root,Curves/Data_SBxB_Centrality${C}_PT${P}.root,Curves/Data_BxBSame_Centrality${C}_PT${P}.root,Curves/Data_BxBDiff_Centrality${C}_PT${P}.root \
         --Systematics Systematics/Data_PP_PT${P}.root,Systematics/Data_Centrality${C}_PT${P}.root,None,None,None,None \
         --Label PP,"PbPb ${C}%","PbPb x PbPb","PbPb x MB","MB x MB (same)","MB x MB (diff)" \
         --Output DeconstructedResult_Centrality${C}_PT${P}.pdf \
         --Color -1,-2,-3,-4,-5,-6 \
         --ExtraText "p_{T} > $P GeV" \
         --Histogram HDeltaREEC
      
      ./ExecuteBasic --Input Curves/Data_PP_PT${P}.root,Curves/Data_Centrality${C}_PT${P}.root,Curves/Data_SBxSB_Centrality${C}_PT${P}.root,Curves/Data_SBxB_Centrality${C}_PT${P}.root,Curves/Data_BxBSame_Centrality${C}_PT${P}.root,Curves/Data_BxBDiff_Centrality${C}_PT${P}.root \
         --Systematics Systematics/Data_PP_PT${P}.root,Systematics/Data_Centrality${C}_PT${P}.root,None,None,None,None \
         --Label PP,"PbPb ${C}%","PbPb x PbPb","PbPb x MB","MB x MB (same)","MB x MB (diff)" \
         --Output DeconstructedResultFar_Centrality${C}_PT${P}.pdf \
         --Color -1,-2,-3,-4,-5,-6 \
         --ExtraText "p_{T} > $P GeV" \
         --WorldYMax 25000 \
         --Histogram HDeltaREEC
   done
done
