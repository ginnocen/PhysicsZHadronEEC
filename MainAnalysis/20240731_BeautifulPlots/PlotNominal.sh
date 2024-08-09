#!/bin/bash

for P in 2 3 4
do
   ./ExecuteBasic --Input Curves/Data_PP_PT${P}.root,Curves/Data_Centrality030_PT${P}.root \
      --Systematics Systematics/Data_PP_PT${P}.root,Systematics/Data_Centrality030_PT${P}.root \
      --Label PP,"PbPb 0-30%" \
      --Output Result_Centrality030_PT${P}.pdf \
      --Color -1,-2 \
      --ExtraText "p_{T} = $P-200 GeV" \
      --Histogram HDeltaREEC
   
   ./ExecuteBasic --Input Curves/Data_PP_PT${P}.root,Curves/Data_Centrality3050_PT${P}.root \
      --Systematics Systematics/Data_PP_PT${P}.root,Systematics/Data_Centrality3050_PT${P}.root \
      --Label PP,"PbPb 30-50%" \
      --Output Result_Centrality3050_PT${P}.pdf \
      --Color -1,-3 \
      --ExtraText "p_{T} = $P-200 GeV" \
      --Histogram HDeltaREEC
   
   ./ExecuteBasic --Input Curves/Data_PP_PT${P}.root,Curves/Data_Centrality5090_PT${P}.root \
      --Systematics Systematics/Data_PP_PT${P}.root,Systematics/Data_Centrality5090_PT${P}.root \
      --Label PP,"PbPb 50-90%" \
      --Output Result_Centrality5090_PT${P}.pdf \
      --Color -1,-4 \
      --ExtraText "p_{T} = $P-200 GeV" \
      --Histogram HDeltaREEC
   
   ./ExecuteBasic --Input Curves/Data_PP_PT${P}.root,Curves/Data_Centrality030_PT${P}.root \
      --Systematics Systematics/Data_PP_PT${P}.root,Systematics/Data_Centrality030_PT${P}.root \
      --Label PP,"PbPb 0-30%" \
      --Output ResultLog_Centrality030_PT${P}.pdf \
      --Color -1,-2 --DoLogX true \
      --ExtraText "p_{T} = $P-200 GeV" \
      --Histogram HDeltaREECLog
   
   ./ExecuteBasic --Input Curves/Data_PP_PT${P}.root,Curves/Data_Centrality3050_PT${P}.root \
      --Systematics Systematics/Data_PP_PT${P}.root,Systematics/Data_Centrality3050_PT${P}.root \
      --Label PP,"PbPb 30-50%" \
      --Output ResultLog_Centrality3050_PT${P}.pdf \
      --Color -1,-3 --DoLogX true \
      --ExtraText "p_{T} = $P-200 GeV" \
      --Histogram HDeltaREECLog
   
   ./ExecuteBasic --Input Curves/Data_PP_PT${P}.root,Curves/Data_Centrality5090_PT${P}.root \
      --Systematics Systematics/Data_PP_PT${P}.root,Systematics/Data_Centrality5090_PT${P}.root \
      --Label PP,"PbPb 50-90%" \
      --Output ResultLog_Centrality5090_PT${P}.pdf \
      --Color -1,-4 --DoLogX true \
      --ExtraText "p_{T} = $P-200 GeV" \
      --Histogram HDeltaREECLog
done

for P in 4
do
   for U in to200 to20 to100
   do
      ./ExecuteBasic --Input Curves/Data_PP_PT${P}${U}.root,Curves/Data_Centrality5090_PT${P}${U}.root \
         --Systematics Systematics/Data_PP_PT${P}.root,Systematics/Data_Centrality5090_PT${P}.root \
         --Label PP,"PbPb 50-90%" \
         --Output Result_Centrality5090_PT${P}${U}.pdf \
         --Color -1,-4 \
         --ExtraText "p_{T}: 4${U} GeV" \
         --Histogram HDeltaREEC

      ./ExecuteBasic --Input Curves/Data_PP_PT${P}${U}.root,Curves/Data_Centrality5090_PT${P}${U}.root \
         --Systematics Systematics/Data_PP_PT${P}.root,Systematics/Data_Centrality5090_PT${P}.root \
         --Label PP,"PbPb 50-90%" \
         --Output ResultLog_Centrality5090_PT${P}${U}.pdf \
         --Color -1,-4 --DoLogX true \
         --ExtraText "p_{T}: 4${U} GeV" \
         --Histogram HDeltaREECLog
   done
done
