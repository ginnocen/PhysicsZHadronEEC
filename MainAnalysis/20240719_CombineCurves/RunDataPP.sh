
Output=Paper20240731
mkdir -p $Output

for P in 2 3 4
do
   # Nominal PP
   ./Execute --Input Input/PPDataxPPData_Reco${P}Inf.root --Coefficients 1 \
      --Histograms HDeltaREEC,HDeltaREECLog --Output ${Output}/Data_PP_PT${P}.root

   # Track efficiency
   ./Execute --Input Input/PPDataxPPData_Reco${P}Inf.root --Coefficients 1 \
      --Histograms HDeltaREEC,HDeltaREECLog --Output ${Output}/Data_PP_PT${P}_TrackEfficiency.root \
      --FinalScale 1.048

   # Extra Z's
   for i in `seq 0 3`
   do
      ./Execute --Input Input/PPDataxPPData_ExtraZ${i}_Reco${P}Inf.root --Coefficients 1 \
         --Histograms HDeltaREEC,HDeltaREECLog --Output ${Output}/Data_PP_PT${P}_ExtraZ${i}.root
   done

   # PU variant
   ./Execute --Input Input/PPDataxPPData_Reco2Inf_PU.root --Coefficients 1 \
      --Histograms HDeltaREEC,HDeltaREECLog --Output ${Output}/Data_PP_PT${P}_PU.root
done


