rm Execute
rm -rf ../../CommonCode/binary/
rm -rf ../../CommonCode/library/
rm MergedOutput.root
rm -rf Output
rm SkimReco.root
rm list.txt
rm -rf output
rm *.txt*
rm SkimReco.root
rm .DS_Store

cd /home/ginnocen/CMSSW_13_2_6_patch2/src
cmsenv

cd -
echo "CMSSW environment is set up"
cd ../../
source SetupAnalysis.sh
cd CommonCode/
make
cd ..
cd SampleGeneration/20241025_ForestReducer_MuMuJet
make
rm SkimReco.root

