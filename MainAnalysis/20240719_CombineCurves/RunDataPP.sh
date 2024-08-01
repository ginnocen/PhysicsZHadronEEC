
# Nominal PP
./Execute --Input Input/PPDataxPPData_Reco2inf.root --Coefficients 1 --Histograms HDeltaREEC,HDeltaREECLog --Output Data_PP.root

# Extra Z's
for i in `seq 0 3`
do
   ./Execute --Input Input/PPDataxPPData_ExtraZ${i}_Reco2inf.root --Coefficients 1 --Histograms HDeltaREEC,HDeltaREECLog --Output Data_PP_ExtraZ${i}.root
done


