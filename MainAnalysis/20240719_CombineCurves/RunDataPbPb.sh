
# Nominal 0-30%
./Execute --Input Input/PbPbDataxPbPbData_Centrality030_Reco2inf.root,Input/PbPbDataxMinBiasData_Centrality030_Reco2inf.root,Input/MinBiasDataxMinBiasData_Centrality030_Reco2inf.root,Input/MinBiasDataxMinBiasData_Centrality030_Reco2inf_Different.root --Coefficients 1,-2,-1,2 --Histograms HDeltaR,HDeltaRLog,HDeltaREEC,HDeltaREECLog --Output Data_Centrality030.root --NormalizationState Data_Centrality030 --NormalizationKey None,SBDiff,BBSame,BBDiff

# Corrected HF with the 60 & 120
./Execute --Input Input/PbPbDataxPbPbData_Centrality030_Reco2inf.root,Input/PbPbDataxMinBiasData_Centrality030Corrected_Reco2inf.root,Input/MinBiasDataxMinBiasData_Centrality030Corrected_Reco2inf.root,Input/MinBiasDataxMinBiasData_Centrality030Corrected_Reco2inf_Different.root --Coefficients 1,-2,-1,2 --Histograms HDeltaR,HDeltaRLog,HDeltaREEC,HDeltaREECLog --Output Data_Centrality030_Corrected.root --NormalizationState Data_Centrality030_Corrected --NormalizationKey None,SBDiff,BBSame,BBDiff
./Execute --Input Input/PbPbDataxPbPbData_Centrality030_Reco2inf.root,Input/PbPbDataxMinBiasData_Centrality030OverCorrected_Reco2inf.root,Input/MinBiasDataxMinBiasData_Centrality030OverCorrected_Reco2inf.root,Input/MinBiasDataxMinBiasData_Centrality030OverCorrected_Reco2inf_Different.root --Coefficients 1,-2,-1,2 --Histograms HDeltaR,HDeltaRLog,HDeltaREEC,HDeltaREECLog --Output Data_Centrality030_OverCorrected.root --NormalizationState Data_Centrality030_OverCorrected --NormalizationKey None,SBDiff,BBSame,BBDiff

# Extra Z's
for i in `seq 0 11`
do
   ./Execute --Input Input/PbPbDataxPbPbData_Centrality030_ExtraZ${i}_Reco2inf.root,Input/PbPbDataxMinBiasData_Centrality030_ExtraZ${i}_Reco2inf.root,Input/MinBiasDataxMinBiasData_Centrality030_Reco2inf.root,Input/MinBiasDataxMinBiasData_Centrality030_Reco2inf_Different.root --Coefficients 2,-2,-1,2 --Histograms HDeltaR,HDeltaRLog,HDeltaREEC,HDeltaREECLog --Output Data_Centrality030_ExtraZ${i}.root --NormalizationState Data_Centrality030_ExtraZ${i} --NormalizationKey None,SBDiff,BBSame,BBDiff
done


