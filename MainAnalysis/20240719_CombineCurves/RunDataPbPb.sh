#!/bin/basn

Output=Paper20240731
mkdir -p "$Output"

for C in 030 3050 5090
do
   for P in 2 3 4
   do
      # Nominal
      ./Execute --Input Input/PbPbDataxPbPbData_Centrality${C}_Reco${P}Inf.root,Input/PbPbDataxMinBiasData_Centrality${C}_Reco${P}Inf.root,Input/MinBiasDataxMinBiasData_Centrality${C}_Reco${P}Inf.root,Input/MinBiasDataxMinBiasData_Centrality${C}_Reco${P}Inf_Different.root \
         --Coefficients 1,-2,-1,2 --Histograms HDeltaR,HDeltaRLog,HDeltaREEC,HDeltaREECLog \
         --Output ${Output}/Data_Centrality${C}_PT${P}.root \
         --NormalizationState Data_Centrality${C}_PT${P} \
         --NormalizationKey None,SBDiff,BBSame,BBDiff

      # Track efficiency
      ./Execute --Input Input/PbPbDataxPbPbData_Centrality${C}_Reco${P}Inf.root,Input/PbPbDataxMinBiasData_Centrality${C}_Reco${P}Inf.root,Input/MinBiasDataxMinBiasData_Centrality${C}_Reco${P}Inf.root,Input/MinBiasDataxMinBiasData_Centrality${C}_Reco${P}Inf_Different.root \
         --Coefficients 1,-2,-1,2 --Histograms HDeltaR,HDeltaRLog,HDeltaREEC,HDeltaREECLog \
         --Output ${Output}/Data_Centrality${C}_PT${P}_TrackEfficiency.root \
         --NormalizationState Data_Centrality${C}_PT${P} \
         --NormalizationKey None,SBDiff,BBSame,BBDiff \
         --FinalScale 1.10

      # Corrected HF with the 60 & 120
      ./Execute --Input Input/PbPbDataxPbPbData_Centrality${C}_Reco${P}Inf.root,Input/PbPbDataxMinBiasData_Centrality${C}Corrected_Reco${P}Inf.root,Input/MinBiasDataxMinBiasData_Centrality${C}Corrected_Reco${P}Inf.root,Input/MinBiasDataxMinBiasData_Centrality${C}Corrected_Reco${P}Inf_Different.root \
         --Coefficients 1,-2,-1,2 --Histograms HDeltaR,HDeltaRLog,HDeltaREEC,HDeltaREECLog \
         --Output ${Output}/Data_Centrality${C}_PT${P}_Corrected.root \
         --NormalizationState Data_Centrality${C}_PT${P}_Corrected \
         --NormalizationKey None,SBDiff,BBSame,BBDiff
      ./Execute --Input Input/PbPbDataxPbPbData_Centrality${C}_Reco${P}Inf.root,Input/PbPbDataxMinBiasData_Centrality${C}OverCorrected_Reco${P}Inf.root,Input/MinBiasDataxMinBiasData_Centrality${C}OverCorrected_Reco${P}Inf.root,Input/MinBiasDataxMinBiasData_Centrality${C}OverCorrected_Reco${P}Inf_Different.root \
         --Coefficients 1,-2,-1,2 --Histograms HDeltaR,HDeltaRLog,HDeltaREEC,HDeltaREECLog \
         --Output ${Output}/Data_Centrality${C}_PT${P}_OverCorrected.root \
         --NormalizationState Data_Centrality${C}_PT${P}_OverCorrected \
         --NormalizationKey None,SBDiff,BBSame,BBDiff

      # Extra Z's
      for Z in `seq 0 11`
      do
         ./Execute --Input Input/PbPbDataxPbPbData_Centrality${C}_ExtraZ${Z}_Reco${P}Inf.root,Input/PbPbDataxMinBiasData_Centrality${C}_ExtraZ${Z}_Reco${P}Inf.root,Input/MinBiasDataxMinBiasData_Centrality${C}_Reco${P}Inf.root,Input/MinBiasDataxMinBiasData_Centrality${C}_Reco${P}Inf_Different.root \
            --Coefficients 1,-2,-1,2 \
            --Histograms HDeltaR,HDeltaRLog,HDeltaREEC,HDeltaREECLog \
            --Output ${Output}/Data_Centrality${C}_PT${P}_ExtraZ${Z}.root \
            --NormalizationState Data_Centrality${C}_PT${P}_ExtraZ${Z} \
            --NormalizationKey None,SBDiff,BBSame,BBDiff
      done

      # HiBin shift
      for H in Up Down
      do
         ./Execute --Input Input/PbPbDataxPbPbData_Centrality${C}HiBin${H}_Reco${P}Inf.root,Input/PbPbDataxMinBiasData_Centrality${C}HiBin${H}_Reco${P}Inf.root,Input/MinBiasDataxMinBiasData_Centrality${C}HiBin${H}_Reco${P}Inf.root,Input/MinBiasDataxMinBiasData_Centrality${C}HiBin${H}_Reco${P}Inf_Different.root \
            --Coefficients 1,-2,-1,2 --Histograms HDeltaR,HDeltaRLog,HDeltaREEC,HDeltaREECLog \
            --Output ${Output}/Data_Centrality${C}HiBin${H}_PT${P}.root \
            --NormalizationState Data_Centrality${C}HiBin${H}_PT${P} \
            --NormalizationKey None,SBDiff,BBSame,BBDiff
      done
   done
done

