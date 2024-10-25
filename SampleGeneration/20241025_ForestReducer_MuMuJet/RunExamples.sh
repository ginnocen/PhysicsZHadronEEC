#!/bin/bash

mkdir -p Output/

############
### Data ###
############

./Execute --Input Samples/HiForestMiniAOD_2.root \
   --Output Output/output.root \
   --Year 2018 \
   --TrackEfficiencyPath ${ProjectBase}/CommonCode/root/ \
   --DoGenLevel false \
   --IsData true \
   --IsPP false \
   --IsBackground false \
   --CheckZ true

