#!/bin/bash

Output=Paper20240808
mkdir -p "$Output"

Histogram=HDeltaR,HDeltaRLog,HDeltaREEC,HDeltaREECLog,HDeltaREECFine,HDeltaREECLogFine

# PP MC
for P in 2 3 4
do
   # Gen
   ./Execute --Input Input/DYLLS0xDYLLS0_${P}to200.root --Coefficients 1 \
      --Histograms $Histogram --Output ${Output}/Gen_PP_PT${P}.root
   
   # PPGen
   ./Execute --Input Input/PPMCxPPMC_${P}to200.root --Coefficients 1 \
      --Histograms $Histogram --Output ${Output}/GenPP_PP_PT${P}.root
   
   # Reco
   ./Execute --Input Input/PPMCxPPMC_Reco${P}to200.root --Coefficients 1 \
      --Histograms $Histogram --Output ${Output}/MC_PP_PT${P}.root
   
   # PU variants
   ./Execute --Input Input/PPMCxPPMC_Reco${P}to200_PU.root --Coefficients 1 \
      --Histograms $Histogram --Output ${Output}/MC_PP_PT${P}_PU.root
   ./Execute --Input Input/PPMCxPPMC_Reco${P}to200_PU0.root --Coefficients 1 \
      --Histograms $Histogram --Output ${Output}/MC_PP_PT${P}_PU0.root
done

for P in 4
do
   for U in to20 to100 to200
   do
      # Cutoffs
      ./Execute --Input Input/DYLLS0xDYLLS0_${P}${U}_Cutoff.root --Coefficients 1 \
         --Histograms $Histogram --Output ${Output}/Gen_PP_PT${P}${U}.root
      ./Execute --Input Input/PPMCxPPMC_${P}${U}_Cutoff.root --Coefficients 1 \
         --Histograms $Histogram --Output ${Output}/GenPP_PP_PT${P}${U}.root
      ./Execute --Input Input/PPMCxPPMC_Reco${P}${U}_Cutoff.root --Coefficients 1 \
         --Histograms $Histogram --Output ${Output}/MC_PP_PT${P}${U}.root
   done

   # No residual
   ./Execute --Input Input/PPMCxPPMC_Reco${P}to200_NoResidual.root --Coefficients 1 \
      --Histograms $Histogram --Output ${Output}/MC_PP_PT${P}_NoResidual.root
done

