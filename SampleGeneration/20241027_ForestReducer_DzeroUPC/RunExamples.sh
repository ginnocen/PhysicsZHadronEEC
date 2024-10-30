#!/bin/bash

rm -rf Output/
mkdir -p Output/

#################
### Data PbPb ###
#################

#./Execute --Input /eos/cms/store/group/phys_heavyions/jdlang/PbPbUPC2023/2023config_2023RerecoHIForward0/HIForward0/crab_2023config_2023RerecoHIForward0/241024_101446/0001/HiForestMiniAOD_1703.root \
./Execute --Input Samples/HiForestMiniAOD_UPCPbPb23_HiVertex_279.root \
   --Output Output/example.root \
   --Year 2023 \
   --MinDzeroPT 1.0 \
   --IsData true \
   --PFTree particleFlowAnalyser/pftree \

###################
### MC Gen PbPb ###
###################

#./Execute --Input Samples/HiForestMiniAOD_30_MC.root \
#   --Output Output/output_30_MC.root \
#   --Year 2023 \
#   --MinDzeroPT 1.0 \
#   --IsData false \
#   --PFTree particleFlowAnalyser/pftree \


