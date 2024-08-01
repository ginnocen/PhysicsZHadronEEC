#!/bin/basn

Output=Paper20240731
mkdir -p "$Output"

# PP MC
for P in 2
do
   ./Execute --Input Input/DYLLS0xDYLLS0_${P}Inf.root --Coefficients 1 \
      --Histograms HDeltaREEC,HDeltaREECLog --Output ${Output}/Gen_PP_PT${P}.root
   
   ./Execute --Input Input/PPMCxPPMC_Reco${P}Inf.root --Coefficients 1 \
      --Histograms HDeltaREEC,HDeltaREECLog --Output ${Output}/MC_PP_PT${P}.root
done

# PbPb MC
for C in 030 3050 5090
do
   for P in 2 
   do
      # Gen
      ./Execute --Input Input/DYLLxDYLL_Centrality${C}_${P}Inf.root,Input/DYLLxHydjet_Centrality${C}_${P}Inf.root,Input/HydjetxHydjet_Centrality${C}_${P}Inf.root,Input/HydjetxHydjet_Centrality${C}_${P}Inf_Different.root \
         --Coefficients 1,-2,-1,2 --Histograms HDeltaR,HDeltaRLog,HDeltaREEC,HDeltaREECLog \
         --Output ${Output}/Gen_Centrality${C}_PT${P}.root

      # Reco
      ./Execute --Input Input/DYLLxDYLL_Centrality${C}_Reco${P}Inf.root,Input/DYLLxHydjet_Centrality${C}_Reco${P}Inf.root,Input/HydjetxHydjet_Centrality${C}_Reco${P}Inf.root,Input/HydjetxHydjet_Centrality${C}_Reco${P}Inf_Different.root \
         --Coefficients 1,-2,-1,2 --Histograms HDeltaR,HDeltaRLog,HDeltaREEC,HDeltaREECLog \
         --Output ${Output}/MC_Centrality${C}_PT${P}.root \
         --NormalizationState MC_Centrality${C}_PT${P} \
         --NormalizationKey None,SBDiff,BBSame,BBDiff
   done
done

