
Output=Paper20240808
mkdir -p $Output

Histogram=HDeltaR,HDeltaRLog,HDeltaREEC,HDeltaREECLog,HDeltaREECFine,HDeltaREECLogFine

for P in 2 3 4
do
   # Nominal PP
   ./Execute --Input Input/PPDataxPPData_Reco${P}to200.root --Coefficients 1 \
      --Histograms $Histogram --Output ${Output}/Data_PP_PT${P}.root

   # Track efficiency
   ./Execute --Input Input/PPDataxPPData_Reco${P}to200.root --Coefficients 1 \
      --Histograms $Histogram --Output ${Output}/Data_PP_PT${P}_TrackEfficiency.root \
      --FinalScale 1.048

   # Extra Z's
   for i in `seq 0 3`
   do
      ./Execute --Input Input/PPDataxPPData_ExtraZ${i}_Reco${P}to200.root --Coefficients 1 \
         --Histograms $Histogram --Output ${Output}/Data_PP_PT${P}_ExtraZ${i}.root
   done

   # PU variant
   ./Execute --Input Input/PPDataxPPData_Reco${P}to200_PU.root --Coefficients 1 \
      --Histograms $Histogram --Output ${Output}/Data_PP_PT${P}_PU.root
done

for P in 4
do
   # Cutoffs
   for U in to20 to100 to200
   do
      ./Execute --Input Input/PPDataxPPData_Reco${P}${U}_Cutoff.root --Coefficients 1 \
         --Histograms $Histogram --Output ${Output}/Data_PP_PT${P}${U}.root
   done
done