# PbPb MC
for C in 030 3050 5090
do
   for P in 2 3 4
   do
      # Gen
      ./Execute --Input Input/DYLLxDYLL_Centrality${C}_${P}to200.root,Input/DYLLxHydjet_Centrality${C}_${P}to200.root,Input/HydjetxHydjet_Centrality${C}_${P}to200.root,Input/HydjetxHydjet_Centrality${C}_${P}to200_Different.root \
         --Coefficients 1,-1,-1,1 --Histograms $Histogram \
         --NormalizationState XGenMC_Centrality${C}_PT${P} \
         --NormalizationKey None,SBDiff,BBSame,BBDiff \
         --Output ${Output}/Gen_Centrality${C}_PT${P}.root

      # Gen deconstructed
      ./Execute --Input Input/DYLLxDYLL_Centrality${C}_${P}to200.root \
         --Coefficients 1 --Histograms HDeltaR,HDeltaRLog,$Histogram \
         --Output ${Output}/Gen_SBxSB_Centrality${C}_PT${P}.root
      ./Execute --Input Input/DYLLxHydjet_Centrality${C}_${P}to200.root \
         --Coefficients 1 --Histograms HDeltaR,HDeltaRLog,$Histogram \
         --Output ${Output}/Gen_SBxB_Centrality${C}_PT${P}.root
      ./Execute --Input Input/HydjetxHydjet_Centrality${C}_${P}to200.root \
         --Coefficients 1 --Histograms HDeltaR,HDeltaRLog,$Histogram \
         --Output ${Output}/Gen_BxBSame_Centrality${C}_PT${P}.root
      ./Execute --Input Input/HydjetxHydjet_Centrality${C}_${P}to200_Different.root \
         --Coefficients 1 --Histograms HDeltaR,HDeltaRLog,$Histogram \
         --Output ${Output}/Gen_BxBDiff_Centrality${C}_PT${P}.root

      # Reco
      ./Execute --Input Input/DYLLxDYLL_Centrality${C}_Reco${P}to200.root,Input/DYLLxHydjet_Centrality${C}_Reco${P}to200.root,Input/HydjetxHydjet_Centrality${C}_Reco${P}to200.root,Input/HydjetxHydjet_Centrality${C}_Reco${P}to200_Different.root \
         --Coefficients 1,-1,-1,1 --Histograms $Histogram \
         --Output ${Output}/MC_Centrality${C}_PT${P}.root \
         --NormalizationState XDYLLMC_Centrality${C}_PT${P} \
         --NormalizationKey None,SBDiff,BBSame,BBDiff

      # Reco deconstructed
      ./Execute --Input Input/DYLLxDYLL_Centrality${C}_Reco${P}to200.root \
         --Coefficients 1 --Histograms HDeltaR,HDeltaRLog,$Histogram \
         --Output ${Output}/MC_SBxSB_Centrality${C}_PT${P}.root \
         --NormalizationState MC_Centrality${C}_PT${P} \
         --NormalizationKey None
      ./Execute --Input Input/DYLLxHydjet_Centrality${C}_Reco${P}to200.root \
         --Coefficients 1 --Histograms HDeltaR,HDeltaRLog,$Histogram \
         --Output ${Output}/MC_SBxB_Centrality${C}_PT${P}.root \
         --NormalizationState MC_Centrality${C}_PT${P} \
         --NormalizationKey SBDiff
      ./Execute --Input Input/HydjetxHydjet_Centrality${C}_Reco${P}to200.root\
         --Coefficients 1 --Histograms HDeltaR,HDeltaRLog,$Histogram \
         --Output ${Output}/MC_BxBSame_Centrality${C}_PT${P}.root \
         --NormalizationState MC_Centrality${C}_PT${P} \
         --NormalizationKey BBSame
      ./Execute --Input Input/HydjetxHydjet_Centrality${C}_Reco${P}to200_Different.root \
         --Coefficients 1,-1,-1,1 --Histograms HDeltaR,HDeltaRLog,$Histogram \
         --Output ${Output}/MC_BxBDiff_Centrality${C}_PT${P}.root \
         --NormalizationState MC_Centrality${C}_PT${P} \
         --NormalizationKey BBDiff
   done
done

for C in 030 3050 5090
do
   for P in 4
   do
      for U in to20 to100 to200
      do
         # Cutoffs
         ./Execute --Input Input/DYLLxDYLL_Centrality${C}_Reco${P}${U}_Cutoff.root,Input/DYLLxHydjet_Centrality${C}_Reco${P}${U}_Cutoff.root,Input/HydjetxHydjet_Centrality${C}_Reco${P}${U}_Cutoff.root,Input/HydjetxHydjet_Centrality${C}_Reco${P}${U}_Cutoff_Different.root \
            --Coefficients 1,-1,-1,1 --Histograms $Histogram \
            --Output ${Output}/MC_Centrality${C}_PT${P}${U}.root \
            --NormalizationState XDYLLMC_Centrality${C}_PT${P} \
            --NormalizationKey None,SBDiff,BBSame,BBDiff
      done

      # No residual
      ./Execute --Input Input/DYLLxDYLL_Centrality${C}_Reco${P}to200_NoResidual.root,Input/DYLLxHydjet_Centrality${C}_Reco${P}to200_NoResidual.root,Input/HydjetxHydjet_Centrality${C}_Reco${P}to200_NoResidual.root,Input/HydjetxHydjet_Centrality${C}_Reco${P}to200_NoResidual_Different.root \
         --Coefficients 1,-1,-1,1 --Histograms $Histogram \
         --Output ${Output}/MC_Centrality${C}_PT${P}_NoResidual.root \
         --NormalizationState XDYLLMC_Centrality${C}_PT${P} \
         --NormalizationKey None,SBDiff,BBSame,BBDiff
   done
done
