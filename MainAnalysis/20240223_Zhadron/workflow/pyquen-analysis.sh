./finalAnalysis.sh output/$1pyquenPP 1_2   $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11}   --MinZPT 40 --MaxZPT 350 --MinTrackPT 1 --MaxTrackPT 2  --Input mergedSample/pyquen_pp-v7.root --MixFile mergedSample/pyquen_pp-v7.root  --IsPP false --IsGenZ true --nMix 1  --MinHiBin 0 --MaxHiBin 600000  
./finalAnalysis.sh output/$1pyquenPP 2_4   $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11}   --MinZPT 40 --MaxZPT 350 --MinTrackPT 2 --MaxTrackPT 4  --Input mergedSample/pyquen_pp-v7.root --MixFile mergedSample/pyquen_pp-v7.root  --IsPP false --IsGenZ true --nMix 1  --MinHiBin 0 --MaxHiBin 600000  
./finalAnalysis.sh output/$1pyquenPP 4_10  $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11}   --MinZPT 40 --MaxZPT 350 --MinTrackPT 4 --MaxTrackPT 10 --Input mergedSample/pyquen_pp-v7.root --MixFile mergedSample/pyquen_pp-v7.root  --IsPP false --IsGenZ true --nMix 1  --MinHiBin 0 --MaxHiBin 600000  
hadd -f plots/$1pyquenPP-result.root output/$1pyquenPP-1_2-result.root output/$1pyquenPP-2_4-result.root output/$1pyquenPP-4_10-result.root
hadd -f plots/$1pyquenPP-nosub.root output/$1pyquenPP-1_2-nosub.root output/$1pyquenPP-2_4-nosub.root output/$1pyquenPP-4_10-nosub.root

./finalAnalysis.sh output/$1pyquenPbPb030 1_2   $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11}   --MinZPT 40 --MaxZPT 350 --MinTrackPT 1 --MaxTrackPT 2  --Input mergedSample/pyquen_PbPb030-v7.root --MixFile mergedSample/pyquen_PbPb030-v7.root  --IsPbPb030 false --IsGenZ true --nMix 1  --MinHiBin 0 --MaxHiBin 600000  
./finalAnalysis.sh output/$1pyquenPbPb030 2_4   $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11}   --MinZPT 40 --MaxZPT 350 --MinTrackPT 2 --MaxTrackPT 4  --Input mergedSample/pyquen_PbPb030-v7.root --MixFile mergedSample/pyquen_PbPb030-v7.root  --IsPbPb030 false --IsGenZ true --nMix 1  --MinHiBin 0 --MaxHiBin 600000  
./finalAnalysis.sh output/$1pyquenPbPb030 4_10  $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11}   --MinZPT 40 --MaxZPT 350 --MinTrackPT 4 --MaxTrackPT 10 --Input mergedSample/pyquen_PbPb030-v7.root --MixFile mergedSample/pyquen_PbPb030-v7.root  --IsPbPb030 false --IsGenZ true --nMix 1  --MinHiBin 0 --MaxHiBin 600000  
hadd -f plots/$1pyquenPbPb030-result.root output/$1pyquenPbPb030-1_2-result.root output/$1pyquenPbPb030-2_4-result.root output/$1pyquenPbPb030-4_10-result.root
hadd -f plots/$1pyquenPbPb030-nosub.root output/$1pyquenPbPb030-1_2-nosub.root output/$1pyquenPbPb030-2_4-nosub.root output/$1pyquenPbPb030-4_10-nosub.root

./finalAnalysis.sh output/$1pyquenPbPbWide030 1_2   $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11}   --MinZPT 40 --MaxZPT 350 --MinTrackPT 1 --MaxTrackPT 2  --Input mergedSample/pyquen_PbPbWide030-v7.root --MixFile mergedSample/pyquen_PbPbWide030-v7.root  --IsPbPbWide030 false --IsGenZ true --nMix 1  --MinHiBin 0 --MaxHiBin 600000  
./finalAnalysis.sh output/$1pyquenPbPbWide030 2_4   $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11}   --MinZPT 40 --MaxZPT 350 --MinTrackPT 2 --MaxTrackPT 4  --Input mergedSample/pyquen_PbPbWide030-v7.root --MixFile mergedSample/pyquen_PbPbWide030-v7.root  --IsPbPbWide030 false --IsGenZ true --nMix 1  --MinHiBin 0 --MaxHiBin 600000  
./finalAnalysis.sh output/$1pyquenPbPbWide030 4_10  $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11}   --MinZPT 40 --MaxZPT 350 --MinTrackPT 4 --MaxTrackPT 10 --Input mergedSample/pyquen_PbPbWide030-v7.root --MixFile mergedSample/pyquen_PbPbWide030-v7.root  --IsPbPbWide030 false --IsGenZ true --nMix 1  --MinHiBin 0 --MaxHiBin 600000  
hadd -f plots/$1pyquenPbPbWide030-result.root output/$1pyquenPbPbWide030-1_2-result.root output/$1pyquenPbPbWide030-2_4-result.root output/$1pyquenPbPbWide030-4_10-result.root
hadd -f plots/$1pyquenPbPbWide030-nosub.root output/$1pyquenPbPbWide030-1_2-nosub.root output/$1pyquenPbPbWide030-2_4-nosub.root output/$1pyquenPbPbWide030-4_10-nosub.root

