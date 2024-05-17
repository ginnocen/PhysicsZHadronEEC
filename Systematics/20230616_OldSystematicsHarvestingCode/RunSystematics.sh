#!/bin/bash

mkdir -p Sys

# Muon matching
./Execute --Variation Root/DataMuMatch.root --Reference Root/Data.root \
   --Output Sys/PbPbMuMatch.root \
   --Scheme Maximum
./Execute --Variation Root/PPDataMuMatch.root --Reference Root/PPData.root \
   --Output Sys/PPMuMatch.root \
   --Scheme Maximum

# Background subtraction
./Execute --Variation Root/DataMixUEUp25.root --Reference Root/DataMix.root \
   --Output Sys/PbPbUEUp25.root \
   --Scheme RMS
./Execute --Variation Root/DataMixUEDown25.root --Reference Root/DataMix.root \
   --Output Sys/PbPbUEDown25.root \
   --Scheme RMS
./Execute --Variation Sys/PbPbUEUp25.root --Reference Sys/PbPbUEDown25.root \
   --Output Sys/PbPbUE25.root \
   --DirectMode true --Scheme DirectMaximum

# Non-closure

# pp PU
./Execute --Variation Root/PPData.root --Reference Root/PPData.root \
   --Output Sys/PPPU.root \
   --Scheme Maximum --DoSubstitute true --SubstituteFrom PV_0_10 --SubstituteTo PV_0_1

# Lepton SF variation
./Execute --Variation Root/DataExtraZWeight0.root --Reference Root/Data.root \
   --Output Sys/PbPbSF0.root \
   --Scheme Maximum
./Execute --Variation Root/DataExtraZWeight1.root --Reference Root/Data.root \
   --Output Sys/PbPbSF1.root \
   --Scheme Maximum
./Execute --Variation Root/DataExtraZWeight2.root --Reference Root/Data.root \
   --Output Sys/PbPbSF2.root \
   --Scheme Maximum
./Execute --Variation Root/DataExtraZWeight3.root --Reference Root/Data.root \
   --Output Sys/PbPbSF3.root \
   --Scheme Maximum
./Execute --Variation Root/DataExtraZWeight4.root --Reference Root/Data.root \
   --Output Sys/PbPbSF4.root \
   --Scheme Maximum
./Execute --Variation Root/DataExtraZWeight5.root --Reference Root/Data.root \
   --Output Sys/PbPbSF5.root \
   --Scheme Maximum
./Execute --Variation Root/DataExtraZWeight6.root --Reference Root/Data.root \
   --Output Sys/PbPbSF6.root \
   --Scheme Maximum
./Execute --Variation Root/DataExtraZWeight7.root --Reference Root/Data.root \
   --Output Sys/PbPbSF7.root \
   --Scheme Maximum
./Execute --Variation Root/DataExtraZWeight8.root --Reference Root/Data.root \
   --Output Sys/PbPbSF8.root \
   --Scheme Maximum
./Execute --Variation Root/DataExtraZWeight9.root --Reference Root/Data.root \
   --Output Sys/PbPbSF9.root \
   --Scheme Maximum
./Execute --Variation Root/DataExtraZWeight10.root --Reference Root/Data.root \
   --Output Sys/PbPbSF10.root \
   --Scheme Maximum
./Execute --Variation Root/DataExtraZWeight11.root --Reference Root/Data.root \
   --Output Sys/PbPbSF11.root \
   --Scheme Maximum
./Execute --Variation Sys/PbPbSF0.root --Reference Sys/PbPbSF1.root \
   --Output Sys/PbPbSFSys1.root \
   --DirectMode true --Scheme DirectMaximum
./Execute --Variation Sys/PbPbSF2.root --Reference Sys/PbPbSF3.root \
   --Output Sys/PbPbSFSys2.root \
   --DirectMode true --Scheme DirectMaximum
./Execute --Variation Sys/PbPbSF4.root --Reference Sys/PbPbSF5.root \
   --Output Sys/PbPbSFSys3.root \
   --DirectMode true --Scheme DirectMaximum
./Execute --Variation Sys/PbPbSF6.root --Reference Sys/PbPbSF7.root \
   --Output Sys/PbPbSFStat1.root \
   --DirectMode true --Scheme DirectMaximum
./Execute --Variation Sys/PbPbSF8.root --Reference Sys/PbPbSF9.root \
   --Output Sys/PbPbSFStat2.root \
   --DirectMode true --Scheme DirectMaximum
./Execute --Variation Sys/PbPbSF10.root --Reference Sys/PbPbSF11.root \
   --Output Sys/PbPbSFStat3.root \
   --DirectMode true --Scheme DirectMaximum
./Execute --Variation Sys/PbPbSFSys1.root --Reference Sys/PbPbSFStat1.root \
   --Output Sys/PbPbSFCombined1.root \
   --DirectMode true --Scheme DirectQuadraticSum
