#!/bin/bash

rm -rf Output/
mkdir -p Output/

#################
### Data PbPb ###
#################

./Execute --Input Samples/HiForestMiniAOD_2_Data.root \
   --Output Output/output_2_Data.root \
   --Year 2018 \
   --TrackEfficiencyPath ${ProjectBase}/CommonCode/root/ \
   --DoGenLevel false \
   --IsData true \
   --IsPP false \
   --IsBackground false \
   --CheckZ true

#################
#### Data pp ####
#################

./Execute --Input Samples/HiForestAOD_22_Data_pp.root \
   --Output Output/output_22_Data_pp.root \
   --Year 2018 \
   --TrackEfficiencyPath ${ProjectBase}/CommonCode/root/ \
   --DoGenLevel false \
   --IsData true \
   --IsPP true \
   --IsBackground false \
   --CheckZ true

###################
### MC Gen PbPb ###
###################

./Execute --Input Samples/HiForestMiniAOD_30_MC.root \
   --Output Output/output_30_MC.root \
   --Year 2018 \
   --TrackEfficiencyPath ${ProjectBase}/CommonCode/root/ \
   --DoGenLevel false \
   --IsData false \
   --IsPP false \
   --IsBackground false \
   --CheckZ true


