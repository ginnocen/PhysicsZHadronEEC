./finalAnalysis.sh output/pp 1_2  --MinTrackPT 1 --MaxTrackPT 2  --Input mergedSample/pp-v6.root --MixFile mergedSample/pp-v6.root  --IsPP true --IsGenZ false --nMix 1
./finalAnalysis.sh output/pp 2_4  --MinTrackPT 2 --MaxTrackPT 4  --Input mergedSample/pp-v6.root --MixFile mergedSample/pp-v6.root  --IsPP true --IsGenZ false --nMix 1
./finalAnalysis.sh output/pp 4_10 --MinTrackPT 4 --MaxTrackPT 10 --Input mergedSample/pp-v6.root --MixFile mergedSample/pp-v6.root  --IsPP true --IsGenZ false --nMix 1
hadd -f plots/pp-result.root output/pp-1_2-result.root output/pp-2_4-result.root output/pp-4_10-result.root