./Execute --Variation Sys/PbPbSFSys2.root --Reference Sys/PbPbSFStat2.root \
   --Output Sys/PbPbSFCombined2.root \
   --DirectMode true --Scheme DirectQuadraticSum
./Execute --Variation Sys/PbPbSFSys3.root --Reference Sys/PbPbSFStat3.root \
   --Output Sys/PbPbSFCombined3.root \
   --DirectMode true --Scheme DirectQuadraticSum
./Execute --Variation Sys/PbPbSFCombined1.root --Reference Sys/PbPbSFCombined2.root \
   --Output Sys/PbPbSFCombined12.root \
   --DirectMode true --Scheme DirectQuadraticSum
./Execute --Variation Sys/PbPbSFCombined12.root --Reference Sys/PbPbSFCombined3.root \
   --Output Sys/PbPbSF.root \
   --DirectMode true --Scheme DirectQuadraticSum

./Execute --Variation Root/PPDataExtraZWeight0.root --Reference Root/PPData.root \
   --Output Sys/PPSF0.root \
   --Scheme Maximum
./Execute --Variation Root/PPDataExtraZWeight1.root --Reference Root/PPData.root \
   --Output Sys/PPSF1.root \
   --Scheme Maximum
./Execute --Variation Root/PPDataExtraZWeight2.root --Reference Root/PPData.root \
   --Output Sys/PPSF2.root \
   --Scheme Maximum
./Execute --Variation Root/PPDataExtraZWeight3.root --Reference Root/PPData.root \
   --Output Sys/PPSF3.root \
   --Scheme Maximum
./Execute --Variation Sys/PPSF0.root --Reference Sys/PPSF1.root \
   --Output Sys/PPSF12.root \
   --DirectMode true --Scheme DirectMaximum
./Execute --Variation Sys/PPSF2.root --Reference Sys/PPSF3.root \
   --Output Sys/PPSF34.root \
   --DirectMode true --Scheme DirectMaximum
./Execute --Variation Sys/PPSF12.root --Reference Sys/PPSF34.root \
   --Output Sys/PPSF.root \
   --DirectMode true --Scheme DirectQuadraticSum

# Centrality definition
./Execute --Variation Root/DataHiBinUp.root --Reference Root/Data.root \
   --Output Sys/PbPbHiBinUp.root \
   --Scheme RMS
./Execute --Variation Root/DataHiBinDown.root --Reference Root/Data.root \
   --Output Sys/PbPbHiBinDown.root \
   --Scheme RMS
./Execute --Variation Sys/PbPbHiBinUp.root --Reference Sys/PbPbHiBinDown.root \
   --Output Sys/PbPbHiBin.root \
   --DirectMode true --Scheme DirectMaximum

# Tracking
./Execute --Variation Root/MCLoose.root --Reference Root/MC.root \
   --Output Sys/PbPbTrack.root \
   --Scheme RMS

# VZ
./Execute --Variation Root/DataVZ1.root --Reference Root/Data.root \
   --Output Sys/PbPbVZ1.root \
   --Scheme RMS --Inflation 0.35
./Execute --Variation Root/DataVZ2.root --Reference Root/Data.root \
   --Output Sys/PbPbVZ2.root \
   --Scheme RMS --Inflation 0.35
./Execute --Variation Root/DataVZ3.root --Reference Root/Data.root \
   --Output Sys/PbPbVZ3.root \
   --Scheme RMS --Inflation 0.35
./Execute --Variation Root/DataVZ4.root --Reference Root/Data.root \
   --Output Sys/PbPbVZ4.root \
   --Scheme RMS --Inflation 0.35
./Execute --Variation Root/PPDataVZ1.root --Reference Root/PPData.root \
   --Output Sys/PPVZ1.root \
   --Scheme RMS
./Execute --Variation Root/PPDataVZ2.root --Reference Root/PPData.root \
   --Output Sys/PPVZ2.root \
   --Scheme RMS
./Execute --Variation Root/PPDataVZ3.root --Reference Root/PPData.root \
   --Output Sys/PPVZ3.root \
   --Scheme RMS
./Execute --Variation Root/PPDataVZ4.root --Reference Root/PPData.root \
   --Output Sys/PPVZ4.root \
   --Scheme RMS

# Now we combine everything
./ExecuteCombine --File Sys/PbPbMuMatch.root,Sys/PbPbUE25.root,Sys/PbPbSFCombined1.root,Sys/PbPbSFCombined2.root,Sys/PbPbSFCombined3.root,Sys/PbPbHiBin.root,Sys/PbPbVZ2.root,Sys/PbPbTrack.root --Output Sys/PbPbAll.root
./ExecuteCombine --File Sys/PPMuMatch.root,Sys/PPPU.root,Sys/PPVZ2.root,Sys/PPSF.root --Output Sys/PPAll.root

