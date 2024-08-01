
N=Input/Data_Centrality030.root

H=HDeltaREEC,HDeltaREECLog

Base=${N},${N}
Variant=Input/Data_Centrality030_Corrected.root,Input/Data_Centrality030_OverCorrected.root
Label=Corrected,OverCorrected
Group=0,0
for i in `seq 0 11`
do
   Base=${Base},$N
   Variant=${Variant},Input/Data_Centrality030_ExtraZ${i}.root
   Label=${Label},ExtraZ${i}
   Group=${Group},1
done

./Execute --Nominal $N \
   --Base $Base --Variant $Variant --Label $Label --Group $Group --GroupLabel ExtraZ \
   --Histogram $H --Output Data_Centrality030.root
