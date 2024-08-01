#!/bin/bash

Output=Paper20240731
mkdir -p $Output

for P in 2 3 4
do
   for C in 030 3050 5090
   do

      N=Input/Data_Centrality${C}_PT${P}.root
      H=HDeltaREEC,HDeltaREECLog

      # Initialize and in corrected & overcorrected
      Base=${N},${N}
      Variant=Input/Data_Centrality${C}_PT${P}_Corrected.root,Input/Data_Centrality${C}_PT${P}_OverCorrected.root
      Label=Corrected,OverCorrected
      Group=0,0

      # Add in extra Z variants
      for i in `seq 0 11`
      do
         Base=${Base},$N
         Variant=${Variant},Input/Data_Centrality${C}_PT${P}_ExtraZ${i}.root
         Label=${Label},ExtraZ${i}
         Group=${Group},1
      done

      # Add in track variant
      Base=${Base},${N}
      Variant=${Variant},Input/Data_Centrality${C}_PT${P}_TrackEfficiency.root
      Label=${Label},Track
      Group=${Group},0

      # Add in hiBin variants
      Base=${Base},$N,$N
      Variant=${Variant},Input/Data_Centrality${C}HiBinDown_PT${P}.root,Input/Data_Centrality${C}HiBinUp_PT${P}.root
      Label=${Label},HiBinDown,HiBinUp
      Group=${Group},2,2

      # Add in nonclosure: TODO
      Base=${Base},$N
      Variant=${Variant},$N
      Label=${Label},NonClosure
      Group=${Group},0

      # Add in lepton rejection: TODO
      Base=${Base},$N
      Variant=${Variant},$N
      Label=${Label},Lepton
      Group=${Group},0

      # Finally combine things
      ./Execute --Nominal $N \
         --Base $Base --Variant $Variant --Label $Label --Group $Group --GroupLabel ExtraZ,HiBin \
         --Histogram $H --Output $Output/Data_Centrality${C}_PT${P}.root
   done
done



