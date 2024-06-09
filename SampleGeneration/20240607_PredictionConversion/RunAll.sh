#!/bin/bash

for i in `seq 0 100`; do
   ./ExecutePU14 --Input Input/pyquen/output/PbPbWide_0_30ZJet_${i}.pu14 \
      --Output Output/pyquen_PbPbWide030_ZJet_${i}.root
done

for i in `seq 0 100`; do
   ./ExecutePU14 --Input Input/pyquen/output/PbPb_0_30ZJet_${i}.pu14 \
      --Output Output/pyquen_PbPb030_ZJet_${i}.root
done

for i in `seq 1 100`; do
   ./ExecutePU14 --Input Input/pyquen/ppZJet0/pp_${i}.pu14 \
      --Output Output/pyquen_pp_ZJet_${i}.root
done

for i in `seq 1 100`; do
   ./ExecuteHepMC --Input Input/jewel/jewel-2.2.0/eventfilesZJet/out_ZJet_pp_${i}.hepmc \
      --Output Output/jewel_pp_ZJet_${i}.root --NegativeID 3
done

for i in `seq 0 100`; do
   ./ExecuteHepMC --Input Input/jewel/jewel-2.2.0/eventfilesZJetPbPb/0-30/out_ZJet_PbPb_${i}.hepmc \
      --Output Output/jewel_PbPb030_ZJet_${i}.root --NegativeID 3
   ./ExecuteHepMC --Input Input/jewel/jewel-2.2.0/eventfilesZJetPbPb/30-50/out_ZJet_PbPb_${i}.hepmc \
      --Output Output/jewel_PbPb3050_ZJet_${i}.root --NegativeID 3
   ./ExecuteHepMC --Input Input/jewel/jewel-2.2.0/eventfilesZJetPbPb/50-70/out_ZJet_PbPb_${i}.hepmc \
      --Output Output/jewel_PbPb5070_ZJet_${i}.root --NegativeID 3
   ./ExecuteHepMC --Input Input/jewel/jewel-2.2.0/eventfilesZJetPbPb/70-90/out_ZJet_PbPb_${i}.hepmc \
      --Output Output/jewel_PbPb7090_ZJet_${i}.root --NegativeID 3
done

# for i in `seq 1 99`; do
#    ./ExecuteHepMC --Input Input/jewel/jewel-2.2.0/eventfilesPbPbNoRecoil/0-10/out_PbPb_${i}.hepmc \
#       --Output Output/jewel_PbPbNoRecoil010_ZJet_${i}.root --NegativeID 3
#    ./ExecuteHepMC --Input Input/jewel/jewel-2.2.0/eventfilesPbPbNoRecoil/10-30/out_PbPb_${i}.hepmc \
#       --Output Output/jewel_PbPbNoRecoil1030_ZJet_${i}.root --NegativeID 3
#    ./ExecuteHepMC --Input Input/jewel/jewel-2.2.0/eventfilesPbPbNoRecoil/30-50/out_PbPb_${i}.hepmc \
#       --Output Output/jewel_PbPbNoRecoil3050_ZJet_${i}.root --NegativeID 3
#    ./ExecuteHepMC --Input Input/jewel/jewel-2.2.0/eventfilesPbPbNoRecoil/50-90/out_PbPb_${i}.hepmc \
#       --Output Output/jewel_PbPbNoRecoil5090_ZJet_${i}.root --NegativeID 3
# done

for i in `seq 0 100`; do
   ./ExecuteHepMC --Input Input/jewel/jewel-2.2.0/eventfilesZJetPbPbNoRecoil/0-30/out_ZJet_PbPb_${i}.hepmc \
      --Output Output/jewel_PbPbNoRecoil030_ZJet_${i}.root --NegativeID 3
   ./ExecuteHepMC --Input Input/jewel/jewel-2.2.0/eventfilesZJetPbPbNoRecoil/30-50/out_ZJet_PbPb_${i}.hepmc \
      --Output Output/jewel_PbPbNoRecoil3050_ZJet_${i}.root --NegativeID 3
   ./ExecuteHepMC --Input Input/jewel/jewel-2.2.0/eventfilesZJetPbPbNoRecoil/50-90/out_ZJet_PbPb_${i}.hepmc \
      --Output Output/jewel_PbPbNoRecoil5090_ZJet_${i}.root --NegativeID 3
done

./ExecuteHybrid --Input Input/sample/dijet80_noq.root --Output Output/hybrid_Dijet80_NoQ.root
./ExecuteHybrid --Input Input/sample/dijet80_q.root   --Output Output/hybrid_Dijet80_Q.root
./ExecuteHybrid --Input Input/sample/zjet10_noq.root  --Output Output/hybrid_ZJet10_NoQ.root
./ExecuteHybrid --Input Input/sample/zjet10_q.root    --Output Output/hybrid_ZJet10_Q.root
