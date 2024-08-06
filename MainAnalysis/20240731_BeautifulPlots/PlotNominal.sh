#!/bin/bash

for P in 2 3 4
do
   ./ExecuteBasic --Input Curves/Data_PP_PT${P}.root,Curves/Data_Centrality030_PT${P}.root \
      --Systematics Systematics/Data_PP_PT${P}.root,Systematics/Data_Centrality030_PT${P}.root \
      --Label PP,"PbPb 0-30%" \
      --Output Result_Centrality030_PT${P}.pdf \
      --Color -1,-2 \
      --ExtraText "p_{T} > $P GeV" \
      --Histogram HDeltaREEC
   
   ./ExecuteBasic --Input Curves/Data_PP_PT${P}.root,Curves/Data_Centrality3050_PT${P}.root \
      --Systematics Systematics/Data_PP_PT${P}.root,Systematics/Data_Centrality3050_PT${P}.root \
      --Label PP,"PbPb 30-50%" \
      --Output Result_Centrality3050_PT${P}.pdf \
      --Color -1,-3 \
      --ExtraText "p_{T} > $P GeV" \
      --Histogram HDeltaREEC
   
   ./ExecuteBasic --Input Curves/Data_PP_PT${P}.root,Curves/Data_Centrality5090_PT${P}.root \
      --Systematics Systematics/Data_PP_PT${P}.root,Systematics/Data_Centrality5090_PT${P}.root \
      --Label PP,"PbPb 50-90%" \
      --Output Result_Centrality5090_PT${P}.pdf \
      --Color -1,-4 \
      --ExtraText "p_{T} > $P GeV" \
      --Histogram HDeltaREEC
   
   ./ExecuteBasic --Input Curves/Data_PP_PT${P}.root,Curves/Data_Centrality030_PT${P}.root \
      --Systematics Systematics/Data_PP_PT${P}.root,Systematics/Data_Centrality030_PT${P}.root \
      --Label PP,"PbPb 0-30%" \
      --Output ResultLog_Centrality030_PT${P}.pdf \
      --Color -1,-2 --DoLogX true \
      --ExtraText "p_{T} > $P GeV" \
      --Histogram HDeltaREECLog
   
   ./ExecuteBasic --Input Curves/Data_PP_PT${P}.root,Curves/Data_Centrality3050_PT${P}.root \
      --Systematics Systematics/Data_PP_PT${P}.root,Systematics/Data_Centrality3050_PT${P}.root \
      --Label PP,"PbPb 30-50%" \
      --Output ResultLog_Centrality3050_PT${P}.pdf \
      --Color -1,-3 --DoLogX true \
      --ExtraText "p_{T} > $P GeV" \
      --Histogram HDeltaREECLog
   
   ./ExecuteBasic --Input Curves/Data_PP_PT${P}.root,Curves/Data_Centrality5090_PT${P}.root \
      --Systematics Systematics/Data_PP_PT${P}.root,Systematics/Data_Centrality5090_PT${P}.root \
      --Label PP,"PbPb 50-90%" \
      --Output ResultLog_Centrality5090_PT${P}.pdf \
      --Color -1,-4 --DoLogX true \
      --ExtraText "p_{T} > $P GeV" \
      --Histogram HDeltaREECLog
done
