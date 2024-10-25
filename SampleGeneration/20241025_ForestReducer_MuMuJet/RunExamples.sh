#!/bin/bash

mkdir -p Output/

############
### Data ###
############

./Execute --Input Samples/HiForestMiniAOD_2_Data.root \
   --Output Output/output.root \
   --Year 2018 \
   --TrackEfficiencyPath ${ProjectBase}/CommonCode/root/ \
   --DoGenLevel false \
   --IsData true \
   --IsPP false \
   --IsBackground false \
   --CheckZ true


############
### MC Gen ###
############

./Execute --Input Samples/HiForestMiniAOD_30_MC.root \
   --Output Output/output.root \
   --Year 2018 \
   --TrackEfficiencyPath ${ProjectBase}/CommonCode/root/ \
   --DoGenLevel true \
   --IsData false \
   --IsPP false \
   --IsBackground false \
   --CheckZ true


