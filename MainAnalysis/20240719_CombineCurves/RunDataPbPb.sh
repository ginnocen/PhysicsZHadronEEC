#!/bin/basn

Output=Paper20240808
mkdir -p "$Output"

Histogram=HDeltaR,HDeltaRLog,HDeltaREEC,HDeltaREECLog,HDeltaREECFine,HDeltaREECLogFine

for C in 030 3050 5090
do
   for P in 2 3 4
   do
      # Nominal
      ./Execute --Input Input/PbPbDataxPbPbData_Centrality${C}_Reco${P}to200.root,Input/PbPbDataxMinBiasData_Centrality${C}_Reco${P}to200.root,Input/MinBiasDataxMinBiasData_Centrality${C}_Reco${P}to200.root,Input/MinBiasDataxMinBiasData_Centrality${C}_Reco${P}to200_Different.root \
         --Coefficients 1,-1,-1,1 --Histograms $Histogram \
         --Output ${Output}/Data_Centrality${C}_PT${P}.root \
         --NormalizationState Data_Centrality${C}_PT${P} \
         --NormalizationKey None,SBDiff,BBSame,BBDiff

      # Deconstructed
      ./Execute --Input Input/PbPbDataxPbPbData_Centrality${C}_Reco${P}to200.root \
         --Coefficients 1 --Histograms $Histogram \
         --Output ${Output}/Data_SBxSB_Centrality${C}_PT${P}.root \
         --NormalizationState Data_Centrality${C}_PT${P} \
         --NormalizationKey None
      ./Execute --Input Input/PbPbDataxMinBiasData_Centrality${C}_Reco${P}to200.root \
         --Coefficients 1 --Histograms $Histogram \
         --Output ${Output}/Data_SBxB_Centrality${C}_PT${P}.root \
         --NormalizationState Data_Centrality${C}_PT${P} \
         --NormalizationKey SBDiff
      ./Execute --Input Input/MinBiasDataxMinBiasData_Centrality${C}_Reco${P}to200.root \
         --Coefficients 1 --Histograms $Histogram \
         --Output ${Output}/Data_BxBSame_Centrality${C}_PT${P}.root \
         --NormalizationState Data_Centrality${C}_PT${P} \
         --NormalizationKey BBSame
      ./Execute --Input Input/MinBiasDataxMinBiasData_Centrality${C}_Reco${P}to200_Different.root \
         --Coefficients 1 --Histograms $Histogram \
         --Output ${Output}/Data_BxBDiff_Centrality${C}_PT${P}.root \
         --NormalizationState Data_Centrality${C}_PT${P} \
         --NormalizationKey BBDiff

      # Track efficiency
      ./Execute --Input Input/PbPbDataxPbPbData_Centrality${C}_Reco${P}to200.root,Input/PbPbDataxMinBiasData_Centrality${C}_Reco${P}to200.root,Input/MinBiasDataxMinBiasData_Centrality${C}_Reco${P}to200.root,Input/MinBiasDataxMinBiasData_Centrality${C}_Reco${P}to200_Different.root \
         --Coefficients 1,-1,-1,1 --Histograms $Histogram \
         --Output ${Output}/Data_Centrality${C}_PT${P}_TrackEfficiency.root \
         --NormalizationState Data_Centrality${C}_PT${P} \
         --NormalizationKey None,SBDiff,BBSame,BBDiff \
         --FinalScale 1.10

      # Corrected HF with the 60 & 120
      ./Execute --Input Input/PbPbDataxPbPbData_Centrality${C}_Reco${P}to200.root,Input/PbPbDataxMinBiasData_Centrality${C}Corrected_Reco${P}to200.root,Input/MinBiasDataxMinBiasData_Centrality${C}Corrected_Reco${P}to200.root,Input/MinBiasDataxMinBiasData_Centrality${C}Corrected_Reco${P}to200_Different.root \
         --Coefficients 1,-1,-1,1 --Histograms $Histogram \
         --Output ${Output}/Data_Centrality${C}_PT${P}_Corrected.root \
         --NormalizationState Data_Centrality${C}_PT${P}_Corrected \
         --NormalizationKey None,SBDiff,BBSame,BBDiff
      ./Execute --Input Input/PbPbDataxPbPbData_Centrality${C}_Reco${P}to200.root,Input/PbPbDataxMinBiasData_Centrality${C}OverCorrected_Reco${P}to200.root,Input/MinBiasDataxMinBiasData_Centrality${C}OverCorrected_Reco${P}to200.root,Input/MinBiasDataxMinBiasData_Centrality${C}OverCorrected_Reco${P}to200_Different.root \
         --Coefficients 1,-1,-1,1 --Histograms $Histogram \
         --Output ${Output}/Data_Centrality${C}_PT${P}_OverCorrected.root \
         --NormalizationState Data_Centrality${C}_PT${P}_OverCorrected \
         --NormalizationKey None,SBDiff,BBSame,BBDiff

      # Extra Z's
      for Z in `seq 0 11`
      do
         ./Execute --Input Input/PbPbDataxPbPbData_Centrality${C}_ExtraZ${Z}_Reco${P}to200.root,Input/PbPbDataxMinBiasData_Centrality${C}_ExtraZ${Z}_Reco${P}to200.root,Input/MinBiasDataxMinBiasData_Centrality${C}_Reco${P}to200.root,Input/MinBiasDataxMinBiasData_Centrality${C}_Reco${P}to200_Different.root \
            --Coefficients 1,-1,-1,1 \
            --Histograms $Histogram \
            --Output ${Output}/Data_Centrality${C}_PT${P}_ExtraZ${Z}.root \
            --NormalizationState Data_Centrality${C}_PT${P}_ExtraZ${Z} \
            --NormalizationKey None,SBDiff,BBSame,BBDiff
      done

      # HiBin shift
      for H in Up Down
      do
         ./Execute --Input Input/PbPbDataxPbPbData_Centrality${C}HiBin${H}_Reco${P}to200.root,Input/PbPbDataxMinBiasData_Centrality${C}HiBin${H}_Reco${P}to200.root,Input/MinBiasDataxMinBiasData_Centrality${C}HiBin${H}_Reco${P}to200.root,Input/MinBiasDataxMinBiasData_Centrality${C}HiBin${H}_Reco${P}to200_Different.root \
            --Coefficients 1,-1,-1,1 --Histograms $Histogram \
            --Output ${Output}/Data_Centrality${C}HiBin${H}_PT${P}.root \
            --NormalizationState Data_Centrality${C}HiBin${H}_PT${P} \
            --NormalizationKey None,SBDiff,BBSame,BBDiff
      done
   done
done
      
for C in 5090
do
   for P in 4
   do
      # Cutoff series
      for U in to20 to100 to200
      do
         ./Execute --Input Input/PbPbDataxPbPbData_Centrality${C}_Reco${P}${U}_Cutoff.root,Input/PbPbDataxMinBiasData_Centrality${C}_Reco${P}${U}_Cutoff.root,Input/MinBiasDataxMinBiasData_Centrality${C}_Reco${P}${U}_Cutoff.root,Input/MinBiasDataxMinBiasData_Centrality${C}_Reco${P}${U}_Cutoff_Different.root \
            --Coefficients 1,-1,-1,1 --Histograms $Histogram \
            --Output ${Output}/Data_Centrality${C}_PT${P}${U}.root \
            --NormalizationState Data_Centrality${C}_PT${P} \
            --NormalizationKey None,SBDiff,BBSame,BBDiff
      done
   done
done