#./finalAnalysis.sh output/PbPbWide030_y0_1 1_2   $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11} --MinZY 0 --MaxZY 1  --MinZPT 40 --MaxZPT 350 --MinTrackPT 1 --MaxTrackPT 2  --Input mergedSample/pyquen_PbPbWide030-v7.root --MixFile mergedSample/pyquen_PbPbWide030-v7.root  --IsPbPbWide030 true --IsGenZ true --nMix 1
#./finalAnalysis.sh output/PbPbWide030_y0_1 2_4   $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11} --MinZY 0 --MaxZY 1  --MinZPT 40 --MaxZPT 350 --MinTrackPT 2 --MaxTrackPT 4  --Input mergedSample/pyquen_PbPbWide030-v7.root --MixFile mergedSample/pyquen_PbPbWide030-v7.root  --IsPbPbWide030 true --IsGenZ true --nMix 1
#./finalAnalysis.sh output/PbPbWide030_y0_1 4_10  $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11} --MinZY 0 --MaxZY 1  --MinZPT 40 --MaxZPT 350 --MinTrackPT 4 --MaxTrackPT 10 --Input mergedSample/pyquen_PbPbWide030-v7.root --MixFile mergedSample/pyquen_PbPbWide030-v7.root  --IsPbPbWide030 true --IsGenZ true --nMix 1
#hadd -f plots/$1PbPbWide030_y0_1-result.root output/$1PbPbWide030_y0_1-1_2-result.root output/$1PbPbWide030_y0_1-2_4-result.root output/$1PbPbWide030_y0_1-4_10-result.root
#hadd -f plots/$1PbPbWide030_y0_1-nosub.root output/$1PbPbWide030_y0_1-1_2-nosub.root output/$1PbPbWide030_y0_1-2_4-nosub.root output/$1PbPbWide030_y0_1-4_10-nosub.root


#./finalAnalysis.sh output/PbPbWide030_y1_10 1_2   $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11} --MinZY 1 --MaxZY 10  --MinZPT 40 --MaxZPT 350 --MinTrackPT 1 --MaxTrackPT 2  --Input mergedSample/pyquen_PbPbWide030-v7.root --MixFile mergedSample/pyquen_PbPbWide030-v7.root  --IsPbPbWide030 true --IsGenZ true --nMix 1
#./finalAnalysis.sh output/PbPbWide030_y1_10 2_4   $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11} --MinZY 1 --MaxZY 10  --MinZPT 40 --MaxZPT 350 --MinTrackPT 2 --MaxTrackPT 4  --Input mergedSample/pyquen_PbPbWide030-v7.root --MixFile mergedSample/pyquen_PbPbWide030-v7.root  --IsPbPbWide030 true --IsGenZ true --nMix 1
#./finalAnalysis.sh output/PbPbWide030_y1_10 4_10  $2 $3 $4 $5 $6 $7 $8 $9 ${10} ${11} --MinZY 1 --MaxZY 10  --MinZPT 40 --MaxZPT 350 --MinTrackPT 4 --MaxTrackPT 10 --Input mergedSample/pyquen_PbPbWide030-v7.root --MixFile mergedSample/pyquen_PbPbWide030-v7.root  --IsPbPbWide030 true --IsGenZ true --nMix 1
#hadd -f plots/$1PbPbWide030_y1_10-result.root output/$1PbPbWide030_y1_10-1_2-result.root output/$1PbPbWide030_y1_10-2_4-result.root output/$1PbPbWide030_y1_10-4_10-result.root
#hadd -f plots/$1PbPbWide030_y1_10-nosub.root output/$1PbPbWide030_y1_10-1_2-nosub.root output/$1PbPbWide030_y1_10-2_4-nosub.root output/$1PbPbWide030_y1_10-4_10-nosub.root
